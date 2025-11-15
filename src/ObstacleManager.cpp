#include "ObstacleManager.h"

#include "config.h"

bool circleIntersectsRect(float cx, float cy, float radius, float rx, float ry, float rw, float rh);

ObstacleManager::ObstacleManager(Adafruit_ST7735 *_tft, const Player &_player) : tft(_tft), player(_player), score(0) {
    for (int i = 0; i < obstaclesLen; i++) {
        obstacles[i] = new Obstacle(tft, SCREEN_WIDTH + i * xGap);
    }
}

void ObstacleManager::reset() {
    score = 0;
    for (int i = 0; i < obstaclesLen; i++) {
        obstacles[i]->reset(SCREEN_WIDTH + i * xGap);
    }
}

void ObstacleManager::tick(float dt) {
    for (Obstacle *o : obstacles) {
        if (!o->scored && player.x + player.r > o->x + o->w) {
            noTone(BUZZER);
            tone(BUZZER, 988, 75);
            tone(BUZZER, 1318, 500);
            score++;
            o->scored = true;
        } 
        if (o->x + o->w < 0) {
            int farthest = 0;
            for (Obstacle *o1 : obstacles) {
                if (o1->x - o->x > farthest - o->x)
                    farthest = o1->x;
            }
            o->reset(farthest + xGap);
        }
        o->tick(dt);
    }
}

void ObstacleManager::clear() const {
    for (Obstacle *o : obstacles) {
        o->clear();
    }
}

void ObstacleManager::draw() const {
    for (Obstacle *o : obstacles) {
        o->draw();
    }
}

bool ObstacleManager::playerIntersects() const {
    for (Obstacle *o : obstacles) {
        if (circleIntersectsRect(player.x, player.getY(), player.r, o->x, o->y2, o->w, o->h2)) {
            return true;
        }

        // Prevent player from going above y1 (top obstacle)
        if (player.getY() - player.r < o->y1 + o->h1 && player.x + player.r > o->x && player.x - player.r < o->x + o->w) {
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
