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
#include "fingerprint.h"
#include "lc12s_wireless.h"

#include "flash.h"

int main(void)
{
    delay_us_ex(1000 * 1000);
    OSInit();
    delay_init();
    NVIC_Configuration();
    __disable_irq();
    lcd_init();
    delay_us_ex(1000 * 1000);
    lcd_init();
    fp_com_init();
    lc12s_init();
    SPI_Flash_Init();
    os_user_config();
    user_init_depend_on_os_config();
    __enable_irq();
    OSStart();
}

