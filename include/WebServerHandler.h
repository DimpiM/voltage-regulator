#ifndef WEBSERVER_HANDLER_H
#define WEBSERVER_HANDLER_H

#include <ArduinoJson.h>   // für DynamicJsonDocument

class WebServerHandler
{
public:
    void init();
    void handleClient();
    String getIpAddress() const;
    double getVoltageCh1() const;
    double getVoltageCh2() const;
    void setVoltageCh1(double voltage);
    void setVoltageCh2(double voltage);
    bool hasNewVoltage();
    void resetVoltageFlag();
private:
    void serveFile(const char *path, const char *contentType);
    void handlePostVoltage();
    void handleGetVoltage();
    DynamicJsonDocument* checkResponse();

    double voltageCh1 = 0.0;
    double voltageCh2 = 0.0;
    bool newVoltageReceived = false;

    String ipAddress;
};

#endif