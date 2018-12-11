#ifndef __MODULE__CHARACTER_LIB_H__
#define __MODULE__CHARACTER_LIB_H__
#include "stm32f10x.h"


typedef enum
{
    ASCII_16X32_NORMAL = 1,
    ASCII_8X16_NORMAL,
}ascii_resolution_e;


void get_ascii_dot_matrix(char ascii, uint8_t resolution, uint8_t *matrix_out);
void get_chinese_dot_matrix(uint8_t *chinese, uint8_t resolution, uint8_t *matrix_out);

#endif
