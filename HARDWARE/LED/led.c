#include "led.h"

void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG|RCC_APB2Periph_GPIOC, ENABLE);	 //使能端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 // 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOG, &GPIO_InitStructure);					 //根据设定参数初始化GPIO
    GPIO_ResetBits(GPIOG,GPIO_Pin_5);						 // 输出高

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	    		 // 端口配置, 推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
    GPIO_ResetBits(GPIOC,GPIO_Pin_9); 						 // 输出高
}

