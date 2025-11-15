#include "Game.h"
#include "config.h"

Game::Game() :
    tft(Adafruit_ST7735(TFT_CS, TFT_DC, TFT_SDA, TFT_SCL, TFT_RST)),
    player(tft),
    obstacleManager(&tft, player),
    ipShown(true) {}

void Game::setup(WebController *wc) {
    tft.initR(INITR_BLACKTAB);
    tft.fillScreen(COLOUR_SKY);

    webcontroller = wc;
    clearBg();
    tft.setTextColor(ST7735_BLACK);
    tft.setCursor(5, 5);
    tft.print("CONNECT REMOTE:");
    tft.setCursor(5, 25);
    tft.printf("WiFi name: %s", SSID);
    tft.setCursor(5, 45);
    tft.printf("WiFi password: %s", PASSWORD);
    tft.setCursor(5, 65);
    tft.printf("Website IP: %s", wc->getIp());
}

void Game::loop() {
    currTime = micros();
    dt = (currTime - prevTime) / 1000000.0f;
    prevTime = currTime;

    lastFpsUpdate += dt;
    if (lastFpsUpdate >= fpsUpdateInterval) {
        lastFpsUpdate = 0;
        webcontroller->broadcastFPS(1.0f / dt);
    }

    prevBtn = curBtn;
    curBtn = digitalRead(BTN);

    if ((!curBtn && prevBtn) || webcontroller->remoteTrigger) {
        trigger = true;
        webcontroller->remoteTrigger = false;
    }

    if (ipShown) {
        if (trigger) {
            trigger = false;
            ipShown = false;
            tft.fillScreen(COLOUR_SKY);
            clearBg();
            player.draw();
            drawCenteredText("Press to START");
        }
        return;
    }


    int score = obstacleManager.getScore();

    if (playing) {
        player.clear();
        obstacleManager.clear();

        if (trigger) {
            trigger = false;
            player.jump();
        }

        player.tick(dt);
        obstacleManager.tick(dt);

        player.draw();
        obstacleManager.draw();

        int score = obstacleManager.getScore();
        
        tft.setTextColor(ST7735_BLACK, COLOUR_SKY);
        char hiText[32];
        sprintf(hiText, "HI:%d", highScore);

        drawCenteredText(hiText, 10);
        drawCenteredText(std::to_string(score).c_str(), 20);

        if (obstacleManager.playerIntersects() || player.getGrounded()) {
            gameOver = true;
            playing = false;
            tft.setTextColor(ST7735_RED);
            drawCenteredText("GAME OVER!");
            noTone(BUZZER);
            if (score > highScore) {
                highScore = score;
                webcontroller->broadcastHighscore(highScore);
                
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
                sprintf(oldhi, "OLD HI:%d", highScore);
                drawCenteredText(oldhi, 35);
            } else {
                tone(BUZZER, 80, 600);
            }
            delay(1000);
        }
    } else {
        if (gameOver) {
            if (trigger) {
                trigger = false;
                gameOver = false;
                player.reset();
                obstacleManager.reset();
                clearBg();
                player.draw();
            }
        } else {
            tft.setTextColor(ST7735_BLACK);
            drawCenteredText("Press to START");
            if (trigger) {
                trigger = false;
                playing = true;
                player.jump();
                tft.setTextColor(COLOUR_SKY);
                drawCenteredText("Press to START");
            }
        }
    }
}

void Game::clearBg() {
    tft.fillScreen(COLOUR_SKY);
    tft.fillRect(0, GROUND_START, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_START, COLOUR_DIRT);
    tft.fillRect(0, GROUND_START, SCREEN_WIDTH, 5, COLOUR_GRASS);
}

void Game::drawCenteredText(const char* s, int16_t y) {
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