#ifndef __MODULE_SHOW_PIC_H__
#define __MODULE_SHOW_PIC_H__
#include "stm32f10x.h"

void lcd_color_box_ex(u16 x_start, u16 y_start, u16 x_long, u16 y_long, const unsigned char* pic);

#endif
