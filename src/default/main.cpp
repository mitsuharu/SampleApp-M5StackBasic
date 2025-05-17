#include <Arduino.h>
#include <M5Unified.h>
#include <MyUtil.h>

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);

  MyUtil::print("Hello world!");
}

void loop()
{
  M5.update(); // タッチやボタン状態の更新

  if (M5.BtnA.wasPressed())
  {
    MyUtil::print("Button A pressed!");
  }

  if (M5.BtnB.wasPressed())
  {
    MyUtil::print("Button B pressed!");
  }

  if (M5.BtnC.wasPressed())
  {
    MyUtil::print("Button C pressed!");
  }

  delay(10);
}
