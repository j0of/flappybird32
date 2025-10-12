#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#include <stdio.h>

#include "Player.h"
#include "ObstacleManager.h"

#include "magicNumbers.h"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_SDA, TFT_SCL, TFT_RST);

Player player;
ObstacleManager obsMan = { player };

int highscore;

void drawCenteredText(const char *s, int16_t y = -1);

volatile bool jumpRequested;
void jumpIntr() {
    jumpRequested = true;
}

void clearBg();

void setup() {
    Serial.begin(9600);
    Serial.println("Hello, world!");

    tft.initR(INITR_BLACKTAB);

    pinMode(BTN, INPUT_PULLUP);
    pinMode(BUZZER, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(BTN), jumpIntr, FALLING);

    clearBg();
    player.draw(tft);
}

unsigned long lastTime = millis();
bool playing = false;
bool gameOver;
bool curBtn, prevBtn;

void loop() {
    unsigned long now = micros();
    float dt = (now - lastTime) / 1000000.0f;
    lastTime = now;

    prevBtn = curBtn;
    curBtn = digitalRead(BTN);

    int score = obsMan.getPassed();

    if (playing) {
        player.clear(tft);
        obsMan.clear(tft);

        if (jumpRequested) {
            player.jump();
            jumpRequested = false;
        }

        player.tick(dt);
        obsMan.tick(dt);

        player.draw(tft);
        obsMan.draw(tft);

        tft.setTextColor(ST7735_WHITE, COLOUR_DIRT);
        tft.setCursor(0, 153);
        tft.printf("FPS: %f", 1.0f / dt);
        
        tft.setTextColor(ST7735_BLACK, COLOUR_SKY);
        char hiText[32];
        sprintf(hiText, "HI:%d", highscore);

        drawCenteredText(hiText, 10);
        drawCenteredText(std::to_string(score).c_str(), 20);

        if (obsMan.playerIntersects() || player.getGrounded()) {
            gameOver = true;
            playing = false;
            tft.setTextColor(ST7735_RED);
            drawCenteredText("GAME OVER!");
            noTone(BUZZER);
            if (score > highscore) {
                tone(BUZZER, 784, 100);
                tone(BUZZER, 1046, 100);
                tone(BUZZER, 1318, 100);
                tone(BUZZER, 784, 100);
                tone(BUZZER, 1046, 100);
                tone(BUZZER, 1318, 100);
                tone(BUZZER, 784, 100);
                tone(BUZZER, 1046, 100);
                tone(BUZZER, 1318, 300);

                tft.fillRect(0, 10, SCREEN_WIDTH, 36, ST7735_ORANGE);
                tft.setTextColor(ST7735_YELLOW);
                drawCenteredText("CONGRATULATIONS!",  15);
                char newhi[16];
                sprintf(newhi, "NEW HI:%d", score);
                drawCenteredText(newhi, 25);
                char oldhi[16];
                sprintf(oldhi, "OLD HI:%d", highscore);
                drawCenteredText(oldhi, 35);

                highscore = score;
            } else {
                tone(BUZZER, 80, 600);
            }
        }
    } else {
        if (gameOver) {
            if (curBtn && !prevBtn) {
                gameOver = false;
                player.reset();
                obsMan.reset();
                clearBg();
                player.draw(tft);
                delay(1000);
            }
        } else {
            tft.setTextColor(ST7735_BLACK);
            drawCenteredText("Press to START");
            if (!curBtn && prevBtn) {
                playing = true;
                tft.setTextColor(COLOUR_SKY);
                drawCenteredText("Press to START");
            }
        }
    }
}

void drawCenteredText(const char* s, int16_t y) {
    int16_t x1, y1;
    uint16_t w, h;

    // Get s bounds
    tft.getTextBounds(s, 0, 0, &x1, &y1, &w, &h);

    // Center coordinates
    int16_t x = (tft.width() - w) / 2;
    if (y == -1) 
        y = (tft.height() - h) / 2;

    tft.setCursor(x, y);
    tft.print(s);
}

void clearBg() {
    tft.fillScreen(COLOUR_SKY);
    tft.fillRect(0, DIRT_START, SCREEN_HEIGHT, SCREEN_HEIGHT - DIRT_START, COLOUR_DIRT);
    tft.fillRect(0, GRASS_START, SCREEN_HEIGHT, DIRT_START - GRASS_START, COLOUR_GRASS);
}