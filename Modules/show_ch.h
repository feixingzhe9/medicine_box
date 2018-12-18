#ifndef __MODULES_SHOW_CH_H__
#define __MODULES_SHOW_CH_H__
#include "stm32f10x.h"
#include "lcd.h"

typedef enum
{
    CH_16X16 = 0,
    CH_32X32
}ch_resolution_e;

int show_16x16_ch(uint16_t x, uint16_t y, const char * ch, uint16_t ch_len, uint16_t color);
int show_32x32_ch(uint16_t x, uint16_t y, const char * ch, uint16_t ch_len, uint16_t color);
int show_32x32_ch_vertical(uint16_t x, uint16_t y, const char * ch, uint16_t ch_len, uint16_t color);
int show_32x32_ch_vertical_ex(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, const char * ch, uint16_t ch_len, uint8_t roll_flag, uint16_t color);
int show_24X24_ch(uint16_t x, uint16_t y, const char * ch, uint16_t ch_len, uint16_t color, uint8_t *data);
int show_8X16_ch(uint16_t x, uint16_t y, const char ch, uint16_t ch_len, uint16_t color, uint8_t *data);
int show_8X16_ch_vertical(uint16_t x, uint16_t y, const char ch, uint16_t ch_len, uint16_t color, uint8_t *data);
int show_16X32_ch_vertical(uint16_t x, uint16_t y, const char ch, uint16_t ch_len, uint16_t color, uint8_t *data);
int show_16X16_ch_vertical(uint16_t x, uint16_t y, const char ch, uint16_t ch_len, uint16_t color, uint8_t *data);
int show_16X16_ch_horizontal(uint16_t x, uint16_t y, const char ch, uint16_t ch_len, uint16_t color, uint8_t *data);
int show_8X16_ch_horizontal(uint16_t x, uint16_t y, const char ch, uint16_t ch_len, uint16_t color, uint8_t *data);

#endif
