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


void led2_task(void *pdata)
{
    led_init();
    while(1)
    {
        LED1=0;
        delay_ms(500);
        LED1=1;
        delay_ms(500);
    };
}





uint32_t cal_ascii_addr(char ascii)
{
    uint32_t base_addr = 0x1F6500;
    if((ascii >= 0x20) && (ascii < 0x7e))
    {
        return  (ascii - 0x20 ) * 48 + base_addr;
    }
    return 0;
}






unsigned long gt (unsigned char c1, unsigned char c2, unsigned char c3, unsigned char c4)
{
    unsigned long h=0;

    if(c2==0x7f)
        return (h);

    if(c1>=0xA1 && c1 <= 0xAB && c2>=0xa1) //Section 1
        h= (c1 - 0xA1) * 94 + (c2 - 0xA1);
    else if(c1>=0xa8 && c1 <= 0xa9 && c2<0xa1) //Section 5
    {
        if(c2>0x7f)
        c2--;
        h=(c1-0xa8)*96 + (c2-0x40)+846;
    }
    if(c1>=0xb0 && c1 <= 0xf7 && c2>=0xa1) //Section 2
        h= (c1 - 0xB0) * 94 + (c2 - 0xA1)+1038;
    else if(c1<0xa1 && c1>=0x81 && c2>=0x40) //Section 3
    {
        if(c2>0x7f)
        c2--;
        h=(c1-0x81)*190 + (c2-0x40) + 1038 +6768;
    }
    else if(c1>=0xaa && c2<0xa1) //Section 4
    {
        if(c2>0x7f)
            c2--;
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


#include "flash.h"
extern int show_24X24_ch(uint16_t x, uint16_t y, const char * ch, uint16_t ch_len, uint16_t color, uint8_t *data);
extern int show_12X24_ch(uint16_t x, uint16_t y, char ch, uint16_t ch_len, uint16_t color, uint8_t *data);
OS_STK FLASH_TEST_TASK_STK[LED1_STK_SIZE];
uint8_t flash_read_test_buf[100];
void falsh_test_task(void *pdata)
{
    while(1)
    {
        delay_ms(1000);

        SPI_Flash_Read(flash_read_test_buf, cal_ascii_addr('A'), 48);
        show_12X24_ch(160, 60, 'A', 1, 0x001f, flash_read_test_buf);
        delay_ms(1000);

        show_12X24_ch(190, 90, 'A', 1, 0x001f, A_lib);
        delay_ms(1000);

        SPI_Flash_Read(flash_read_test_buf, gt(0xc3, 0xba, 0, 0), 72);
        show_24X24_ch(130, 30, "人", 1, 0x001F, flash_read_test_buf);
        delay_ms(1000);

        show_24X24_ch(130, 60, "人", 1, 0x001F, ch_lib_1);
        delay_ms(1000);
    }
}




