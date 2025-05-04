#pragma once

class BLECharacteristic;

class BLEHandler
{
    friend class BLEHandlerCallbacks;

public:
    BLEHandler();
    void prepare();
    void loop();

private:
    BLECharacteristic *pCharacteristic;
    bool deviceConnected;
    void setDeviceConnected(bool connected);
};
