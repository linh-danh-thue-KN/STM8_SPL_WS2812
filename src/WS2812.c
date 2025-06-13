#include <WS2812.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
// #include "stm8s.h"

 uint8_t brightness;
 uint32_t endTime;
 int16_t pin;
 uint8_t bOffset;
 uint8_t gOffset;
 bool _begun;
 static  uint8_t pixel_array[NUM_LEDS * 3];
 uint8_t leds = NUM_LEDS; // Số lượng LED
 uint8_t *pixels=pixel_array; // Pointer to pixel data
 uint8_t numBytes;
 uint8_t numLEDs= NUM_LEDS; // Số lượng LED
 uint8_t rOffset;
 uint8_t wOffset;

static volatile uint8_t bytesend;  // tạo biến toàn cục
static volatile uint8_t bit_count; // biến đếm
static volatile uint8_t bita;      // biến tạm để lưu byte

// void show(uint8_t byte)
// {
//     // static volatile uint8_t _bytesend ;
//     bytesend = byte; // SDCC sẽ thấy biến dùng trong C
//     __asm
//         ld a,#24 
//         ld _bit_count, a 
//         ld a, _bytesend;Load byte vào A
//         send_loop : 
//             sla a;Shift left A, bit 7 vào carry
//             jrc send_1;Nếu carry = 1, gửi bit 1
//         send_0 : 
//             bset 0x500F,#4;HIGH
//             nop
//             nop
//             nop
//             nop
//             bres 0x500F,#4;LOW
//             nop
//             nop
//             nop
//             nop
//             nop
//             nop
//             nop
//             nop
//             dec _bit_count;Giảm biến đếm bit
//             jrne send_loop; Nếu còn bit để gửi, quay lại vòng lặp
//             ret
//         send_1 :;Gửi bit 1 
//             bset 0x500F, #4;HIGH
//             nop
//             nop
//             nop
//             nop
//             nop
//             nop
//             nop
//             nop
//             nop
//             nop
//             nop
//             bres 0x500F,#4;    LOW
//             nop     
//             nop
//             dec _bit_count;Giảm biến đếm bit
//             jrne send_loop // Nếu còn bit để gửi, quay lại vòng lặp
//             ret
//     __endasm;
// }

static volatile uint16_t byte_index;
static volatile uint16_t data_index;
static volatile  uint16_t total_bytes = NUM_LEDS * 3; // Tổng số byte cần gửi


void ws2812_show()
{
  // pixel_array[0] = 250; // Đặt giá trị đầu tiên của mảng pixel_array để tránh lỗi tràn
  // pixel_array[1] = 0; // Đặt giá trị thứ hai của mảng pixel_array để tránh lỗi tràn
  // pixel_array[2] = 0; // Đặt giá trị thứ ba của mảng pixel_array để tránh lỗi tràn
  // pixel_array[3] = 0; // Đặt giá trị thứ tư của mảng pixel_array để tránh lỗi tràn
  // pixel_array[4] = 255; // Đặt giá trị thứ năm của mảng pixel_array để tránh lỗi tràn
  // pixel_array[5] = 0; // Đặt giá trị thứ sáu của mảng pixel_array để tránh lỗi tràn
    __asm
        clr     _byte_index
        clr     _byte_index + 1
        ldw     x, #_pixel_array
        ldw     y, #TOTAL_BYTES ; // Tổng số byte cần gửi
    next_byte:              ;// Load pixel_array[byte_index] → A
        ld      a, #8 ; //Set bit count to 8
        ld      _bit_count, a ; // Set bit count to 8
        ld      a, (x)
        incw    x ;
    send_loop:
        nop
        nop
        nop
        sla     a              ; //shift left, MSB → carry
        jrc     send_bit_1

    send_bit_0:
        bset    0x500F, #4     ;// HIGH PD4
        nop
        nop
        nop
        nop
        bres    0x500F, #4     ; //LOW  PD4
        nop
        nop
        nop
        nop
        nop
        dec     _bit_count    ; // Giảm biến đếm bit
        jrne    send_loop      ; //lenh trc khac 0 thi goi send_loop
       ; jp      next_check_done ; //goi ham next_check_done maf ko can dieu kien
        decw   y ; // Giảm số byte còn lại
        jrne next_byte ; // Nếu còn byte để gửi, quay lại vòng lặp
        ret ;// Trả về sau khi gửi hết dữ liệu================

    send_bit_1:
        bset    0x500F, #4     ;// HIGH PD4
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        bres    0x500F, #4     ; //LOW  PD4
        
        dec     _bit_count
        jrne    send_loop
        ;jp      next_check_done ; //nếu đã gửi hết 8 bit, chuyển sang byte tiếp theo

    ;next_check_done:
        ;inc   _byte_index
        ;ld    a, _byte_index
        ;cp    a, #TOTAL_BYTES
        ;jrne  next_byte
        decw   y ; // Giảm số byte còn lại
        jrne next_byte ; // Nếu còn byte để gửi, quay lại vòng lặp
        ret ;// Trả về sau khi gửi hết dữ liệu================
    __endasm;
}
void begin(uint8_t t, uint8_t brightnessValue)
{
  brightness = brightnessValue;
    rOffset = (t >> 4) & 0b11; // regarding R/G/B/W offsets
    gOffset = (t >> 2) & 0b11;
    bOffset = t & 0b11;
}

void setPixelColor(uint16_t n, uint32_t c)
{
    if (n < numLEDs)
    {
        uint8_t *p, r = (uint8_t)(c >> 16), g = (uint8_t)(c >> 8), b = (uint8_t)c;
        if (brightness)
        { // See notes in setBrightness()
            r = (r * brightness) >> 8;
            g = (g * brightness) >> 8;
            b = (b * brightness) >> 8;
        }
        p = &pixels[n * 3];
        p[rOffset] = r;
        p[gOffset] = g;
        p[bOffset] = b;
    }
}

void setPixelColorRGB(uint16_t n, uint8_t r, uint8_t g,
                                      uint8_t b)
{
    if (n < numLEDs)
    {
        if (brightness)
        { // See notes in setBrightness()
            r = (r * brightness) >> 8;
            g = (g * brightness) >> 8;
            b = (b * brightness) >> 8;
        }
        uint8_t *p;
        p = &pixels[n * 3];
        p[rOffset] = r;
        p[gOffset] = g;
        p[bOffset] = b;
    }
}




