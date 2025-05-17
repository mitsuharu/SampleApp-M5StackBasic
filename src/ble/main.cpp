#include <Arduino.h>
#include <M5Unified.h>
#include <MyUtil.h>
#include "BLEHandler.h"

// iOS 側のサンプル
// https://github.com/mitsuharu/SampleBLEApp

BLEHandler handler = BLEHandler();

void setup()
{
    auto cfg = M5.config();
    M5.begin(cfg);

    MyUtil::print("Starting BLE work!", 2);
    handler.prepare();
}

void loop()
{
    handler.loop();
}
