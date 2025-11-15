#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Wifi.h>
#include <stdio.h>

#include "Game.h"
#include "WebController.h"
#include "config.h"

WebController wc;
Game game;

void setup() {
    Serial.begin(9600);
    Serial.println("Hello, world!");

    pinMode(BTN, INPUT_PULLUP);
    pinMode(BUZZER, OUTPUT);

    wc.setup();
    game.setup(&wc);
}

void loop() {
    game.loop();
}