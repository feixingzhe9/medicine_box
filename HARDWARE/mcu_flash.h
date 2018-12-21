#ifndef __HARDWARE_MCU_FLASH_H__
#define __HARDWARE_MCU_FLASH_H__
#include "sys.h"
#include "stm32f10x_flash.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE    512            //��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN    1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000     //STM32 FLASH����ʼ��ַ
//FLASH������ֵ

#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else
#define STM_SECTOR_SIZE	2048
#endif


u16 STMFLASH_ReadHalfWord(u32 faddr);       //��������
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);  //ָ����ַ��ʼд��ָ�����ȵ�����
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);	                    //ָ����ַ��ʼ��ȡָ����������
void flash_write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);        //��ָ����ַ��ʼд��ָ�����ȵ�����
void flash_read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);           //��ָ����ַ��ʼ����ָ�����ȵ�����

//����д��
void Test_Write(u32 WriteAddr,u16 WriteData);

uint8_t flash_erase(uint32_t addr);

#endif

















