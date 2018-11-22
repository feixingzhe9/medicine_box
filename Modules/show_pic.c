/*
 *  Author: Kaka Xie
 *  brief: show picture
 */

//#include "show_pic.h"
#include "lcd.h"


int show_pic(u16 x, u16 y, u16 pic_h, u16 pic_v, const unsigned char* pic)
{
//    unsigned long i;
//    unsigned int j;

//    // 	lcd_write_cmd(0x16); //Set_address_mode
//    // 	lcd_write_data(0x20); //横屏，从左下角开始，从左到右，从下到上
//    // 	lcd_block_write(x,x+pic_H-1,y,y+pic_V-1);
//    for (i = 0; i < pic_H * pic_V * 2; i += 2)
//    {
//    j = pic[i];
//    j = j << 8;
//    j = j + pic[i + 1];
//    *(__IO u16 *) (BANK_1_LCD_DATA) = j;
//    }
//    // 	lcd_write_cmd(0x36); //Set_address_mode
//    // 	lcd_write_data(0xaa);


    u32 temp;

    lcd_block_write(x, x + pic_v - 1, y, y + pic_h - 1);
    for (temp = 0; temp < pic_v * pic_h; temp++)
    {
        *(__IO u16 *) (BANK_1_LCD_DATA) = pic[temp];
    }
    return -1;
}


void lcd_color_box_ex(u16 x_start, u16 y_start, u16 x_long, u16 y_long, const unsigned char* pic)
{
    u32 i;
    uint16_t color = 0;
    if((x_long > 0) && (y_long > 0))
    {
        lcd_block_write(x_start, x_start + x_long - 1, y_start, y_start + y_long - 1);
        for (i = 0; i < x_long * y_long - 1; i++)
        {
            color = pic[i * 2 + 1];
            color <<= 8;
            color += pic[i * 2];
            *(__IO u16 *) (BANK_1_LCD_DATA) = color;
            //*(__IO u16 *) (BANK_1_LCD_DATA) = pic[temp];
        }
    }
}



//void lcd_fill_pic(u16 x, u16 y,u16 pic_h, u16 pic_v, const unsigned char* pic)
//{
//    unsigned long i;
//    unsigned int j;

//    // 	WriteComm(0x16); //Set_address_mode
//    // 	lcd_write_data(0x20); //横屏，从左下角开始，从左到右，从下到上
//    // 	BlockWrite(x,x+pic_H-1,y,y+pic_V-1);
//    for (i = 0; i < pic_h * pic_v * 2; i += 2)
//    {
//        j=pic[i];
//        j=j<<8;
//        j=j+pic[i+1];
//        *(__IO u16 *) (BANK_1_LCD_DATA) = j;
//    }
//    // 	WriteComm(0x36); //Set_address_mode
//    // 	lcd_write_data(0xaa);
//}
