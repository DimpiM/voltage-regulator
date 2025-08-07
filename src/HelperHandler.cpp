#include <Wire.h>
#include <ArduinoJson.h>
#include "HelperHandler.h"

void HelperHandler::scanI2CDevicesInit(int SDA_PIN, int SCL_PIN)
{
    Wire.begin(SDA_PIN, SCL_PIN);
}

void HelperHandler::scanI2CDevices()
{
    byte error, address;
    int nDevices = 0;

    Serial.println("Scanning...");

    for (address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0)
        {
            Serial.print("I2C device found at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.print(address, HEX);
            Serial.println("  !");

            nDevices++;
        }
        else if (error == 4)
        {
            Serial.print("Unknown error at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");
}