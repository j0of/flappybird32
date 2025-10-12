#pragma once

#include <Adafruit_ST7735.h>
class Player {
public:
    Player();
    void reset();

    void tick(float dt);
    void clear(Adafruit_ST7735 &tft) const;
    void draw(Adafruit_ST7735 &tft) const;

    float getY() const;
    float getVel() const;
    bool getGrounded() const;

    void jump();

    static constexpr int x = 20;
    static constexpr int r = 8;
private:
    float y;
    float vel;
    bool shouldJump;
    bool grounded;

    static constexpr float initY = 50.0f;
    static constexpr int gravity = 300;
    static constexpr int jumpPower = -75;
    static constexpr int maxVel = 500;
};