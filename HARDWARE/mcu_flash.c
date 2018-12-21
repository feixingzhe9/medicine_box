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
//buffer:����ָ��
//num_to_write:����(16λ)��
void STMFLASH_Write_NoCheck(u32 write_addr, u16 *buffer, u16 num_to_write)
{
    u16 i;
    for(i = 0; i < num_to_write; i++)
    {
        FLASH_ProgramHalfWord(write_addr, buffer[i]);
        write_addr += 2;//��ַ����2.
    }
}
//��ָ����ַ��ʼд��ָ�����ȵ�����
//write_addr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//buffer:����ָ��
//num_to_write:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
u16 mcu_flash_buf[STM_SECTOR_SIZE / 2];//�����2K�ֽ�
void flash_write(u32 write_addr, u16 *buffer, u16 num_to_write)
{
    u32 sector_pos;     //������ַ
    u16 sector_offset;     //������ƫ�Ƶ�ַ(16λ�ּ���)
    u16 sector_remain;  //������ʣ���ַ(16λ�ּ���)
    u16 i;
    u32 offset_addr;    //ȥ��0X08000000��ĵ�ַ
    if(write_addr < STM32_FLASH_BASE || (write_addr >= (STM32_FLASH_BASE + 1024 * STM32_FLASH_SIZE)))
    {
        return;//�Ƿ���ַ
    }
    FLASH_Unlock();     //����
    offset_addr = write_addr - STM32_FLASH_BASE;    //ʵ��ƫ�Ƶ�ַ.
    sector_pos = offset_addr / STM_SECTOR_SIZE;         //������ַ  0~127 for STM32F103RBT6
    sector_offset = (offset_addr % STM_SECTOR_SIZE) / 2;   //�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
    sector_remain = STM_SECTOR_SIZE / 2 - sector_offset;   //����ʣ��ռ��С
    if(num_to_write <= sector_remain)
    {
        sector_remain = num_to_write;//�����ڸ�������Χ
    }
    while(1)
    {
        flash_read(sector_pos * STM_SECTOR_SIZE + STM32_FLASH_BASE, mcu_flash_buf, STM_SECTOR_SIZE / 2);//������������������
        for(i = 0; i < sector_remain; i++)//У������
        {
            if(mcu_flash_buf[sector_offset + i] != 0XFFFF)
            {
                break;//��Ҫ����
            }
        }
        if(i < sector_remain)//��Ҫ����
        {
            FLASH_ErasePage(sector_pos * STM_SECTOR_SIZE + STM32_FLASH_BASE);//�����������
            for(i = 0; i < sector_remain; i++)//����
            {
                mcu_flash_buf[i + sector_offset] = buffer[i];
            }
            STMFLASH_Write_NoCheck(sector_pos * STM_SECTOR_SIZE + STM32_FLASH_BASE, mcu_flash_buf, STM_SECTOR_SIZE / 2);//д����������
        }else
        {
            STMFLASH_Write_NoCheck(write_addr, buffer, sector_remain);//д�Ѿ������˵�,ֱ��д������ʣ������.
        }
        if(num_to_write == sector_remain)
        {
            break;//д�������
        }
        else//д��δ����
        {
            sector_pos++;               //������ַ��1
            sector_offset = 0;             //ƫ��λ��Ϊ0
            buffer += sector_remain;   //ָ��ƫ��
            write_addr += sector_remain;    //д��ַƫ��
            num_to_write -= sector_remain;  //�ֽ�(16λ)���ݼ�
            if(num_to_write > (STM_SECTOR_SIZE / 2))
            {
                sector_remain = STM_SECTOR_SIZE / 2;//��һ����������д����
            }
            else
            {
                sector_remain = num_to_write;//��һ����������д����
            }
        }
    };
    FLASH_Lock();//����
}
#endif

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//buffer:����ָ��
//num_to_write:����(16λ)��
void flash_read(u32 ReadAddr,u16 *buffer,u16 NumToRead)
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		buffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.
	}
}


uint8_t flash_erase(uint32_t addr)
{
    if(addr % STM_SECTOR_SIZE == 0)
    {
        FLASH_Unlock(); //����
        FLASH_ErasePage(addr);
        FLASH_Lock();   //����
        return 1;
    }
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//write_addr:��ʼ��ַ
//WriteData:Ҫд�������
void Test_Write(u32 write_addr,u16 WriteData)
{
	flash_write(write_addr,&WriteData,1);//д��һ����
}



