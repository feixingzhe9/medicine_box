#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	
#include "includes.h"
#include "LCD.h"

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
#define DIS_TEST_STK_SIZE  					512
OS_STK DIS_TEST_TASK_STK[DIS_TEST_STK_SIZE];

//�������ӿ�
void led2_task(void *pdata);

void dis_test_task(void *pdata);

 int main(void)
 {
     int i = 0;
	delay_init();	     //��ʱ��ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�
    Lcd_Initialize();
    Lcd_ColorBox(0,0,320,480,Red);
//    for(i = 0; i < 100000000; i++)
//    {
//        __asm("nop")
//    }
//	Lcd_ColorBox(0,0,320,480,White);//Delay(5000000);
//    for(i = 0; i < 100000000; i++)
//    {
//        __asm("nop")
//    }
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

void dis_test_task(void *pdata)
{
    int i = 0;
    while(1)
    {
        Lcd_ColorBox(0,0,320,480,Red);delay_ms(1000);
        Lcd_ColorBox(0,0,320,480,Blue);delay_ms(1000);
        Lcd_ColorBox(0,0,320,480,White);delay_ms(1000);
        Lcd_ColorBox(0,0,20,80,Green);delay_ms(1000);
        Lcd_ColorBox(100,100,20,80,Green);delay_ms(1000);
        Lcd_ColorBox(200,200,20,80,Green);delay_ms(1000);
        for(i = 0; i < 320; i++)
        {
            DrawPixel(i, i, Red);
        }
        delay_ms(1000);
        for(i = 0; i < 320; i++)
        {
            DrawPixel(320 - i, i, Red);
        }
        delay_ms(1000);
        for(i = 0; i < 320; i++)
        {
            DrawPixel(180, i, Red);
        }
        delay_ms(1000);
        for(i = 0; i < 320; i++)
        {
            DrawPixel(i, 180, Red);
        }
        delay_ms(1000);
        
}

}
