#include <Arduino.h>
#include <Wire.h>
#include "SH1106Wire.h"

#define SDA_PIN 8
#define SCL_PIN 9

// Display initialisieren (Adresse, SDA, SCL)
SH1106Wire display(0x3C, SDA_PIN, SCL_PIN);
// put function declarations here:

String message = "Hallo ESP32-C3! ";
int16_t x = 0;     // Startposition
int16_t textWidth; // Breite des Texts in Pixel

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);

    // Display initialisieren
    display.init();

    // Optional: Kontrast setzen (0–255)
    display.setContrast(255);

    // Display löschen
    display.clear();

    // Beispieltext anzeigen
    display.setFont(ArialMT_Plain_16); // Großer Text
    textWidth = display.getStringWidth(message);
    x = display.getWidth(); // Startposition rechts außerhalb des Displays
}

void loop()
{
    display.clear();

    // Text anzeigen an Position x
    display.drawString(x, 20, message);
    display.display();

    // Position verschieben (nach links)
    x -= 2;

    // Wenn Text ganz links raus ist → zurücksetzen
    if (x + textWidth < 0)
    {
        x = display.getWidth();
    }

    delay(50); // Geschwindigkeit der Animation
}
