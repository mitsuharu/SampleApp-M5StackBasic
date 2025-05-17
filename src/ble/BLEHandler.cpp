// BLEHandler.cpp
#include "BLEHandler.h"
#include <Arduino.h>
#include <MyUtil.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define DEVICE_NAME "M5Stack-SampleApp"
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// 実装参照
// https://github.com/espressif/arduino-esp32/blob/master/libraries/BLE/examples/Server/Server.ino

class BLEHandlerCallbacks : public BLEServerCallbacks
{
public:
    BLEHandlerCallbacks(BLEHandler *handler) : handler(handler) {}

    void onConnect(BLEServer *pServer) override
    {
        handler->setDeviceConnected(true);
        MyUtil::print("Device connected", 2);
    }

    void onDisconnect(BLEServer *pServer) override
    {
        handler->setDeviceConnected(false);
        MyUtil::print("Device disconnected", 2);
    }

private:
    BLEHandler *handler;
};

BLEHandler::BLEHandler() {}

void BLEHandler::setDeviceConnected(bool connected)
{
    deviceConnected = connected;
}

void BLEHandler::prepare()
{
    BLEDevice::init(DEVICE_NAME);
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new BLEHandlerCallbacks(this));

    BLEService *pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY);

    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setValue("Hello World says Neil");
    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

    MyUtil::print("wait to connect", 2);
}

void BLEHandler::loop()
{
    if (deviceConnected)
    {
        char buf[16];
        int r = random(0, 1000);
        sprintf(buf, "RND:%d", r);
        pCharacteristic->setValue(buf);
        pCharacteristic->notify();
        MyUtil::print(buf, 2);
        delay(1000);
    }
}
