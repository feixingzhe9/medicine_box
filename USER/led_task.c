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
OS_STK LED2_TASK_STK[LED2_STK_SIZE];


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

#include "lock.h"
void led2_task(void *pdata)
{
    led_init();
    while(1)
    {
        LED1=0;
//        lock_on();
        delay_ms(500);
        LED1=1;
//        lock_off();
        delay_ms(500);
    };
}





uint32_t cal_ascii_addr(char ascii)
{
    uint32_t base_addr = 0x1F6500;
    uint32_t addr = 0;
    if((ascii >= 0x20) && (ascii < 0x7e))
    {
        addr = ascii - 0x20;
        addr *= 48;
        addr += base_addr;
        return addr;
//        return ((ascii - 0x20 ) * 48 + base_addr);
    }
    return 0;
}




uint32_t cal_ascii_8X16_addr(char ascii)
{
    uint32_t base_addr = 0x100600;
    uint32_t addr = 0;
    if((ascii >= 0x20) && (ascii < 0x7e))
    {
        addr = ascii - 0x20;
        addr *= 16;
        addr += base_addr;
        return addr;
//        return ((ascii - 0x20 ) * 48 + base_addr);
    }
    return 0;
}


uint32_t get_ch_addr(unsigned char c1, unsigned char c2, unsigned char c3, unsigned char c4)
{
    uint32_t h=0;

    if(c2==0x7f)
        return (h);

    if(c1>=0xA1 && c1 <= 0xAB && c2>=0xa1) //Section 1
    {
        h= (c1 - 0xA1) * 94 + (c2 - 0xA1);
    }
    else if(c1>=0xa8 && c1 <= 0xa9 && c2<0xa1) //Section 5
    {
        if(c2>0x7f)
        {
            c2--;
        }
        h=(c1-0xa8)*96 + (c2-0x40)+846;
    }
    if(c1>=0xb0 && c1 <= 0xf7 && c2>=0xa1) //Section 2
    {
        h= (c1 - 0xB0) * 94 + (c2 - 0xA1)+1038;
    }
    else if(c1<0xa1 && c1>=0x81 && c2>=0x40) //Section 3
    {
        if(c2>0x7f)
        {
            c2--;
        }
        h=(c1-0x81)*190 + (c2-0x40) + 1038 +6768;
    }
    else if(c1>=0xaa && c2<0xa1) //Section 4
    {
        if(c2>0x7f)
        {
            c2--;
        }
        h=(c1-0xaa)*96 + (c2-0x40) + 1038 +12848;
    }
    else if(c1==0x81 && c2>=0x39) // 四字节区 1
    {
        h =1038 + 21008+(c3-0xEE)*10+c4-0x39;
    }
    else if(c1==0x82)//四字节区 2
    {
        h =1038 + 21008+161+(c2-0x30)*1260+(c3-0x81)*10+c4-0x30;
    }
    return(h);
}





uint8_t A_lib[] =
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x50,0x00,
0xD0,0x00,0x90,0x00,0x90,0x00,0x88,0x00,0x88,0x01,0x08,0x01,0xF8,0x01,0x04,0x03,
0x04,0x03,0x04,0x02,0x02,0x02,0x02,0x06,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,/*"A",0*/
};

uint8_t ch_lib_1[] =
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x1C,0x00,0x00,0x0C,0x00,0x00,
    0x0C,0x00,0x00,0x1C,0x00,0x00,0x1C,0x00,0x00,0x1C,0x00,0x00,0x1C,0x00,0x00,0x24,
    0x00,0x00,0x24,0x00,0x00,0x26,0x00,0x00,0x46,0x00,0x00,0x42,0x00,0x00,0xC3,0x00,
    0x00,0x81,0x01,0x80,0x01,0x01,0xC0,0x00,0x03,0x60,0x00,0x0E,0x30,0x00,0x3C,0x08,
    0x00,0x38,0x06,0x00,0x00,0x00,0x00,0x00/*"人",0*/
};

uint8_t A_8X16_lib[] =
{
    0x00,0x00,0x00,0x08,0x08,0x18,0x14,0x14,0x24,0x3C,0x22,0x42,0x42,0xE7,0x00,0x00
};

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
    SPI_Flash_Read(p_arr, address, byte_long);
    return 0;
}

void falsh_test_task(void *pdata)
{
    char a = 0;
    while(1)
    {
#if 0
//        delay_ms(1000);
//        lcd_color_box(0, 0, LCD_X_MAX, LCD_Y_MAX, White);delay_ms(1000);
//        SPI_Flash_Read(flash_read_test_buf, cal_ascii_addr('C'), 48);
////        SPI_Flash_Read(flash_read_test_buf, 0x1F6500 + 48 * 0x0d, 48);
//        show_12X24_ch(160, 60, 'A', 1, 0x001f, flash_read_test_buf);delay_ms(1000);
//        show_8X16_ch(160, 160, 'b', 1, 0x001f, flash_read_test_buf);delay_ms(1000);
//        show_12X24_ch(190, 90, 'A', 1, 0x001f, A_lib);delay_ms(1000);


//        SPI_Flash_Read(flash_read_test_buf, get_ch_addr(0xba, 0xc3, 0, 0), 72);
////        SPI_Flash_Read(flash_read_test_buf, get_ch_addr(0x81, 0x39, 0xee, 0x39), 72);
//        show_24X24_ch(130, 30, "人", 1, 0x001F, flash_read_test_buf);
//        delay_ms(1000);

//        show_24X24_ch(130, 60, "人", 1, 0x001F, ch_lib_1);
//        delay_ms(1000);
#else
        delay_ms(100);
        lcd_color_box(0, 0, LCD_X_MAX, LCD_Y_MAX, White);delay_ms(500);
//        ASCII_GetData('F', ASCII_8X16, flash_read_test_buf);
//        show_8X16_ch(160, 160, 'b', 1, 0x001f, flash_read_test_buf);delay_ms(1000);

//        show_8X16_ch_vertical(0, 160, 'b', 1, 0x001f, flash_read_test_buf);delay_ms(1000);


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
//        SPI_Flash_Read(flash_read_test_buf, get_ch_addr(0xba, 0xc3, 0, 0), 72);
////        SPI_Flash_Read(flash_read_test_buf, get_ch_addr(0x81, 0x39, 0xee, 0x39), 72);
//        show_24X24_ch(130, 30, "人", 1, 0x001F, flash_read_test_buf);
//        delay_ms(1000);

//        show_24X24_ch(130, 60, "人", 1, 0x001F, ch_lib_1);
//        delay_ms(1000);
//
//        SPI_Flash_Read(flash_read_test_buf, cal_ascii_8X16_addr('Z'), 16);
//        show_8X16_ch(260, 160, 'b', 1, 0x001f, flash_read_test_buf);delay_ms(1000);
#endif
    }
}




