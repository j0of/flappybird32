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
    void broadcastHighscore(int highscore);
    void broadcastFPS(float fps);
    String getIp() { return String(WiFi.softAPIP().toString()); };
    bool remoteTrigger;
private:
    static void loopTask(void *params);
    void onWebSocketEvent(uint8_t clientNum, WStype_t type, unsigned char *payload, size_t length);
    void onIndexRequest(AsyncWebServerRequest *request);
    String generateHTML();

    static constexpr char *ssid = "flappybird32 remote control";
    static constexpr char *password = "joof.sleeps";
    static constexpr int httpPort = 80;
    static constexpr int tcpPort = 1337;

    AsyncWebServer server;
    WebSocketsServer ws;
    char msgBuf[10];

    String header;
};