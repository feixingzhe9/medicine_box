/*
 *  Author: Kaka Xie
 *  brief: show picture
 */

//#include "show_pic.h"
#include "lcd.h"


int show_pic(u16 x, u16 y, u16 pic_H, u16 pic_V, const unsigned char* pic)
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
//    *(__IO u16 *) (Bank1_LCD_D) = j;
//    }
//    // 	lcd_write_cmd(0x36); //Set_address_mode
//    // 	lcd_write_data(0xaa);


    u32 temp;

    lcd_block_write(x, x + pic_V - 1, y, y + pic_H - 1);
    for (temp = 0; temp < pic_V * pic_H; temp++)
    {
        *(__IO u16 *) (Bank1_LCD_D) = pic[temp];
    }
    return -1;
}


void lcd_color_box_ex(u16 xStart, u16 yStart, u16 xLong, u16 yLong, const unsigned char* pic)
{
    u32 i;
    uint16_t color = 0;
    if((xLong > 0) && (yLong > 0))
    {
        lcd_block_write(xStart, xStart + xLong - 1, yStart, yStart + yLong - 1);
        for (i = 0; i < xLong * yLong - 1; i++)
        {
            color = pic[i];
            color <<= 8;
            color += pic[i + 1];
            *(__IO u16 *) (Bank1_LCD_D) = color;
            //*(__IO u16 *) (Bank1_LCD_D) = pic[temp];
        }
    }
}



void LCD_Fill_Pic(u16 x, u16 y,u16 pic_H, u16 pic_V, const unsigned char* pic)
{
    unsigned long i;
    unsigned int j;

    // 	WriteComm(0x16); //Set_address_mode
    // 	lcd_write_data(0x20); //横屏，从左下角开始，从左到右，从下到上
    // 	BlockWrite(x,x+pic_H-1,y,y+pic_V-1);
    for (i = 0; i < pic_H*pic_V*2; i+=2)
    {
        j=pic[i];
        j=j<<8;
        j=j+pic[i+1];
        *(__IO u16 *) (Bank1_LCD_D) = j;
    }
    // 	WriteComm(0x36); //Set_address_mode
    // 	lcd_write_data(0xaa);
}
