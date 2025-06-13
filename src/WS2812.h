#ifndef __WS2812_H
#define __WS2812_H
#include <stdint.h>
#include <stdbool.h>
// #include "stm8s.h"

#define NUM_LEDS 10/// Số lượng LED maximum
#define TOTAL_BYTES  (NUM_LEDS * 3)

#define RGB ((0 << 6) | (0 << 4) | (1 << 2) | (2)) ///< Transmit as R,G,B
#define RBG ((0 << 6) | (0 << 4) | (2 << 2) | (1)) ///< Transmit as R,B,G
#define GRB ((1 << 6) | (1 << 4) | (0 << 2) | (2)) ///< Transmit as G,R,B
#define GBR ((2 << 6) | (2 << 4) | (0 << 2) | (1)) ///< Transmit as G,B,R
#define BRG ((1 << 6) | (1 << 4) | (2 << 2) | (0)) ///< Transmit as B,R,G
#define BGR ((2 << 6) | (2 << 4) | (1 << 2) | (0)) ///< Transmit as B,G,R



void show(uint8_t byte);
void begin(uint8_t t, uint8_t brightnessValue);
void ws2812_show();
void setPixelColor(uint16_t n, uint32_t c);
void setPixelColorRGB(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
void setBrightness(uint8_t);

#endif /* __WS2812_H */