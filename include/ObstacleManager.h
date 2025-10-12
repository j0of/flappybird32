#pragma once

#include "Obstacle.h"
#include "Adafruit_ST7735.h"

class Obstacle;

class ObstacleManager {
public:
    ObstacleManager(const Player &player);
    void reset();

    void tick(float dt);
    void clear(Adafruit_ST7735 &tft) const;
    void draw(Adafruit_ST7735 &tft) const;
    int getPassed() const;
    
    bool playerIntersects() const;
private:
    int passed;
    static constexpr int obstaclesLen = 2;
    Obstacle obstacles[obstaclesLen];
    const Player &player;
    static constexpr int xGap = 100;
};