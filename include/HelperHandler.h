#ifndef HELPER_HANDLER_H
#define HELPER_HANDLER_H

class HelperHandler
{
public:
    void scanI2CDevicesInit(int SDA_PIN, int SCL_PIN);
    void scanI2CDevices();
};

#endif