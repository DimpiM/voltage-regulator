#include <Arduino.h>
#include <WiFiManager.h>
#include <WebServer.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>
#include "WebServerHandler.h"

static WebServer server(80);

void WebServerHandler::init()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    WiFiManager wm;
    if (!wm.autoConnect("VoltageRegulator", "123456789"))
    {
        Serial.println("Keine Verbindung, starte AP-Modus...");

        ipAddress = "AP-Mode: " + WiFi.softAPIP().toString();
    }
    else
    {
        isConnected = true;
        ipAddress = WiFi.localIP().toString();
        Serial.println("connected... IP-Adresse: " + ipAddress);
    }

    if (isConnected)
    {
        // === HTTP-Routen registrieren ===
        server.on("/", [this]()
                  { serveFile("/index.html", "text/html"); });
        server.on("/style.css", [this]()
                  { serveFile("/style.css", "text/css"); });
        server.on("/script.js", [this]()
                  { serveFile("/script.js", "application/javascript"); });
        server.on("/voltage", HTTP_POST, [this]()
                  { handlePostVoltage(); });
        server.on("/voltage", HTTP_GET, [this]()
                  { handleGetVoltage(); });

        Serial.println("HTTP-Server starting...");
        server.begin();
        Serial.println("HTTP-Server started");
    }
}

void WebServerHandler::handleClient()
{
    if (isConnected)
    {
        server.handleClient();
    }
}

String WebServerHandler::getIpAddress() const
{
    return ipAddress;
}

void WebServerHandler::serveFile(const char *path, const char *contentType)
{
    File file = SPIFFS.open(path, "r");
    if (!file)
    {
        server.send(404, "text/plain", "File Not Found");
        return;
    }

    server.streamFile(file, contentType);
    file.close();
}

void WebServerHandler::handlePostVoltage()
{
    DynamicJsonDocument *json = checkResponse();
    if (json)
    {

        voltageCh1 = (*json)["voltageCh1"];
        voltageCh2 = (*json)["voltageCh2"];
        newVoltageReceived = true;

        // Here you would typically handle the received voltages
        Serial.printf("CH1 Voltage: %.2f V, CH2 Voltage: %.2f V\n", voltageCh1, voltageCh2);

        server.send(200, "text/plain", "Voltages Updated");

        delete json; // Clean up the DynamicJsonDocument
    }
}

void WebServerHandler::handleGetVoltage()
{
    DynamicJsonDocument response(256);
    response["voltageCh1"] = voltageCh1;
    response["voltageCh2"] = voltageCh2;

    String jsonResponse;
    serializeJson(response, jsonResponse);

    server.send(200, "application/json", jsonResponse);
}

DynamicJsonDocument *WebServerHandler::checkResponse()
{
    if (server.hasArg("plain") == false)
    {
        server.send(400, "text/plain", "No Body Received");
        return nullptr;
    }

    String body = server.arg("plain");

    // Parse JSON
    DynamicJsonDocument *doc = new DynamicJsonDocument(1024);
    DeserializationError error = deserializeJson(*doc, body);
    if (error)
    {
        server.send(400, "text/plain", "Invalid JSON");
        delete doc; // Clean up if deserialization fails
        return nullptr;
    }

    return doc;
}

double WebServerHandler::getVoltageCh1() const
{
    return voltageCh1;
}

double WebServerHandler::getVoltageCh2() const
{
    return voltageCh2;
}

bool WebServerHandler::hasNewVoltage()
{
    return newVoltageReceived;
}

void WebServerHandler::resetVoltageFlag()
{
    newVoltageReceived = false;
}

void WebServerHandler::setVoltageCh1(double voltage)
{
    voltageCh1 = voltage;
}

void WebServerHandler::setVoltageCh2(double voltage)
{
    voltageCh2 = voltage;
}
