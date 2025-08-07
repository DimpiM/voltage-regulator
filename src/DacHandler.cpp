#include <Wire.h>
#include "Adafruit_MCP4725.h"
#include "DacHandler.h"

static Adafruit_MCP4725 dac;

void DacHandler::init(uint8_t i2cAddress, int resolution)
{
    if (!dac.begin(i2cAddress))
    {
        Serial.println("Failed to initialize MCP4725 DAC!");
    }
    this->resolution = resolution;
}

void DacHandler::setVoltage(double voltage)
{
    if(currentVoltage != voltage)
    {
        currentVoltage = voltage;
        // Convert voltage to the range of the DAC (0-5V for MCP4725)
        uint16_t output = static_cast<uint16_t>((voltage  * resolution) / 5.0);
        Serial.println("Setting voltage to: " + String(voltage) + "V - " + String(output));
        // Set the voltage on the DAC
        if (!dac.setVoltage(output, false))
        {
            Serial.println("Failed to set voltage on MCP4725 DAC!");
        }
    }
}

void DacHandler::setResolution(int newResolution)
{
    if (newResolution > 0)
    {
        resolution = newResolution;
    }
    else
    {
        Serial.println("Invalid resolution. Must be greater than 0.");
    }
}