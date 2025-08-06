#ifndef DISPLAY_HANDLER_H
#define DISPLAY_HANDLER_H

#include <Arduino.h>

class DisplayHandler
{
public:
    void init();
    void showBootWindow();
    void showMainWindow(double ch1Voltage, double ch2Voltage, String ipAddress, int currentSelectedCh);
};

#endif