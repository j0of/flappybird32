#include "ObstacleManager.h"

#include "magicNumbers.h"
#include "math.h"

bool circleIntersectsRect(float cx, float cy, float radius, float rx, float ry, float rw, float rh);

ObstacleManager::ObstacleManager(const Player &player) : player(player) {
    for (int i = 0; i < obstaclesLen; i++) {
        obstacles[i] = { SCREEN_WIDTH + i * xGap };
    }
}

void ObstacleManager::reset() {
    passed = 0;
    for (int i = 0; i < obstaclesLen; i++) {
        obstacles[i].reset(SCREEN_WIDTH + i * xGap);
    }
}

void ObstacleManager::tick(float dt) {
    for (Obstacle &o : obstacles) {
        if (!o.scored && player.x + player.r > o.x + o.w) {
            noTone(BUZZER);
            tone(BUZZER, 988, 75);
            tone(BUZZER, 1318, 500);
            passed++;
            o.scored = true;
        } 
        if (o.x + o.w < 0) {
            int farthest = 0;
            for (const Obstacle &o1 : obstacles) {
                if (o1.x - o.x > farthest - o.x)
                    farthest = o1.x;
            }
            o.reset(farthest + xGap);
        }
        o.tick(dt);
    }
}

void ObstacleManager::clear(Adafruit_ST7735 &tft) const {
    for (const Obstacle &o : obstacles) {
        o.clear(tft);
    }
}

void ObstacleManager::draw(Adafruit_ST7735 &tft) const {
    for (const Obstacle &o : obstacles) {
        o.draw(tft);
    }
}

int ObstacleManager::getPassed() const {
    return passed;
}

bool ObstacleManager::playerIntersects() const {
    for (const Obstacle &o : obstacles) {
        if (circleIntersectsRect(player.x, player.getY(), player.r, o.x, o.y2, o.w, o.h2)) {
            return true;
        }

        // Prevent player from going above y1 (top obstacle)
        if (player.getY() - player.r < o.y1 + o.h1 && player.x + player.r > o.x && player.x - player.r < o.x + o.w) {
            // Player is overlapping the top obstacle
            return true;
        }
    }
    return false;
}

bool circleIntersectsRect(float cx, float cy, float radius, float rx, float ry, float rw, float rh) {
    // Find closest point on rectangle to circle
    float closestX = cx;
    float closestY = cy;

    if (cx < rx) closestX = rx;
    else if(cx > rx + rw) closestX = rx + rw;

    if (cy < ry) closestY = ry;
    else if (cy > ry + rh) closestY = ry + rh;

    // Distance between circle center and closest point
    float dx = cx - closestX;
    float dy = cy - closestY;

    return (dx*dx + dy*dy) <= radius*radius;
}
