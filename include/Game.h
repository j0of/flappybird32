#pragma once

#include <Adafruit_ST7735.h>

#include "Player.h"
#include "ObstacleManager.h"
#include "WebController.h"

class Game {
public:
    Game();
    void setup(WebController *wc);
    void loop();
    int getHighScore() const { return highScore; };
    bool checkGameOver() const { return gameOver; };
private:
    void drawCenteredText(const char *s, int16_t y = -1);
    void clearBg();

    Adafruit_ST7735 tft;
    Player player;
    ObstacleManager obstacleManager;
    WebController *webcontroller;
    unsigned long prevTime, currTime;
    float dt;
    int highScore;

    static constexpr float fpsUpdateInterval = 0.5;
    float lastFpsUpdate;

    bool playing;
    bool gameOver;
    bool curBtn, prevBtn;
    bool trigger;
    bool ipShown;
};