#ifndef __HARDWARE_SPI_H__
#define __HARDWARE_SPI_H__
#include "stm32f10x.h"
#include "sys.h"

void SPI2_Init(void);			 //��ʼ��SPI��
void SPI2_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�

#endif
