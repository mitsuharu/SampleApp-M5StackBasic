#include <Arduino.h>
#include <M5Unified.h>
#include <M5UnitENV.h>
#include "BLEHandler.h"

// iOS 側のサンプル
// https://github.com/mitsuharu/SampleBLEApp

BLEHandler handler = BLEHandler();
SHT4X sht4 = SHT4X();

void setup()
{
    auto cfg = M5.config();
    M5.begin(cfg);

    M5.Display.setTextSize(2);
    M5.Display.setTextScroll(true);
    M5.Display.setFont(&fonts::efontJA_12);

    // env4
    // core2 の場合 https://github.com/m5stack/M5Unit-ENV/blob/master/examples/Unit_ENVIV_M5Core2/Unit_ENVIV_M5Core2.ino
    if (!sht4.begin(&Wire, SHT40_I2C_ADDR_44, 21, 22, 400000U))
    {
        M5.Display.setTextColor(TFT_YELLOW);
        M5.Display.println(">> couldn't find SHT4x");
        while (1)
            delay(1);
    }
    sht4.setPrecision(SHT4X_HIGH_PRECISION);
    sht4.setHeater(SHT4X_NO_HEATER);

    // BLE
    M5.Display.println(">> start BLE server");
    handler.prepare();
    M5.Display.println(">> Go ready to connect");
}

void loop()
{
    if (sht4.update() && handler.deviceConnected)
    {
        String temperature = String(sht4.cTemp);

        M5.Display.printf(">> temp: %s dC\n", temperature.c_str());

        // 温度
        handler.loop(temperature);
    }
}
