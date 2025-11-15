#pragma once

#include <Adafruit_ST7735.h>

class Player {
public:
    Player(Adafruit_ST7735 &_tft);
    void reset();

    void tick(float dt);
    void clear() const;
    void draw() const;

    float getY() const { return y; };
    float getVel() const { return vel; };
    bool getGrounded() const { return grounded; };

    void jump();

    static constexpr int x = 20;
    static constexpr int r = 8;
private:
    Adafruit_ST7735 &tft;

    float y;
    float vel;
    float jumpTimer;
    bool grounded;

    static constexpr float initY = 50.0f;
    static constexpr int gravity = 340;
    static constexpr int jumpPower = -120;
    static constexpr int jumpCooldown = 0.02;
    static constexpr int maxVel = 500;
};