# Flappybird32
Mini flappybird console implemented using ESP32 and Arduino framework.

https://github.com/user-attachments/assets/f266951c-387f-4cfc-aca1-4248d2725a74

# Components
- ESP-WROOM32
- Dollatek 1.8' 128x160 RGB TFT_LCD (ST7735)
-  Push button
-  Slide switch
-  Passive Buzzer
-  10k ohm resistor

# Wiring
| ESP32 Pin | Component |
| --- | --- |
| D14 | TFT SCL |
| D27 | TFT SDA |
| D26 | TFT RES |
| D25 | TFT DC |
| D33 | TFT CS |
| D4 | Push button (A) |
| D16 | Slide switch (Middle) |

5V  ----- TFT VCC

GND ----- TFT GND

GND ----- 10k ohms ----- Button (B)

GND ----- (-) Passive buzzer (+) ----- Slide switch (B)
