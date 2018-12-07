/*
 *  Author: Kaka Xie
 *  brief: display
 */

#include "delay.h"
#include "sys.h"
#include "display_task.h"
#include "show_ch.h"
#include "show_pic.h"
#include "character_lib.h"
#include "GT31L16M1Y80.h"

OS_STK DIS_TEST_TASK_STK[DIS_TEST_STK_SIZE];

OS_STK DISPLAY_TASK_STK[DIS_TEST_STK_SIZE];

void display_one_chinese(uint16_t start_x, uint16_t start_y, uint8_t *chinese, uint16_t color)
{
    uint8_t chinese_matrix[32] = {0};   //max 16X16 need 32 bytes
    get_chinese_dot_matrix(chinese, 0, chinese_matrix);
    show_16X16_ch_vertical(start_x, start_y, ' ', 1, color, chinese_matrix);
}

void display_many_chinese(uint16_t start_x, uint16_t start_y, uint8_t *chinese, uint16_t len, uint16_t color)
{
    uint16_t i;
    for(i = 0; i < len; i += 2)
    {
        display_one_chinese(start_x + (i / 2) * 16, start_y, &chinese[i], color);
    }
}

void display_one_ascii(uint16_t start_x, uint16_t start_y, char ascii, uint8_t resolution, uint16_t color)
{
    uint8_t ascii_matrix[64] = {0};    //max 16X32 need 64 bytes
    get_ascii_dot_matrix(ascii, 0, ascii_matrix);
    switch(resolution)
    {
        case ASCII_16X32_NORMAL:
            get_ascii_dot_matrix(ascii, resolution, ascii_matrix);
            show_16X32_ch_vertical(start_x, start_y, ' ', 1, color, ascii_matrix);
            break;
        case ASCII_8X16_NORMAL:
            get_ascii_dot_matrix(ascii, resolution, ascii_matrix);
            show_8X16_ch_vertical(start_x, start_y, ' ', 1, color, ascii_matrix);
            break;
        default: break;
    }
}

void display_many_ascii(uint16_t start_x, uint16_t start_y, char* ascii, uint16_t len, uint8_t resolution, uint16_t color)
{
    uint8_t i;
    uint8_t space = 16;
    switch(resolution)
    {
        case ASCII_16X32_NORMAL:
            space = 16;
            break;
        case ASCII_8X16_NORMAL:
            space = 8;
            break;
        default: return;
    }
    for(i = 0; i < len; i++)
    {
        display_one_ascii(start_x + i * space, start_y, ascii[i], resolution, color);
    }
}


void display_many_chinese_middle(uint16_t start_y, uint8_t* chinese, uint16_t len, uint8_t resolution, uint16_t color)
{
    if(len <= (LCD_X_MAX / 16) * 2)
    {
        display_many_chinese((LCD_X_MAX - ((len * 16) / 2)) / 2, start_y, chinese, len, color);
    }

}


void display_many_ascii_middle(uint16_t start_y, char* ascii, uint16_t len, uint8_t resolution, uint16_t color)
{
    uint16_t start_x = 0;
    switch(resolution)
    {
        case ASCII_16X32_NORMAL:
            start_x = (LCD_X_MAX - (len * 16)) / 2;
            break;
        case ASCII_8X16_NORMAL:
            start_x = (LCD_X_MAX - (len * 8)) / 2;
            break;
        default: return;
    }
    display_many_ascii(start_x, start_y, ascii, len, resolution, color);
}


void display_string(uint16_t start_x, uint16_t start_y, uint8_t* str, uint16_t len, uint8_t resolution, uint16_t color)
{
    uint16_t i;
    for(i = 0; i < len; i++)
    {
        if(str[i] >= 0x80)
        {
            display_one_chinese(start_x + i * 8, start_y, &str[i], color);
            i++;
        }
        else
        {
            display_one_ascii(start_x + i * 8, start_y, str[i], ASCII_8X16_NORMAL, color);
        }
    }
}

