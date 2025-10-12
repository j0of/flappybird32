#include "Player.h"

#include "magicNumbers.h"

Player::Player() : y(initY) {}

void Player::reset() {
    y = initY;
    vel = 0.0f;
    shouldJump = false;
}

void Player::tick(float dt) {
    vel += gravity * dt;

    if (shouldJump)
    {
        tone(BUZZER, 1000, 100);
        vel = jumpPower;
        shouldJump = false;
    }

    if (vel > maxVel)
        vel = maxVel;

    if (y + (vel * dt) > GRASS_START - r - 3) {
        grounded = true;
        y = GRASS_START - r - 3;
        vel = 0;
    } else {
        grounded = false;
    }

    y += vel * dt;
}

void Player::clear(Adafruit_ST7735 &tft) const {
    tft.drawCircle(x, y, r + 1, COLOUR_SKY);
    tft.fillCircle(x, y, r, COLOUR_SKY);
}

void Player::draw(Adafruit_ST7735 &tft) const {
    tft.fillCircle(x, y, r, COLOUR_BIRD);
    tft.drawCircle(x, y, r + 1, ST7735_BLACK);
}

float Player::getY() const {
    return y;
}

float Player::getVel() const {
    return vel;
}

bool Player::getGrounded() const {
    return grounded;
}

void Player::jump() {
    shouldJump = true;
}