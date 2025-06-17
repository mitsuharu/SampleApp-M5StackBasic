#pragma once

#include <Arduino.h>

class BLECharacteristic;

class BLEHandler
{
    friend class BLEHandlerCallbacks;

public:
    BLEHandler();
    void prepare();
    void loop(String message);
    bool deviceConnected;

private:
    BLECharacteristic *pCharacteristic;
    void setDeviceConnected(bool connected);
};
