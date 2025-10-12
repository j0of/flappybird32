#pragma once

#include <Adafruit_ST7735.h>
#include "Player.h"

class ObstacleManager;

class Obstacle {
    friend class ObstacleManager;
public:
    Obstacle(int xPos = 0);
private:
    void reset(int xPos);

    void tick(float dt);
    void clear(Adafruit_ST7735 &tft) const;
    void draw(Adafruit_ST7735 &tft) const;

    static constexpr int w = 25;
    float x;
    int y2;
    int h1, h2;
    bool scored;

    static constexpr int y1 = 0;
    static constexpr int speed = 80;
    static constexpr int yGap = 50;
};