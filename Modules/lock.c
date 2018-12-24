/*
 *  Author: Kaka Xie
 *  brief: lock operation
 */


#include "lock.h"


void lock_gpio_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);   //ʹ�ܶ˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;               // �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOG, &GPIO_InitStructure);                  //�����趨������ʼ��GPIO
//    GPIO_SetBits(GPIOG,GPIO_Pin_5);                       // �����
    GPIO_ResetBits(GPIOG,GPIO_Pin_5);                       // �����

    /* lock input detection config */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;               // �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;        //�������
    GPIO_Init(GPIOG, &GPIO_InitStructure);                  //�����趨������ʼ��GPIO

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

uint8_t get_lock_status(void)
{
    return GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_13);
}
