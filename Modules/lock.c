#include "lock.h"


void lock_gpio_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //使能端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 // 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOG, &GPIO_InitStructure);					 //根据设定参数初始化GPIO
//    GPIO_SetBits(GPIOG,GPIO_Pin_5);						 // 输出高
    GPIO_ResetBits(GPIOG,GPIO_Pin_5);						 // 输出高
}


void init_lock(void)
{
    lock_gpio_init();
}


void lock_on(void)
{
    GPIO_SetBits(GPIOG,GPIO_Pin_5);
}

void lock_off(void)
{
    GPIO_ResetBits(GPIOG,GPIO_Pin_5);
}

