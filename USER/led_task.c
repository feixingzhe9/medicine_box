/*
 *  Author: Kaka Xie
 *  brief: led task
 */

#include "led_task.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "led.h"

OS_STK LED1_TASK_STK[LED1_STK_SIZE];
OS_STK INDICATOR_LED_TASK_STK[INDICATOR_LED_STK_SIZE];

void led1_task(void *pdata)
{
    while(1)
    {
        LED0 = 1;
        delay_ms(80);
        LED0 = 0;
        delay_ms(920);
    };
}


#include "battery.h"
uint16_t test_adc_voltage = 0;
void indicator_led_task(void *pdata)
{
    led_init();
    battery_adc_init();
    while(1)
    {
        INDICATOR_LED = 0;
        delay_ms(500);
        INDICATOR_LED = 1;
        delay_ms(500);
        test_adc_voltage =  get_battery_voltage();
    };
}

