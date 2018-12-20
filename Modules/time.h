#ifndef __MODULE_TIME_H__
#define __MODULE_TIME_H__
#include "stm32f10x.h"


#pragma pack(1)
typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t weekday;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
}time_t;
#pragma pack()

#endif

