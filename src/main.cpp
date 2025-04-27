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
  // put your main code here, to run repeatedly:
}
