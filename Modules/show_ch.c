

#include "show_ch.h"
#include "ch_lib.h"
#include "lcd.h"

int show_ch(uint16_t x, uint16_t y, const char * ch, uint16_t ch_len, uint16_t color)
{
    int i = 0;
    int ch_lib_len = sizeof(ch_lib) / sizeof(ch_lib[0]);
    uint8_t j = 0;
    uint8_t bit_cnt = 0;
    uint16_t ch_cnt = 0;
    for(ch_cnt = 0; ch_cnt < ch_len; ch_cnt++)
    {
        if((x < LCD_WIDTH - 16) && (y < LCD_LENGTH - 16))
        {
            for(i = 0; i < ch_lib_len; i++)
            {
                if((ch_lib[i].index[0] == ch[ch_cnt * 2]) && (ch_lib[i].index[1] == ch[ch_cnt * 2 + 1]))
                {
                    for(j = 0; j < 32; j++)
                    {
                        for(bit_cnt = 0; bit_cnt < 8; bit_cnt++)
                        {
                            if(ch_lib[i].mat[j] & (1 << bit_cnt))
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
