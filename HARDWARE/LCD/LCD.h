#ifndef __HARDWARE_LCD_H__
#define __HARDWARE_LCD_H__
#include "stm32f10x.h"

//��Ļ��ʼʱ��ʾ��ʽ��ע�⣺��IDelayʱ��ʾ��һ��������������ˢ�µ�
//��ʱ�����ֶ���ˢ�½��������  lcd_wr_reg(0x0007,0x0173);������ʾ
//��Immediatelyʱû�б�ע�͵��ǲ���Ҫ�˹���

/* ѡ��BANK1-BORSRAM1 ���� TFT����ַ��ΧΪ0X60000000~0X63FFFFFF
 * FSMC_A16 ��LCD��DC(�Ĵ���/����ѡ��)��
 * 16 bit => FSMC[24:0]��ӦHADDR[25:1]
 * �Ĵ�������ַ = 0X60000000
 * RAM����ַ = 0X60020000 = 0X60000000+2^16*2 = 0X60000000 + 0X20000 = 0X60020000
 * ��ѡ��ͬ�ĵ�ַ��ʱ����ַҪ���¼��㡣
 */

#define BANK_1_LCD_DATA     ((u32)0x60020000)//Disp Data ADDR
#define BANK_1_LCD_CMD      ((u32)0x60000000)//Disp Reg ADDR


#define LCD_SET_RST         do\
                            {\
                                GPIOD->BSRR = GPIO_Pin_13;\
                            }while(0)


#define LCD_CLR_RST         do\
                            {\
                                GPIOD->BRR  = GPIO_Pin_13;\
                            }while(0)


#define LCD_BACK_LIGHT_ON   do\
                            {\
                                GPIOA->BSRR = GPIO_Pin_1;\
                            }while(0)


#define LCD_BACK_LIGHT_OFF  do\
                            {\
                                GPIOA->BRR  = GPIO_Pin_1;\
                            }while(0)


#define DIRECTION_VERTICAL          0x0001
#define DIRECTION_HORIZONTAL        0x0002

#define DIRECTION_TYPE_A            0x0100
#define DIRECTION_TYPE_B            0x0200

#define DIS_DIRECTION   (DIRECTION_HORIZONTAL | DIRECTION_TYPE_A)

#if (DIS_DIRECTION & DIRECTION_VERTICAL)

#define LCD_Y_MAX       480
#define LCD_X_MAX       320

#elif (DIS_DIRECTION & DIRECTION_HORIZONTAL)

#define LCD_Y_MAX       320
#define LCD_X_MAX       480

#endif


void lcd_init(void);
void lcd_wr_reg(u16 index,u16 config);
void lcd_wr_start(void);

void lcd_color_box(u16 x_start, u16 y_start, u16 x_long, u16 y_long, u16 color);
void lcd_draw_pixel(u16 x, u16 y, u16 color);
u16 lcd_read_pixel(u16 x,u8 y);
void lcd_fill_pic(u16 x, u16 y,u16 pic_h, u16 pic_v, const unsigned char* pic);
void lcd_block_write(unsigned int x_start,unsigned int x_end,unsigned int y_start,unsigned int y_end);


#define White           0xFFFF
#define Black           0x0000
#define Blue            0x001F
#define Blue2           0x051F
#define Red             0xF800
#define Magenta         0xF81F
#define Green           0x07E0
#define Cyan            0x7FFF
#define Yellow          0xFFE0

#endif
