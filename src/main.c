#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "stm8s.h"
#include "WS2812.h"
#define LED_PIN 4   // Chân PD4 kết nối với WS2812 

void Delay(uint16_t nCount);
void gpio_init(void)
{
    GPIOD->DDR |= (1 << LED_PIN);  // Output
    GPIOD->CR1 |= (1 << LED_PIN);  // Push-pull
    GPIOD->ODR &= ~(1 << LED_PIN); // LOW
}

uint32_t next = 1;

void randomSeed(uint32_t seed)
{
    next = seed;
}

long random(long min, long max)
{
    next = next * 1103515245 + 12345;
    long val = (next >> 16) & 0x7FFF;
    return min + (val % (max - min));
}

void main(void)
{
    // Cấu hình hệ thống
    CLK->CKDIVR = 0; // Chia 1 → 16MHz

    gpio_init();
    begin(GRB, 125); // Bắt đầu với định dạng RGB và độ sáng tối đa

    setPixelColorRGB(0, 0xFF, 0xFF, 0xFF); // Thiết lập màu đỏ cho LED đầu tiên
    setPixelColorRGB(1, 0x00, 0xFF, 0x00); // Thiết lập màu xanh lá cho LED thứ hai
    setPixelColorRGB(2, 0x00, 0x00, 0xFF); // Thiết lập màu xanh dương cho LED thứ ba
    setPixelColorRGB(3, 0xFF, 0x00, 0xFF); // Thiết lập màu tím cho LED thứ tư
    setPixelColorRGB(4, 0xFF, 0xA5, 0x00); // Thiết lập màu cam cho LED thứ năm
    setPixelColorRGB(5, 0x00, 0xFF, 0xFF); // Thiết lập màu cyan cho LED thứ sáu
    setPixelColorRGB(6, 0x80, 0x00, 0x80); // Thiết lập màu tím nhạt cho LED thứ bảy
    setPixelColorRGB(7, 0xFF, 0x00, 0x00); // Thiết lập màu đỏ cho LED thứ tám
    ws2812_show();                         // Hiển thị dữ liệu
    Delay(100);
    while (1)
    {

        for (int i = 0; i < NUM_LEDS; i++)
        {
            uint8_t r = random(0, 255);
            uint8_t g = random(0, 255);
            uint8_t b = random(0, 255);
            setPixelColorRGB(i, r, g, b);
        }
        ws2812_show();
        Delay(80);
    }
}
void Delay(uint16_t nCount)
{
    for (uint16_t i = 0; i < nCount; i++)
    {
        for (uint16_t j = 0; j < 1000; j++)
        {
            __asm__("nop"); // Lệnh chờ 1 chu kỳ, tránh tối ưu hóa
        }
    }
}
void delay_us(uint16_t us)
{
    for (uint16_t i = 0; i < us; i++)
    {
        __asm__("nop"); // Lệnh chờ 1 chu kỳ, tránh tối ưu hóa
    }
}

/**
 * @brief Delay
 * @param nCount
 * @retval None
 */

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif
