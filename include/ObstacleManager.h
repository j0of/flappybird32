#pragma once

#include "Obstacle.h"
#include "Adafruit_ST7735.h"

class Obstacle;

class ObstacleManager {
public:
    ObstacleManager(Adafruit_ST7735 *_tft, const Player &_player);
    void reset();

    void tick(float dt);
    void clear() const;
    void draw() const;
    int getScore() const { return score; };
    
    bool playerIntersects() const;
private:
    Adafruit_ST7735 *tft;
    int score;
    static constexpr int obstaclesLen = 2;
    Obstacle *obstacles[obstaclesLen];
    const Player &player;
    static constexpr int xGap = 100;
};