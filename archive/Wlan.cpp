#include <Arduino.h>
#include <WiFiManager.h>
#include <WebServer.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>

#define LED_BUILTIN 8
WebServer server(80);

// put function declarations here:
void serveFile(const char *, const char *);
void handleLedPost();

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    Serial.println("Starting...");
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    WiFiManager wm;
    // reset settings - wipe stored credentials for testing, these are stored by the esp library
    // wm.resetSettings();
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

    // === HTTP-Routen registrieren ===
    server.on("/", []() { serveFile("/index.html", "text/html"); });
    server.on("/style.css", []() { serveFile("/style.css", "text/css"); });
    server.on("/script.js", []() { serveFile("/script.js", "application/javascript"); });
    server.on("/led", HTTP_POST, handleLedPost);

    Serial.println("HTTP-Server starting...");
    server.begin();
    Serial.println("HTTP-Server started");
}

void loop()
{
    // put your main code here, to run repeatedly:
    server.handleClient();
}

void serveFile(const char *path, const char *contentType)
{
    File file = SPIFFS.open(path, "r");
    if (!file)
    {
        server.send(404, "text/plain", "Datei nicht gefunden");
        return;
    }
    server.streamFile(file, contentType);
    file.close();
}

void handleLedPost()
{
    if (server.hasArg("plain") == false)
    {
        server.send(400, "text/plain", "Kein Body empfangen");
        return;
    }

    String body = server.arg("plain");
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, body);

    if (error)
    {
        server.send(400, "text/plain", "Ungültiges JSON");
        return;
    }

    bool ledOn = doc["on"];
    digitalWrite(LED_BUILTIN, ledOn ? LOW : HIGH);

    String status = ledOn ? "LED ist AN" : "LED ist AUS";
    server.send(200, "text/plain", status);
}

// put function definitions here:
// void blink(long interval)
// {
//     long currentMillis = millis();
//     if (currentMillis - lastMillis >= interval)
//     {
//         lastMillis = currentMillis;
//         digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
//         // Serial.println("Test");
//     }
// }