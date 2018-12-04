#ifndef __FLASH_H
#define __FLASH_H
#include "sys.h"

//W25Xϵ��/Qϵ��оƬ�б�
//W25Q80 ID  0XEF13
//W25Q16 ID  0XEF14
//W25Q32 ID  0XEF15
//W25Q32 ID  0XEF16
#define W25Q80 	0XEF13
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16

extern u16 SPI_FLASH_TYPE;//��������ʹ�õ�flashоƬ�ͺ�

#define	SPI2_FLASH_CS PBout(12)  //ѡ��FLASH

////////////////////////////////////////////////////////////////////////////

//ָ���
#define W25X_WriteEnable		0x06
#define W25X_WriteDisable		0x04
#define W25X_ReadStatusReg		0x05
#define W25X_WriteStatusReg		0x01
#define W25X_ReadData			0x03
#define W25X_FastReadData		0x0B
#define W25X_FastReadDual		0x3B
#define W25X_PageProgram		0x02
#define W25X_BlockErase			0xD8
#define W25X_SectorErase		0x20
#define W25X_ChipErase			0xC7
#define W25X_PowerDown			0xB9
#define W25X_ReleasePowerDown	0xAB
#define W25X_DeviceID			0xAB
#define W25X_ManufactDeviceID	0x90
#define W25X_JedecDeviceID		0x9F

void spi2_flash_init(void);
u16  spi2_flash_read_id(void);  	    //��ȡFLASH ID
u8	 spi2_flash_read_sr(void);        //��ȡ״̬�Ĵ���
void spi2_flash_wirete_sr(u8 sr);  	//д״̬�Ĵ���
void spi2_flash_write_enable(void);  //дʹ��
void spi2_flash_write_disable(void);	//д����
void spi2_flash_write_no_check(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void spi2_flash_read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //��ȡflash
void spi2_flash_write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//д��flash
void spi2_flash_erase_chip(void);    	  //��Ƭ����
void spi2_flash_erase_sector(u32 Dst_Addr);//��������
void spi2_flash_wait_busy(void);           //�ȴ�����
void spi2_flash_power_dow(void);           //�������ģʽ
void spi2_flash_wakeup(void);			  //����
#endif



