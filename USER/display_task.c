/*
 *  Author: Kaka Xie
 *  brief: display
 */

#include "delay.h"
#include "sys.h"
#include "display_task.h"
#include "show_ch.h"
#include "show_pic.h"

OS_STK DIS_TEST_TASK_STK[DIS_TEST_STK_SIZE];

extern const unsigned char pic_background[307200];
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
        lcd_color_box(0, 0, LCD_X_MAX, LCD_Y_MAX, Red);delay_ms(1000);
        lcd_color_box(0, 0, LCD_X_MAX, LCD_Y_MAX, White);delay_ms(1000);
        lcd_color_box_ex(0, 0, LCD_X_MAX, LCD_Y_MAX, pic_background);delay_ms(1000);
        lcd_color_box(100, 100, 60, 90, Green);delay_ms(1000);
        lcd_color_box_ex(0, 0, LCD_X_MAX, LCD_Y_MAX, pic_background);delay_ms(1000);
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
        for(i = 0; i < 480; i += 32)
        {
            //show_16x16_ch(0, i, "×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²â×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ", 20, Red);
            //show_ch(0, 320, "×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²â×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ", 20, Red);
            show_32x32_ch(0, i, "×Ö¿â²âÊÔÊÖÊõÊÒ×Ö¿â²âÊÔÊÖÊõÊÒ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²â×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ×Ö¿â²âÊÔ", 15, Blue);
//            show_32x32_ch_vertical(0, i, "ÊúÅÅÅµÑÇ", 4, Red);
//            show_32x32_ch(0, i, "ÊúÅÅÅµÑÇ", 4, Red);
        }
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

