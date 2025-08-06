#ifndef ENCODER_HANDLER_H
#define ENCODER_HANDLER_H

#include <Arduino.h>

class EncoderHandler
{
public:
    void init(int minVal, int maxVal, int startVal = 0); // Bereich festlegen
    int update();                                        // Encoder lesen, Wert anpassen
    int getValue();                                      // aktuellen Wert abfragen
    void setValue(int val);
    void invertDirection(bool inverted);
    bool isButtonPressed();

private:
    int minValue = 0;
    int maxValue = 100;
    int currentValue = 0;
    long oldPosition = 0;
    bool inverted = false;

    bool lastButtonState = HIGH;
    bool buttonEvent = false;
};

#endif