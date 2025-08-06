#include <Wire.h>
#include "SH1106Wire.h"
#include "DisplayHandler.h"

#define SDA_PIN 8
#define SCL_PIN 9

static SH1106Wire display(0x3C, SDA_PIN, SCL_PIN);

void DisplayHandler::init()
{
    display.init();
    display.setContrast(255);
    display.clear();
}


void DisplayHandler::showMainWindow(double ch1Voltage, double ch2Voltage, String ipAddress, int currentSelectedCh)
{
    display.clear();

    const int chXPosition = 5;
    const int rectHeight = 22;
    // CH1 Label
    display.setFont(ArialMT_Plain_10);
    display.drawString(chXPosition, 7, "CH1:");

    // CH1 Value
    display.setFont(ArialMT_Plain_16);
    String ch1Text = String(ch1Voltage, 1) + " V";
    display.drawString(chXPosition + 30, 2, ch1Text);

    // CH1 Pfeil wenn ausgewählt
    if (currentSelectedCh == 1)
    {
        int widthText = chXPosition + 30 + display.getStringWidth(ch1Text) + 4;
        display.drawRect(0,0, widthText, rectHeight);
    }

    // CH2 Label
    display.setFont(ArialMT_Plain_10);
    display.drawString(chXPosition, 27, "CH2:");

    // CH2 Value
    display.setFont(ArialMT_Plain_16);
    String ch2Text = String(ch2Voltage, 1) + " V";
    display.drawString(chXPosition + 30, 22, ch2Text);

    // CH2 Pfeil wenn ausgewählt
    if (currentSelectedCh == 2)
    {
        int widthText = chXPosition + 30 + display.getStringWidth(ch2Text) + 4;
        display.drawRect(0,22, widthText, rectHeight);
    }

    // IP-Adresse unten zentriert
    display.setFont(ArialMT_Plain_10);
    int16_t ipWidth = display.getStringWidth(ipAddress);
    int16_t ipX = (display.getWidth() - ipWidth) / 2;
    int16_t ipY = display.getHeight() - 10;  // ganz unten, ca. 10 Pixel hoch
    display.drawString(ipX, ipY, ipAddress);

    display.display();
}

void DisplayHandler::showBootWindow()
{
    String text = "Initialize"; // kannst du nach Wunsch ändern
    display.clear();
    display.setFont(ArialMT_Plain_16);

    int textWidth = display.getStringWidth(text);
    int x = (display.getWidth() - textWidth) / 2;
    int y = (display.getHeight() - 16) / 2; // Vertikal zentrieren bei 16px Schrift

    display.drawString(x, y, text);
    display.display();
}