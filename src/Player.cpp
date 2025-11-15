#include "Player.h"

#include "config.h"

Player::Player(Adafruit_ST7735 &_tft) : tft(_tft), y(initY) {}

void Player::reset() {
    y = initY;
    vel = 0;
    jumpTimer = 0;
}

void Player::tick(float dt) {
    vel += gravity * dt;

    if (jumpTimer > 0)
        jumpTimer -= dt;

    if (vel > maxVel)
        vel = maxVel;

    if (y + (vel * dt) > GROUND_START - r - 3) {
        grounded = true;
        y = GROUND_START - r - 3;
        vel = 0;
    } else {
        grounded = false;
    }

    y += vel * dt;
}

void Player::clear() const {
    tft.drawCircle(x, y, r + 1, COLOUR_SKY);
    tft.fillCircle(x, y, r, COLOUR_SKY);
}

void Player::draw() const {
    tft.fillCircle(x, y, r, COLOUR_BIRD);
    tft.drawCircle(x, y, r + 1, ST7735_BLACK);
}

void Player::jump() {
    if (jumpTimer <= 0) {
        tone(BUZZER, 1000, 100);
        vel = jumpPower;
        jumpTimer = jumpCooldown;
    }
}