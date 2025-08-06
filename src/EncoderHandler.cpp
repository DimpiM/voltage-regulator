#include <Arduino.h>
#include <Encoder.h>
#include "EncoderHandler.h"

#define ENCODER_CLK 2
#define ENCODER_DT 3
#define ENCODER_SW 4

static Encoder myEncoder(ENCODER_CLK, ENCODER_DT);
// int minValue = 0;
// int maxValue = 100;
// int currentValue = 0;
// long oldPosition = 0;
// bool inverted = false;

// bool lastButtonState = HIGH;
// bool buttonEvent = false;

void EncoderHandler::init(int minVal, int maxVal, int startVal)
{
    pinMode(ENCODER_SW, INPUT_PULLUP);
    minValue = minVal;
    maxValue = maxVal;

    if (startVal < minVal) startVal = minVal;
    if (startVal > maxVal) startVal = maxVal;

    currentValue = startVal;

    oldPosition = myEncoder.read() / 2;
}

void EncoderHandler::invertDirection(bool shouldInvert)
{
    inverted = shouldInvert;
}

int EncoderHandler::update()
{
    // --- Drehencoder ---
    long newPosition = myEncoder.read() / 2;
    // Drehrungserkennung
    int delta = newPosition - oldPosition;
    if (inverted)
        delta *= -1;

    if (delta != 0)
    {

        // Begrenze Schrittweite
        if (delta > 1) delta = 1;
        if (delta < -1) delta = -1;

        // Neuen Wert berechnen mit Begrenzung
        int newValue = currentValue + delta;
        if (newValue >= minValue && newValue <= maxValue)
        {
            currentValue = newValue;
        }

        oldPosition = newPosition;
    }

    // --- Taster-Entprellung ---
    bool currentButtonState = digitalRead(ENCODER_SW);
    if (lastButtonState == HIGH && currentButtonState == LOW)
    {
        buttonEvent = true; // Nur bei Flanke LOW → gedrückt
    }
    lastButtonState = currentButtonState;

    delay(5); // debounce
    return currentValue;
}

int EncoderHandler::getValue()
{
    return currentValue;
}

void EncoderHandler::setValue(int val)
{
    if (val < minValue) val = minValue;
    if (val > maxValue) val = maxValue;

    currentValue = val;
}

bool EncoderHandler::isButtonPressed()
{
    if (buttonEvent)
    {
        buttonEvent = false;
        return true;
    }
    return false;
}