#pragma once

#include <Arduino.h>
#include <Wifi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

class WebController {
public:
    WebController();
    void loop();
    void setup();
    void broadcastFPS(float fps);
    void broadcastNewhi(int newhi, int oldhi);
    void broadcastGameover();
    String getIp() { return String(WiFi.softAPIP().toString()); };
    bool remoteTrigger;
private:
    static void loopTask(void *params);
    void onWebSocketEvent(uint8_t clientNum, WStype_t type, unsigned char *payload, size_t length);
    void onIndexRequest(AsyncWebServerRequest *request);
    String generateHTML();

    AsyncWebServer server;
    WebSocketsServer ws;
    char msgBuf[16];

    String header;
};