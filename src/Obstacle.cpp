#include "Obstacle.h"

#include <Arduino.h>
#include "config.h"

Obstacle::Obstacle(Adafruit_ST7735 *_tft, int _x) : tft(_tft), x(_x) {
    reset(_x);
}

void Obstacle::reset(int xPos) {
    x = xPos;
    int rand = random(5, GROUND_START - yGap - 5);
    y2 = rand + yGap;
    h1 = rand;
    h2 = GROUND_START - (rand + yGap);
    scored = false;
}

void Obstacle::tick(float dt) {
    x -= speed * dt;
}

void Obstacle::clear() const {
    tft->fillRect(x, y1, w, h1, COLOUR_SKY);
    tft->fillRect(x, y2, w, h2, COLOUR_SKY);
}

void Obstacle::draw() const {
    tft->fillRect(x, y1, w, h1, COLOUR_PIPE);
    tft->fillRect(x, y2, w, h2, COLOUR_PIPE);
}

