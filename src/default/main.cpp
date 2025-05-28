#include <Arduino.h>
#include <M5Unified.h>

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);

  M5.Display.setTextSize(2);
  M5.Display.setTextScroll(true);
  M5.Display.setFont(&fonts::efontJA_12);

  M5.Display.println(">> Hello world!");
  M5.Display.println(">> Press any button");
}

void loop()
{
  M5.update(); // タッチやボタン状態の更新

  if (M5.BtnA.wasPressed())
  {
    M5.Display.setTextColor(TFT_MAGENTA);
    M5.Display.println("<< Button A was pressed.");
  }

  if (M5.BtnB.wasPressed())
  {
    M5.Display.setTextColor(TFT_YELLOW);
    M5.Display.println("<< Button B was pressed.");
  }

  if (M5.BtnC.wasPressed())
  {
    M5.Display.setTextColor(TFT_CYAN);
    M5.Display.println("<< Button C was pressed.");
  }

  delay(10);
}
