/*
 *  Author: Kaka Xie
 *  brief: main entrance
 */

#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	
#include "includes.h"
#include "lcd.h"
#include "math.h"
#include "show_ch.h"
#include "common.h"

int main(void)
{
    OSInit();
    delay_init();
    NVIC_Configuration();
    __disable_irq();
    LED_Init();
    lcd_init();
    //lcd_color_box(0,0,320,480,Red);
    uart_init(19200);
    os_user_config();
    user_init_depend_on_os_config();
    __enable_irq();
    OSStart();

}

