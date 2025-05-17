#include <Arduino.h>
#include <M5Unified.h>
#include <MyUtil.h>

#include "M5UnitENV.h"

// https://docs.m5stack.com/ja/unit/ENV%E2%85%A3%20Unit
// https://github.com/m5stack/M5Unit-ENV/blob/master/examples/Unit_ENVIV_M5Core/Unit_ENVIV_M5Core.ino

SHT4X sht4 = SHT4X();
BMP280 bmp = BMP280();

void setup()
{
    auto cfg = M5.config();
    M5.begin(cfg);

    MyUtil::print("ENV-IV Unit");

    if (!sht4.begin(&Wire, SHT40_I2C_ADDR_44, 21, 22, 400000U))
    {
        MyUtil::print("Couldn't find SHT4x");
        while (1)
            delay(1);
    }

    // You can have 3 different precisions, higher precision takes longer
    sht4.setPrecision(SHT4X_HIGH_PRECISION);
    sht4.setHeater(SHT4X_NO_HEATER);

    if (!bmp.begin(&Wire, BMP280_I2C_ADDR, 21, 22, 400000U))
    {
        Serial.println("Couldn't find BMP280");
        while (1)
            delay(1);
    }
    /* Default settings from datasheet. */
    bmp.setSampling(BMP280::MODE_NORMAL,     /* Operating Mode. */
                    BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    BMP280::FILTER_X16,      /* Filtering. */
                    BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop()
{
    // SHT40
    if (sht4.update())
    {
        // 温度
        String temperature = "Temp: " + String(sht4.cTemp) + " degrees C";

        // 湿度
        String humidity = "RH: " + String(sht4.humidity) + " %";

        MyUtil::print("SHT4X");
        MyUtil::print(temperature);
        MyUtil::print(humidity);
    }

    // BMP280
    if (bmp.update())
    {
        // 温度
        String temperature = "Temp: " + String(bmp.cTemp) + " degrees C";

        // 気圧
        String pressure = "Pressure: " + String(bmp.pressure) + " Pa";

        // 高度
        String altitude = "Altitude: " + String(bmp.altitude) + " m";

        MyUtil::print("BMP280");
        MyUtil::print(temperature);
        MyUtil::print(pressure);
        MyUtil::print(altitude);
    }
    delay(1000);
}
