#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	
#include "includes.h"
#include "lcd.h"
#include "math.h"
#include "show_ch.h"
#include "show_pic.h"

/////////////////////////UCOSII�����ջ����///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO      			10 //��ʼ��������ȼ�����Ϊ���
//���������ջ��С
#define START_STK_SIZE  				64
//���������ջ�ռ�	
OS_STK START_TASK_STK[START_STK_SIZE];
//�������ӿ�
void start_task(void *pdata);	
 			   
//LED1����
//�����������ȼ�
#define LED1_TASK_PRIO       			7 
//���������ջ��С
#define LED1_STK_SIZE  		    		64
//���������ջ�ռ�	
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//�������ӿ�
void led1_task(void *pdata);


//LED2����
//�����������ȼ�
#define LED2_TASK_PRIO       			6 
//���������ջ��С
#define LED2_STK_SIZE  					64
//���������ջ�ռ�	
OS_STK LED2_TASK_STK[LED2_STK_SIZE];

#define DIS_TEST_TASK_PRIO       			8
#define DIS_TEST_STK_SIZE  					1024
OS_STK DIS_TEST_TASK_STK[DIS_TEST_STK_SIZE];

//�������ӿ�
void led2_task(void *pdata);

void dis_test_task(void *pdata);

int main(void)
{
    delay_init();	     //��ʱ��ʼ��	  
    NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�
    lcd_init();
    lcd_color_box(0,0,320,480,Red);
    OSInit();
    OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
    OSStart();
}


//��ʼ����
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
	OSStatInit();					//��ʼ��ͳ������.�������ʱ1��������	
 	OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)    
 	OSTaskCreate(led1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);						   
 	OSTaskCreate(led2_task,(void *)0,(OS_STK*)&LED2_TASK_STK[LED2_STK_SIZE-1],LED2_TASK_PRIO);	 
    OSTaskCreate(dis_test_task,(void *)0,(OS_STK*)&DIS_TEST_TASK_STK[DIS_TEST_STK_SIZE - 1],DIS_TEST_TASK_PRIO);	     
	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
}

//LED1����
void led1_task(void *pdata)
{	 	
	while(1)
	{
		//LED0=1;
		delay_ms(80);
		LED0=0;
		delay_ms(920);
	};
}

//LED2����
void led2_task(void *pdata)
{	  
	while(1)
	{
		LED1=0;
		delay_ms(300);
		LED1=1;
		delay_ms(300);
	};
}

extern int show_pic(u16 x, u16 y, u16 pic_H, u16 pic_V, const unsigned char* pic);
extern void lcd_color_box_ex(u16 xStart, u16 yStart, u16 xLong, u16 yLong, const unsigned char* pic);
extern void LCD_Fill_Pic(u16 x, u16 y,u16 pic_H, u16 pic_V, const unsigned char* pic);
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
        lcd_color_box(0,0,320,480,White);delay_ms(1000);
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


//        show_ch(100, 100, "��", 1, Red);
//        delay_ms(1000);
//        show_ch(120, 120, "��", 1, Red);
//        delay_ms(1000);
//        show_ch(140, 140, "��", 1, Red);
//        delay_ms(1000);
//        show_ch(160, 160, "��", 1, Red);
//        delay_ms(1000);
        for(i = 0; i < 480; i += 32)
        {
            //show_16x16_ch(0, i, "�ֿ�����ֿ�����ֿ�����ֿ�����ֿ�����ֿ�����ֿ���ֿ�����ֿ�����ֿ�����ֿ����", 20, Red);
            //show_ch(0, 320, "�ֿ�����ֿ�����ֿ�����ֿ�����ֿ�����ֿ�����ֿ���ֿ�����ֿ�����ֿ�����ֿ����", 20, Red);
            //show_32x32_ch(0, i, "�ֿ�����������ֿ�����������ֿ�����ֿ�����ֿ�����ֿ�����ֿ���ֿ�����ֿ�����ֿ�����ֿ����", 10, Red);
            show_32x32_ch_vertical(0, i, "����ŵ��", 4, Red);
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
        for(i = 0; i < 480; i += 10)
        {
            lcd_color_box_ex(0, i, 320, 480 - i, gImage_MM_T035);
        }
        delay_ms(5000);
    }
    

}