extern uint8_t flash_read_test_buf[100];
void display_task(void *pdata)
{
    uint8_t init_flag = 0;
    while(1)
    {
        if(init_flag == 0)
        {
            init_flag = 1;
            lcd_color_box(0, 0, LCD_X_MAX, LCD_Y_MAX, White);
        }
//        display_one_chinese(10, 10, "ÉÏ", Blue);
        display_many_chinese_middle(120, "ÉÏº£Ä¾Ä¾»úÆ÷ÈË", 14,0, Blue);
        display_many_chinese(10, 30, "ÉÏº£Ä¾Ä¾»úÆ÷ÈË", 14, Black);
        display_many_ascii(10, 50, "hello world", sizeof("hello world"), ASCII_16X32_NORMAL, Blue);
        display_many_ascii(10, 80, "hello world", sizeof("hello world"), ASCII_8X16_NORMAL, Blue);
        display_many_ascii_middle(140, "Mrobot", sizeof("Mrobot"), ASCII_16X32_NORMAL, Blue);
        display_one_ascii(10, 100, 'A', ASCII_16X32_NORMAL, Blue);
        display_string(10, 180, "ABCDEÄ¾Ä¾»úÆ÷ÈËFG", sizeof("ABCDEÄ¾Ä¾»úÆ÷ÈËFG"), 0, Blue);
        delay_ms(100);
    }
}



//extern const unsigned char pic_background[307200];
void dis_test_task(void *pdata)
{
    int i = 0;

    while(1)
    {
//        for(i = 0; i < 100; i++)
//        {
//            lcd_color_box(0, 0, 320, 480, (0x00f1 << 5) + (0x00f2 << 10) + i);
//            //delay_ms(10);
//        }

//        lcd_color_box(0,0,320,480,Red);delay_ms(1000);
//        lcd_color_box(0,0,320,480,Blue);delay_ms(1000);
        delay_ms(1000);
        lcd_color_box(0, 0, LCD_X_MAX, LCD_Y_MAX, Red);delay_ms(1000);
        lcd_color_box(0, 0, LCD_X_MAX, LCD_Y_MAX, White);delay_ms(1000);
//        lcd_color_box_ex(0, 0, LCD_X_MAX, LCD_Y_MAX, pic_background);delay_ms(1000);
        lcd_color_box(100, 100, 60, 90, Green);delay_ms(1000);
//        lcd_color_box_ex(0, 0, LCD_X_MAX, LCD_Y_MAX, pic_background);delay_ms(1000);
//        lcd_color_box(0,0,20,80,Green);delay_ms(1000);
//        lcd_color_box(100,100,20,80,Green);delay_ms(1000);
//        lcd_color_box(200,260,20,80,Green);delay_ms(1000);
//
//        for(i = 0; i < 320; i++)
//        {
//            lcd_draw_pixel(i, i, Red);
//        }
//        delay_ms(1000);
//        for(i = 0; i < 320; i++)
//        {
//            lcd_draw_pixel(320 - i, i, Red);
//        }
//        delay_ms(1000);
//        for(i = 0; i < 320; i++)
//        {
//            lcd_draw_pixel(180, i, Red);
//        }
//        delay_ms(1000);
//        for(i = 0; i < 320; i++)
//        {
//            lcd_draw_pixel(i, 180, Red);
//        }


//        show_ch(100, 100, "×Ö", 1, Red);
//        delay_ms(1000);
//        show_ch(120, 120, "¿â", 1, Red);
//        delay_ms(1000);
//        show_ch(140, 140, "²â", 1, Red);
//        delay_ms(1000);
//        show_ch(160, 160, "ÊÔ", 1, Red);
//        delay_ms(1000);
//        for(i = 0; i < 480; i += 32)
//        {
//            //show_16x16_ch(0, i, "×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²â×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ", 20, Red);
//            //show_ch(0, 320, "×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²â×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ", 20, Red);
//            show_32x32_ch(0, i, "×Ö¿â²âÊÔÊÖÊõÊÒ×Ö¿â²âÊÔÊÖÊõÊÒ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²â×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ", 15, Blue);
////            show_32x32_ch_vertical(0, i, "ÊúÅÅÅµÑÇ", 4, Red);
////            show_32x32_ch(0, i, "ÊúÅÅÅµÑÇ", 4, Red);
//        }
        delay_ms(1000);
//        for(i = 0; i < 480; i++)
//        {
//            lcd_draw_pixel(i, 470, Red);
//        }
//        delay_ms(1000);
//        for(i = 0; i < 480; i++)
//        {
//            lcd_draw_pixel(310, i, Red);
//        }
//        delay_ms(1000);
        //lcd_color_box(300,200,20,80,Green);delay_ms(1000);
        //lcd_color_box(300,400,20,80,Green);delay_ms(1000);
        //lcd_fill_pic(0, 0, 320, 480, gImage_MM_T035);delay_ms(1000);



//        for(i = 0; i < 480; i += 10)
//        {
//            lcd_color_box_ex(0, i, 320, 480 - i, gImage_MM_T035);
//        }

//        lcd_color_box_ex(0, 0, 320, 480, gImage_MM_T035);
        delay_ms(1000);
    }

}

