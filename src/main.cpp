#include <Arduino.h>
#include "EncoderHandler.h"
#include "DisplayHandler.h"

EncoderHandler encoder;
DisplayHandler display;

double currentVoltageCh1 = 2.1;
double currentVoltageCh2 = 1.2;
int selectedChannel = 1; // 1 = CH1, 2 = CH2

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);

    display.init();
    display.showBootWindow();
    encoder.init(0, 50, (int)(currentVoltageCh1 * 10));
    encoder.invertDirection(true); // Drehrichtung umkehren

    delay(3000); // Kurze Pause nach dem Booten
}

void loop()
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
        Serial.println("CH1 Voltage: " + String(currentVoltageCh1));
    }
    else if (selectedChannel == 2 && newVoltage != currentVoltageCh2)
    {
        currentVoltageCh2 = newVoltage;
        Serial.println("CH2 Voltage: " + String(currentVoltageCh2));
    }

    display.showMainWindow(currentVoltageCh1, currentVoltageCh2, "192.168.0.123", selectedChannel);
}