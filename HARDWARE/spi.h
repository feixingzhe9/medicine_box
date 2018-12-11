#ifndef __HARDWARE_SPI_H__
#define __HARDWARE_SPI_H__
#include "stm32f10x.h"
#include "sys.h"

void spi2_init(void);               //��ʼ��SPI��
void spi2_set_speed(u8 SpeedSet); //����SPI�ٶ�
u8 spi2_read_write_byte(u8 TxData);//SPI���߶�дһ���ֽ�

#endif
