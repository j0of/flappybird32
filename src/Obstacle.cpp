#include "Obstacle.h"

#include <Arduino.h>
#include "magicNumbers.h"

Obstacle::Obstacle(int xPos) : x(xPos) {
    reset(xPos);
}

void Obstacle::reset(int xPos) {
    x = xPos;
    int rand = random(5, GRASS_START - yGap - 5);
    y2 = rand + yGap;
    h1 = rand;
    h2 = GRASS_START - (rand + yGap);
    scored = false;
}

void Obstacle::tick(float dt) {
    x -= speed * dt;
}

void Obstacle::clear(Adafruit_ST7735 &tft) const {
    tft.fillRect(x, y1, w, h1, COLOUR_SKY);
    tft.fillRect(x, y2, w, h2, COLOUR_SKY);
}

void Obstacle::draw(Adafruit_ST7735 &tft) const {
    tft.fillRect(x, y1, w, h1, COLOUR_PIPE);
    tft.fillRect(x, y2, w, h2, COLOUR_PIPE);
}

