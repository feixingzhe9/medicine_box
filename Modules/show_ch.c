/*
 *  Author: Kaka Xie
 *  brief: display Chinese characters
 */

#include "show_ch.h"
#include "ch_lib.h"
#include "lcd.h"

/*显示汉字字符是高频操作，为了提高效率，将16X16 32X32 以及横排 竖排的分开操作，牺牲空间换时间*/


//------ show 16X16 ------//
int show_16x16_ch(uint16_t x, uint16_t y, const char * ch, uint16_t ch_len, uint16_t color)
{
    int i = 0;
    int ch_lib_len = sizeof(ch_16x16_lib) / sizeof(ch_16x16_lib[0]);
    uint8_t j = 0;
    uint8_t bit_cnt = 0;
    uint16_t ch_cnt = 0;

    for(ch_cnt = 0; ch_cnt < ch_len; ch_cnt++)
    {
        if((x < LCD_WIDTH - 16 + 1) && (y < LCD_LENGTH - 16 + 1))
        {
            for(i = 0; i < ch_lib_len; i++)
            {
                if((ch_16x16_lib[i].index[0] == ch[ch_cnt * 2]) && (ch_16x16_lib[i].index[1] == ch[ch_cnt * 2 + 1]))
                {
                    for(j = 0; j < 32; j++)
                    {
                        for(bit_cnt = 0; bit_cnt < 8; bit_cnt++)
                        {
                            if(ch_16x16_lib[i].mat[j] & (1 << bit_cnt))
                            {
                                lcd_draw_pixel(x + ch_cnt * 16 + (j % 2) * 8 + bit_cnt, y + j / 2, color);
                            }
                        }
                    }
                }
            }
        }
    }

    return 1;
}


//------ show 32X32 ------//
int show_32x32_ch(uint16_t x, uint16_t y, const char * ch, uint16_t ch_len, uint16_t color)
{
    int i = 0;
    int ch_lib_len = sizeof(ch_32x32_lib) / sizeof(ch_32x32_lib[0]);
    uint8_t j = 0;
    uint8_t bit_cnt = 0;
    uint16_t ch_cnt = 0;

    for(ch_cnt = 0; ch_cnt < ch_len; ch_cnt++)
    {
        if((x < LCD_WIDTH - 32 + 1) && (y < LCD_LENGTH - 32 + 1))
        {
            for(i = 0; i < ch_lib_len; i++)
            {
                if((ch_32x32_lib[i].index[0] == ch[ch_cnt * 2]) && (ch_32x32_lib[i].index[1] == ch[ch_cnt * 2 + 1]))
                {
                    for(j = 0; j < 128; j++)
                    {
                        for(bit_cnt = 0; bit_cnt < 8; bit_cnt++)
                        {
                            if(ch_32x32_lib[i].mat[j] & (1 << bit_cnt))
                            {
                                lcd_draw_pixel(x + ch_cnt * 32 + (j % 4) * 8 + bit_cnt, y + j / 4, color);
                            }
                        }
                    }
                }
            }
        }
    }

    return 1;
}



//------ show 32X32 ------//
int show_32x32_ch_vertical(uint16_t x, uint16_t y, const char * ch, uint16_t ch_len, uint16_t color)
{
    int i = 0;
    int ch_lib_len = sizeof(ch_32x32_lib_vertical) / sizeof(ch_32x32_lib_vertical[0]);
    uint8_t j = 0;
    uint8_t bit_cnt = 0;
    uint16_t ch_cnt = 0;

    for(ch_cnt = 0; ch_cnt < ch_len; ch_cnt++)
    {
        if((x < LCD_WIDTH - 32 + 1) && (y < LCD_LENGTH - 32 + 1))
        {
            for(i = 0; i < ch_lib_len; i++)
            {
                if((ch_32x32_lib_vertical[i].index[0] == ch[ch_cnt * 2]) && (ch_32x32_lib_vertical[i].index[1] == ch[ch_cnt * 2 + 1]))
                {
                    for(j = 0; j < 128; j++)
                    {
                        for(bit_cnt = 0; bit_cnt < 8; bit_cnt++)
                        {
                            if(ch_32x32_lib_vertical[i].mat[j] & (1 << bit_cnt))
                            {
                                lcd_draw_pixel(x + ch_cnt * 32 +  j / 4, y + (j % 4) * 8 + bit_cnt, color);
                            }
                        }
                    }
                }
            }
        }
    }

    return 1;
}



//------ show 32X32 ------//
int show_32x32_ch_vertical_ex(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, const char * ch, uint16_t ch_len, uint8_t roll_flag, uint16_t color)
{
    int i = 0;
    int ch_lib_len = sizeof(ch_32x32_lib_vertical) / sizeof(ch_32x32_lib_vertical[0]);
    uint8_t j = 0;
    uint8_t bit_cnt = 0;
    uint16_t ch_cnt = 0;

    for(ch_cnt = 0; ch_cnt < ch_len; ch_cnt++)
    {
        if((x_start < LCD_WIDTH - 32 + 1) && (x_start < LCD_LENGTH - 32 + 1))
        {
            for(i = 0; i < ch_lib_len; i++)
            {
                if((ch_32x32_lib_vertical[i].index[0] == ch[ch_cnt * 2]) && (ch_32x32_lib_vertical[i].index[1] == ch[ch_cnt * 2 + 1]))
                {
                    for(j = 0; j < 128; j++)
                    {
                        for(bit_cnt = 0; bit_cnt < 8; bit_cnt++)
                        {
                            if(ch_32x32_lib_vertical[i].mat[j] & (1 << bit_cnt))
                            {
                                lcd_draw_pixel(x_start + ch_cnt * 32 +  j / 4, x_start + (j % 4) * 8 + bit_cnt, color);
                            }
                        }
                    }
                }
            }
        }
    }

    return 1;
}



//------ show 32X32 ------//
int show_part_32x32_ch_vertical(\
    uint16_t x, uint16_t y, uint8_t part_x_start, uint8_t part_y_start, uint8_t part_x_end, uint8_t part_y_end, const char * ch, uint16_t ch_len, uint16_t color)
{
    int i = 0;
    int ch_lib_len = sizeof(ch_32x32_lib_vertical) / sizeof(ch_32x32_lib_vertical[0]);
    uint8_t j = 0;
    uint8_t bit_cnt = 0;
    uint16_t ch_cnt = 0;
    if((part_x_start > part_x_end) || (part_y_start > part_y_end))
    {
        return -1;
    }
    for(ch_cnt = 0; ch_cnt < ch_len; ch_cnt++)
    {
        if((x < LCD_WIDTH - 32 + 1) && (y < LCD_LENGTH - 32 + 1))
        {
            for(i = 0; i < ch_lib_len; i++)
            {
                if((ch_32x32_lib_vertical[i].index[0] == ch[ch_cnt * 2]) && (ch_32x32_lib_vertical[i].index[1] == ch[ch_cnt * 2 + 1]))
                {
                    for(j = 0; j < 128; j++)
                    {
                        for(bit_cnt = 0; bit_cnt < 8; bit_cnt++)
                        {
                            if(ch_32x32_lib_vertical[i].mat[j] & (1 << bit_cnt))
                            {
                                lcd_draw_pixel(x + ch_cnt * 32 +  j / 4, y + (j % 4) * 8 + bit_cnt, color);
                            }
                        }
                    }
                }
            }
        }
    }

    return 1;
}
