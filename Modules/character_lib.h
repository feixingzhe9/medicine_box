#ifndef __MODULE__CHARACTER_LIB_H__
#define __MODULE__CHARACTER_LIB_H__
#include "stm32f10x.h"


//typedef enum
//{
//    USER_ASCII_16X32_NORMAL = 1,
//    USER_ASCII_8X16_NORMAL,
//}ascii_resolution_e;

//typedef enum
//{
//    USER_CHINESE_16X16 = 1,
//    USER_CHINESE_24X24,
//    USER_CHINESE_32X32
//}chinese_resolution_e;


typedef enum
{
    USER_CH_HIGH_8 = 1,
    USER_CH_HIGH_16,
    USER_CH_HIGH_24,
    USER_CH_HIGH_32
}char_resolution_high_e;

void get_ascii_dot_matrix(char ascii, uint8_t resolution, uint8_t *matrix_out);
void get_chinese_dot_matrix(uint8_t *chinese, uint8_t resolution, uint8_t *matrix_out);

#endif
