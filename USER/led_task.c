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
        LED0=1;
        delay_ms(80);
        LED0=0;
        delay_ms(920);
    };
}

void indicator_led_task(void *pdata)
{
    led_init();
    while(1)
    {
        INDICATOR_LED = 0;
        delay_ms(500);
        INDICATOR_LED = 1;
        delay_ms(500);
    };
}



#include "flash.h"
#include "display_task.h"
#include "show_ch.h"
#include "show_pic.h"
#include "GT31L16M1Y80.h"
extern int show_24X24_ch(uint16_t x, uint16_t y, const char * ch, uint16_t ch_len, uint16_t color, uint8_t *data);
extern int show_12X24_ch(uint16_t x, uint16_t y, char ch, uint16_t ch_len, uint16_t color, uint8_t *data);

OS_STK FLASH_TEST_TASK_STK[LED1_STK_SIZE];
uint8_t flash_read_test_buf[100];

unsigned char r_dat_bat(unsigned long address,unsigned long byte_long,unsigned char *p_arr)
{
    spi2_flash_read(p_arr, address, byte_long);
    return 0;
}

void falsh_test_task(void *pdata)
{
    char a = 0;
    char chinese[] = "Ä¾Ä¾»úÆ÷ÈË";
    while(1)
    {
        delay_ms(100);
        lcd_color_box(0, 0, LCD_X_MAX, LCD_Y_MAX, White);delay_ms(500);

        for(a = 'a'; a <= 'z'; a++)
        {
            ASCII_GetData(a, ASCII_16X32, flash_read_test_buf);
            show_16X32_ch_vertical(10 + (a - 'a') * 16, 42, 'b', 1, 0x001f, flash_read_test_buf);
        }
        for(a = 'A'; a <= 'Z'; a++)
        {
            ASCII_GetData(a, ASCII_16X32, flash_read_test_buf);
            show_16X32_ch_vertical(10 + (a - 'A') * 16, 10, 'b', 1, 0x001f, flash_read_test_buf);
        }
        delay_ms(1000);

        for(a = 'a'; a <= 'z'; a++)
        {
            ASCII_GetData(a, ASCII_8X16, flash_read_test_buf);
            show_8X16_ch_vertical(10 + (a - 'a') * 12, 100, 'b', 1, 0x001f, flash_read_test_buf);
        }
        for(a = 'A'; a <= 'Z'; a++)
        {
            ASCII_GetData(a, ASCII_8X16, flash_read_test_buf);
            show_8X16_ch_vertical(10 + (a - 'A') * 8, 120, 'b', 1, 0x001f, flash_read_test_buf);
        }
        delay_ms(1000);

        gt_16_GetData(0xb0, 0xa1, 0, 0, flash_read_test_buf);   // °¡
        show_16X16_ch_vertical(10, 80, 't', 1, 0x001F, flash_read_test_buf);
        gt_16_GetData(0xba, 0xc3, 0, 0, flash_read_test_buf);   // ºÃ
        show_16X16_ch_vertical(30, 80, 't', 1, 0x001F, flash_read_test_buf);delay_ms(1000);

        for(a = 0; a < sizeof(chinese); a += 2)
        {
            gt_16_GetData(chinese[a], chinese[a + 1], 0, 0, flash_read_test_buf);
            show_16X16_ch_vertical(10 + a * 16 / 2, 200, 't', 1, 0x001F, flash_read_test_buf);
        }
        delay_ms(3000);
    }
}

