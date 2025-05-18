#include <Arduino.h>
#include <M5Unified.h>
#include <M5ModuleLLM.h>

// ref
// https://docs.m5stack.com/en/guide/llm/llm/arduino_api
// https://zenn.dev/yh1224/articles/7db32ee02aa9956bf

M5ModuleLLM module_llm;
M5ModuleLLM_VoiceAssistant voice_assistant(&module_llm);
// String llm_work_id;

String audio_work_id;
String tts_work_id;
String asr_work_id;

/* Must be capitalized */
String wake_up_keyword = "OK";
String wake_up_language = "en_US";
String kws_work_id;
String vad_work_id;
String whisper_work_id;
String llm_work_id;
String melotts_work_id;

/* On ASR data callback */
void on_asr_data_input(String data, bool isFinish, int index)
{
    M5.Display.setTextColor(TFT_GREEN, TFT_BLACK);
    M5.Display.printf(">> %s\n", data.c_str());

    /* If ASR data is finish */
    if (isFinish)
    {
        M5.Display.setTextColor(TFT_YELLOW, TFT_BLACK);
        M5.Display.print(">> ");
    }
};

/* On LLM data callback */
void on_llm_data_input(String data, bool isFinish, int index)
{
    M5.Display.print(data);

    /* If LLM data is finish */
    if (isFinish)
    {
        M5.Display.print("\n");
    }
};

void setup()
{
    auto cfg = M5.config();
    M5.begin(cfg);

    M5.Display.setTextSize(2);
    M5.Display.setTextScroll(true);
    M5.Display.setFont(&fonts::efontJA_12);

    M5.Display.println("LLM start!");

    int band = 115200;
    int rxd = M5.getPin(m5::pin_name_t::port_c_rxd);
    int txd = M5.getPin(m5::pin_name_t::port_c_txd);
    Serial2.begin(115200, SERIAL_8N1, rxd, txd);

    module_llm.begin(&Serial2);

    /* Make sure module is connected */
    M5.Display.println(">> Check ModuleLLM connection..");
    while (1)
    {
        if (module_llm.checkConnection())
        {
            break;
        }
    }

    /* Reset ModuleLLM */
    M5.Display.println(">> Reset ModuleLLM..");
    module_llm.sys.reset();

    // /* Setup LLM module and save returned work id */
    // M5.Display.println(">> Setup llm..");
    // llm_work_id = module_llm.llm.setup();
    // String question = "技術書典って知ってる？";
    // M5.Display.printf("<< %s\n", question.c_str());
    // String response = "";
    // module_llm.llm.inferenceAndWaitResult(
    //     llm_work_id,
    //     question.c_str(),
    //     [&](String &result)
    //     {
    //         response += result;
    //     });
    // M5.Display.printf(">> %s\n", response.c_str());

    // Setup Audio
    M5.Display.println(">> Setup Audio..");
    audio_work_id = module_llm.audio.setup();
    if (audio_work_id.isEmpty())
    {
        // エラー
        M5.Display.println(">> failed to set up Audio\n");
    }

    // /* Begin voice assistant preset */
    // M5.Display.printf(">> Begin voice assistant..\n");
    // int ret = voice_assistant.begin("HELLO");
    // if (ret != MODULE_LLM_OK)
    // {
    //     while (1)
    //     {
    //         M5.Display.setTextColor(TFT_RED);
    //         M5.Display.printf(">> Begin voice assistant failed\n");
    //     }
    // }

    // /* Register on ASR data callback function */
    // voice_assistant.onAsrDataInput(on_asr_data_input);

    // /* Register on LLM data callback function */
    // voice_assistant.onLlmDataInput(on_llm_data_input);

    // M5.Display.printf(">> Voice assistant ready\n");

    // // Setup TTS
    // M5.Display.println(">> Setup TTS..\n");
    // m5_module_llm::ApiTtsSetupConfig_t tts_config;
    // tts_work_id = module_llm.tts.setup(tts_config, "tts_setup", "ja-JP");
    // if (tts_work_id.isEmpty())
    // {
    //     // エラー
    //     M5.Display.println(">> failed to set up TTS\n");
    // }

    // ウェイクワードを設定する
    M5.Display.println(">> Setup kws..");
    m5_module_llm::ApiKwsSetupConfig_t kws_config;
    kws_config.kws = wake_up_keyword;
    kws_work_id = module_llm.kws.setup(kws_config, "kws_setup", wake_up_language);
    if (kws_work_id.isEmpty())
    {
        // エラー
        M5.Display.println(">> failed to set up kws");
        while (1)
            ;
    }

    // 音声活動検出（VAD: Voice Activity Detection） を行う
    M5.Display.println(">> Setup vad..");
    m5_module_llm::ApiVadSetupConfig_t vad_config;
    vad_config.input = {"sys.pcm"};
    vad_work_id = module_llm.vad.setup(vad_config, "vad_setup");
    if (vad_work_id.isEmpty())
    {
        // エラー
        M5.Display.println(">> failed to set up vad");
        while (1)
            ;
    }

    // whisper で音声からテキストを受け取る
    M5.Display.println(">> Setup whisper..");
    m5_module_llm::ApiWhisperSetupConfig_t whisper_config;
    whisper_config.model = "whisper-tiny";
    whisper_config.input = {"sys.pcm", kws_work_id, vad_work_id};
    whisper_config.language = "ja";
    whisper_work_id = module_llm.whisper.setup(whisper_config, "whisper_setup");
    if (whisper_work_id.isEmpty())
    {
        // エラー
        M5.Display.println(">> failed to set up whisper");
        while (1)
            ;
    }

    // LLM を設定する
    M5.Display.println(">> Setup llm..");
    m5_module_llm::ApiLlmSetupConfig_t llm_config;
    llm_config.max_token_len = 1023;
    llm_config.model = "qwen2.5-1.5B-p256-ax630c";
    llm_config.input = {"llm.utf-8", whisper_work_id, kws_work_id};
    llm_config.enoutput = true;
    llm_work_id = module_llm.llm.setup(llm_config);
    if (llm_work_id.isEmpty())
    {
        // エラー
        M5.Display.println(">> failed to set up llm");
        while (1)
            ;
    }

    M5.Display.println(">> Setup melotts..");
    m5_module_llm::ApiMelottsSetupConfig_t melotts_config;
    // IMPORTANT: Verify the correct Japanese MeloTTS model name for your M5Module-LLM.
    // Using a placeholder "melotts_ja". Common ones might be "melotts-ja" or similar.
    melotts_config.model = "melotts-ja-jp"; // Placeholder - VERIFY THIS MODEL NAME!
    melotts_config.input = {llm_work_id};
    melotts_config.response_format = "sys.pcm";
    melotts_config.enaudio = true;
    melotts_config.enoutput = false;
    melotts_work_id = module_llm.melotts.setup(melotts_config, "melotts", "ja_JP");
    if (melotts_work_id.isEmpty())
    {
        // エラー
        M5.Display.println(">> failed to set up melotts");
        while (1)
            ;
    }

    // M5.Display.println(">> Setup ok\n>> Say \"%s\" to wakeup\n", wake_up_keyword.c_str());

    M5.Display.println(">> Stand by OK");
}

