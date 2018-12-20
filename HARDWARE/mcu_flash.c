#include "mcu_flash.h"
#include "delay.h"
#include "usart.h"
#include "string.h"


//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr;
}
#if STM32_FLASH_WREN	//如果使能了写
//不检查的写入
//write_addr:起始地址
//pBuffer:数据指针
//num_to_write:半字(16位)数
void STMFLASH_Write_NoCheck(u32 write_addr, u16 *pBuffer, u16 num_to_write)
{
    u16 i;
    for(i = 0; i < num_to_write; i++)
    {
        FLASH_ProgramHalfWord(write_addr, pBuffer[i]);
        write_addr += 2;//地址增加2.
    }
}
//从指定地址开始写入指定长度的数据
//write_addr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//num_to_write:半字(16位)数(就是要写入的16位数据的个数.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else
#define STM_SECTOR_SIZE	2048
#endif
u16 mcu_flash_buf[STM_SECTOR_SIZE/2];//最多是2K字节
void flash_write(u32 write_addr,u16 *pBuffer,u16 num_to_write)
{
    u32 secpos;	   //扇区地址
    u16 secoff;	   //扇区内偏移地址(16位字计算)
    u16 secremain; //扇区内剩余地址(16位字计算)
    u16 i;
    u32 offaddr;   //去掉0X08000000后的地址
    if(write_addr<STM32_FLASH_BASE||(write_addr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//非法地址
    FLASH_Unlock();						//解锁
    offaddr=write_addr-STM32_FLASH_BASE;		//实际偏移地址.
    secpos=offaddr/STM_SECTOR_SIZE;			//扇区地址  0~127 for STM32F103RBT6
    secoff=(offaddr%STM_SECTOR_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
    secremain=STM_SECTOR_SIZE/2-secoff;		//扇区剩余空间大小
    if(num_to_write<=secremain)secremain=num_to_write;//不大于该扇区范围
    while(1)
    {
        flash_read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,mcu_flash_buf,STM_SECTOR_SIZE/2);//读出整个扇区的内容
        for(i=0;i<secremain;i++)//校验数据
        {
            if(mcu_flash_buf[secoff+i]!=0XFFFF)break;//需要擦除
        }
        if(i<secremain)//需要擦除
        {
            FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个扇区
            for(i=0;i<secremain;i++)//复制
            {
            mcu_flash_buf[i+secoff]=pBuffer[i];
            }
            STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,mcu_flash_buf,STM_SECTOR_SIZE/2);//写入整个扇区
        }else STMFLASH_Write_NoCheck(write_addr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间.
        if(num_to_write==secremain)break;//写入结束了
        else//写入未结束
        {
            secpos++;				//扇区地址增1
            secoff=0;				//偏移位置为0
            pBuffer+=secremain;  	//指针偏移
            write_addr+=secremain;	//写地址偏移
            num_to_write-=secremain;	//字节(16位)数递减
            if(num_to_write>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//下一个扇区还是写不完
            else secremain=num_to_write;//下一个扇区可以写完了
        }
    };
    FLASH_Lock();//上锁
}
#endif

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//num_to_write:半字(16位)数
void flash_read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//write_addr:起始地址
//WriteData:要写入的数据
void Test_Write(u32 write_addr,u16 WriteData)
{
	flash_write(write_addr,&WriteData,1);//写入一个字
}


#define SUPER_RFID_DEFAULT_VALUE        "6666"
#define SUPER_PASSWORD_DEFAULT_VALUE    "6666"
#define RFID_SIZE           4
#define PASSWORD_SIZE       4
#define FLASH_SAVE_ADDR     0X08070000 				//设置FLASH 保存地址(必须为偶数)

#define FLASH_PASSWORD_ADDR         0X08070000
#define FLASH_RFID_ADDR             0X08070010

void get_rfid_in_flash(char *rfid)
{
    char rfid_tmp[4] = {0};

    flash_read(FLASH_RFID_ADDR, (u16*)rfid_tmp, RFID_SIZE);
    for(uint8_t i = 0; i < RFID_SIZE; i++)
    {
        if( (rfid_tmp[i] > '9') || (rfid_tmp[i] < '0') )
        {
            // value invalid !
            memcpy( rfid, SUPER_RFID_DEFAULT_VALUE, RFID_SIZE);
            return ;
        }
    }
    memcpy( rfid, rfid_tmp, RFID_SIZE);
}

void get_password_in_flash(char *password)
{
    char password_tmp[4] = {0};

    flash_read(FLASH_PASSWORD_ADDR, (u16*)password_tmp, PASSWORD_SIZE);
    for(uint8_t i = 0; i < RFID_SIZE; i++)
    {
        if( (password_tmp[i] > '9') || (password_tmp[i] < '0') )
        {
            // value invalid !
            memcpy( password, SUPER_PASSWORD_DEFAULT_VALUE, RFID_SIZE);
            return ;
        }
    }
    memcpy( password, password_tmp, RFID_SIZE);
}

void save_rfid_to_flash(char *rfid)
{
    flash_write(FLASH_RFID_ADDR,(u16*)rfid, RFID_SIZE);
}

void save_password_to_flash(char *password)
{
    flash_write(FLASH_PASSWORD_ADDR, (u16*)password, PASSWORD_SIZE);
}


