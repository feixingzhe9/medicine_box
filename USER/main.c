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
#include "id.h"
#include "lock.h"
#include "battery.h"

uint8_t test_id = 0;
int main(void)
{
    OSInit();
    delay_init();
    NVIC_Configuration();
    __disable_irq();
    init_lock();
//    delay_us_ex(3000 * 1000);
    id_key_init();
    test_id = get_my_id();
    lcd_init();
//    delay_us_ex(1000 * 1000);
//    lcd_init();
    fp_en();
    fp_com_init();
//    delay_us_ex(1000 * 1000);
    lc12s_init();
    spi2_flash_init();
    battery_adc_init();
    os_user_config();
    user_init_depend_on_os_config();
    __enable_irq();
    OSStart();
}

