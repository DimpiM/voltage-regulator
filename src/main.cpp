#include <Arduino.h>
#include "EncoderHandler.h"
#include "DisplayHandler.h"
#include "WebServerHandler.h"
#include "DacHandler.h"
#include "HelperHandler.h"

EncoderHandler encoder;
DisplayHandler display;
WebServerHandler webServer;
DacHandler dacCh1;
DacHandler dacCh2;
HelperHandler helper;

String ipAddress = "none";
double currentVoltageCh1 = 2.0;
double currentVoltageCh2 = 1.0;
int selectedChannel = 1; // 1 = CH1, 2 = CH2

void handleWebServerChanges();
void handleEncoderChanges();
void handleDACChanges();

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    //helper.scanI2CDevicesInit(8, 9); // SDA_PIN, SCL_PIN

    display.init();
    display.showBootWindow();
    webServer.init();
    ipAddress = webServer.getIpAddress();
    encoder.init(0, 50, (int)(currentVoltageCh1 * 10));
    encoder.invertDirection(true); // Drehrichtung umkehren
    dacCh1.init(0x61, 4095);
    dacCh2.init(0x60, 4095);

    webServer.setVoltageCh1(currentVoltageCh1);
    webServer.setVoltageCh2(currentVoltageCh2);
}

void loop()
{
    handleWebServerChanges();

    handleEncoderChanges();

    display.showMainWindow(currentVoltageCh1, currentVoltageCh2, ipAddress, selectedChannel);

    handleDACChanges();
}

void handleDACChanges()
{
    dacCh1.setVoltage(currentVoltageCh1);
    dacCh2.setVoltage(currentVoltageCh2);
}

void handleWebServerChanges()
{
    webServer.handleClient();

    if (webServer.hasNewVoltage())
    {
        currentVoltageCh1 = webServer.getVoltageCh1();
        currentVoltageCh2 = webServer.getVoltageCh2();
        webServer.resetVoltageFlag(); // Verhindert mehrfaches Auslesen
        int newStart = (selectedChannel == 1) ? (int)(currentVoltageCh1 * 10)
                                              : (int)(currentVoltageCh2 * 10);
        encoder.setValue(newStart);
    }
}

void handleEncoderChanges()
{
    int value = encoder.update();
    // Reagiere auf Taster → Kanal wechseln
    if (encoder.isButtonPressed())
    {
        selectedChannel = (selectedChannel == 1) ? 2 : 1;

        // Encoder neu initialisieren auf neuen Wert
        int newStart = (selectedChannel == 1) ? (int)(currentVoltageCh1 * 10)
                                              : (int)(currentVoltageCh2 * 10);
        encoder.setValue(newStart);
    }

    double newVoltage = value / 10.0;
    if (selectedChannel == 1 && newVoltage != currentVoltageCh1)
    {
        currentVoltageCh1 = newVoltage;
        webServer.setVoltageCh1(currentVoltageCh1);
        //Serial.println("CH1 Voltage: " + String(currentVoltageCh1));
    }
    else if (selectedChannel == 2 && newVoltage != currentVoltageCh2)
    {
        currentVoltageCh2 = newVoltage;
        webServer.setVoltageCh2(currentVoltageCh2);
        Serial.println("CH2 Voltage: " + String(currentVoltageCh2));
    }

    // int newStart = (selectedChannel == 1) ? (int)(currentVoltageCh1 * 10)
    //                                       : (int)(currentVoltageCh2 * 10);
    // encoder.setValue(newStart);
}