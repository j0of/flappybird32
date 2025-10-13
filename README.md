# Flappybird32
Mini flappybird console implemented using ESP32 and Arduino framework.

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
| D14 | TFT CS |
| D13 | TFT RES |
| D12 | TFT DC |
| D25 | TFT SCL |
| D26 | TFT SDA |
| D32 | Push button (A) |
| D15 | Slide switch (Middle) |

5V  ----- TFT VCC

GND ----- TFT GND

GND ----- 10k ohms ----- Button (B)

GND ----- (-) Passive buzzer (+) ----- Slide switch (B)
