#ifndef DAC_HANDLER_H
#define DAC_HANDLER_H

#include <ArduinoJson.h>  

class DacHandler
{
public:
    void init(uint8_t i2cAddress, int resolution);
    void setVoltage(double voltage);
    void setResolution(int newResolution);
private:
    int resolution = 4096;
    double currentVoltage = 0.0;
};

#endif