void loop()
{
    // voice_assistant.update();

    /* Update ModuleLLM */
    module_llm.update();

    /* Handle module response messages */
    for (auto &msg : module_llm.msg.responseMsgList)
    {
        if (msg.work_id == kws_work_id)
        {
            // ウェイクワードを検出した
            M5.Display.setTextColor(TFT_GREENYELLOW);
            M5.Display.printf(">> キーワード検出\n");
            // M5.Display.printf(">> キーワード検出: %s\n", wake_up_keyword.c_str());
        }
        else if (msg.work_id == whisper_work_id && msg.object == "asr.utf-8")
        {
            // whisper で音声からテキストを受け取る
            JsonDocument doc;
            deserializeJson(doc, msg.raw_msg);
            String asr_result = doc["data"].as<String>();

            M5.Display.setTextColor(TFT_YELLOW);
            M5.Display.printf("<< %s\n", asr_result.c_str());
        }
        else if (msg.work_id == llm_work_id && msg.object == "llm.utf-8.stream")
        {
            /* Parse message json and get ASR result */
            JsonDocument doc;
            deserializeJson(doc, msg.raw_msg);
            //                String llm_result = doc["data"].as<String>();
            String llm_result = doc["data"]["delta"].as<String>();

            M5.Display.setTextColor(TFT_WHITE);
            M5.Display.printf("%s\n", llm_result.c_str());
        }
    }

    /* Clear handled messages */
    module_llm.msg.responseMsgList.clear();
}
