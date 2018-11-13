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
#define Bank1_LCD_D    ((u32)0x60020000)    //Disp Data ADDR
#define Bank1_LCD_C    ((u32)0x60000000)	   //Disp Reg ADDR

#define Set_Rst GPIOD->BSRR = GPIO_Pin_13;
#define Clr_Rst GPIOD->BRR  = GPIO_Pin_13;

#define Lcd_Light_ON   GPIOA->BSRR = GPIO_Pin_1;
#define Lcd_Light_OFF  GPIOA->BRR  = GPIO_Pin_1;


//typedef enum
//{
//    DIRECTION_VERTICAL = 1,
//    DIRECTION_HORIZONTAL,
//}dis_direction_enum;

#define DIRECTION_VERTICAL          0x0001
#define DIRECTION_HORIZONTAL        0x0002

#define DIRECTION_TYPE_A            0x0100
#define DIRECTION_TYPE_B            0x0200

#define DIS_DIRECTION   (DIRECTION_VERTICAL | DIRECTION_TYPE_A)

#if (DIS_DIRECTION & DIRECTION_VERTICAL)

#define LCD_Y_MAX       480
#define LCD_X_MAX       320

#elif (DIS_DIRECTION & DIRECTION_HORIZONTAL)

#define LCD_Y_MAX       320
#define LCD_X_MAX       480

#endif

//Lcd��ʼ������ͼ����ƺ���
void Lcd_Configuration(void);
void DataToWrite(u16 data);
void lcd_init(void);
void lcd_wr_reg(u16 Index,u16 CongfigTemp);
void Lcd_WR_Start(void);
//Lcd�߼����ƺ���
void lcd_color_box(u16 x,u16 y,u16 xLong,u16 yLong,u16 Color);
void lcd_draw_pixel(u16 x, u16 y, u16 Color);
u16 lcd_read_pixel(u16 x,u8 y);
void lcd_fill_pic(u16 x, u16 y,u16 pic_H, u16 pic_V, const unsigned char* pic);
void lcd_block_write(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);
char Tiky_Button(char *filename,u16 x,u16 y);

/*���峣����ɫ*/
// #define red 0x001f
// #define blue 0xf800
// #define green 0x07e0
// #define black 0x0000
// #define white 0xffff
// #define yellow 0x07ff
// #define orange 0x05bf
// #define Chocolate4 0x4451
// #define Grey 0xefbd//��ɫ

#define White          0xFFFF
#define Black          0x0000
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

#endif
