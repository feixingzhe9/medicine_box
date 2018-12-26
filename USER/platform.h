#ifndef __USER_PLATFORM_H__
#define __USER_PLATFORM_H__
#include "stm32f10x.h"
#include "ucos_ii.h"


uint32_t get_tick(void);
void int_to_str(uint32_t num, char * str);
#endif
