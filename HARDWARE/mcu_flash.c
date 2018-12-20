#include "mcu_flash.h"
#include "delay.h"
#include "usart.h"
#include "string.h"


//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr;
}
#if STM32_FLASH_WREN	//���ʹ����д
//������д��
//write_addr:��ʼ��ַ
//pBuffer:����ָ��
//num_to_write:����(16λ)��
void STMFLASH_Write_NoCheck(u32 write_addr, u16 *pBuffer, u16 num_to_write)
{
    u16 i;
    for(i = 0; i < num_to_write; i++)
    {
        FLASH_ProgramHalfWord(write_addr, pBuffer[i]);
        write_addr += 2;//��ַ����2.
    }
}
//��ָ����ַ��ʼд��ָ�����ȵ�����
//write_addr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//num_to_write:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else
#define STM_SECTOR_SIZE	2048
#endif
u16 mcu_flash_buf[STM_SECTOR_SIZE/2];//�����2K�ֽ�
void flash_write(u32 write_addr,u16 *pBuffer,u16 num_to_write)
{
    u32 secpos;	   //������ַ
    u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
    u16 secremain; //������ʣ���ַ(16λ�ּ���)
    u16 i;
    u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
    if(write_addr<STM32_FLASH_BASE||(write_addr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
    FLASH_Unlock();						//����
    offaddr=write_addr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
    secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
    secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
    secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С
    if(num_to_write<=secremain)secremain=num_to_write;//�����ڸ�������Χ
    while(1)
    {
        flash_read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,mcu_flash_buf,STM_SECTOR_SIZE/2);//������������������
        for(i=0;i<secremain;i++)//У������
        {
            if(mcu_flash_buf[secoff+i]!=0XFFFF)break;//��Ҫ����
        }
        if(i<secremain)//��Ҫ����
        {
            FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
            for(i=0;i<secremain;i++)//����
            {
            mcu_flash_buf[i+secoff]=pBuffer[i];
            }
            STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,mcu_flash_buf,STM_SECTOR_SIZE/2);//д����������
        }else STMFLASH_Write_NoCheck(write_addr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.
        if(num_to_write==secremain)break;//д�������
        else//д��δ����
        {
            secpos++;				//������ַ��1
            secoff=0;				//ƫ��λ��Ϊ0
            pBuffer+=secremain;  	//ָ��ƫ��
            write_addr+=secremain;	//д��ַƫ��
            num_to_write-=secremain;	//�ֽ�(16λ)���ݼ�
            if(num_to_write>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
            else secremain=num_to_write;//��һ����������д����
        }
    };
    FLASH_Lock();//����
}
#endif

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//num_to_write:����(16λ)��
void flash_read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//write_addr:��ʼ��ַ
//WriteData:Ҫд�������
void Test_Write(u32 write_addr,u16 WriteData)
{
	flash_write(write_addr,&WriteData,1);//д��һ����
}


#define SUPER_RFID_DEFAULT_VALUE        "6666"
#define SUPER_PASSWORD_DEFAULT_VALUE    "6666"
#define RFID_SIZE           4
#define PASSWORD_SIZE       4
#define FLASH_SAVE_ADDR     0X08070000 				//����FLASH �����ַ(����Ϊż��)

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


