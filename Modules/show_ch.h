#ifndef __MODULES_SHOW_CH_H__
#define __MODULES_SHOW_CH_H__
#include "stm32f10x.h"

typedef enum
{
    CH_16X16 = 0,
    CH_32X32
}ch_resolution_e;

int show_16x16_ch(uint16_t x, uint16_t y, const char * ch, uint16_t ch_len, uint16_t color);
int show_32x32_ch(uint16_t x, uint16_t y, const char * ch, uint16_t ch_len, uint16_t color);
#endif
