#include <Arduino.h>
#include <WiFiManager.h>

#define LED_BUILTIN 8
long lastMillis;

// put function declarations here:
void blink(long);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  WiFiManager wm;
  // reset settings - wipe stored credentials for testing, these are stored by the esp library
  wm.resetSettings();
  bool res;
  res = wm.autoConnect("VoltageRegulator", "123456789");
  if (!res)
  {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else
  {
    Serial.println("connected...yeey :)");
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  blink(500);
}

// put function definitions here:
void blink(long interval)
{
  long currentMillis = millis();
  if (currentMillis - lastMillis >= interval)
  {
    lastMillis = currentMillis;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    // Serial.println("Test");
  }
}