#include "flash.h"
#include "spi.h"
#include "delay.h"


//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25X16
//����Ϊ2M�ֽ�,����32��Block,512��Sector

//��ʼ��SPI FLASH��IO��
void spi2_flash_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  // PB12 ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_12);


    spi2_init();        //��ʼ��SPI
    spi2_set_speed(SPI_BaudRatePrescaler_2);//����Ϊ18Mʱ��,����ģʽ
    //SPI_FLASH_TYPE=spi2_flash_read_id();//��ȡFLASH ID.

}

//��ȡSPI_FLASH��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
uint8_t spi2_flash_read_sr(void)
{
    uint8_t byte = 0;
    SPI2_FLASH_CS = 0;                            //ʹ������
    spi2_read_write_byte(W25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������
    byte = spi2_read_write_byte(0Xff);             //��ȡһ���ֽ�
    SPI2_FLASH_CS = 1;                            //ȡ��Ƭѡ
    return byte;
}
//дSPI_FLASH״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void spi2_flash_wirete_sr(u8 sr)
{
    SPI2_FLASH_CS = 0;                              //ʹ������
    spi2_read_write_byte(W25X_WriteStatusReg);      //����дȡ״̬�Ĵ�������
    spi2_read_write_byte(sr);                       //д��һ���ֽ�
    SPI2_FLASH_CS = 1;                              //ȡ��Ƭѡ
}
//SPI_FLASHдʹ��
//��WEL��λ
void spi2_flash_write_enable(void)
{
    SPI2_FLASH_CS = 0;                            //ʹ������
    spi2_read_write_byte(W25X_WriteEnable);      //����дʹ��
    SPI2_FLASH_CS = 1;                            //ȡ��Ƭѡ
}
//SPI_FLASHд��ֹ
//��WEL����
void spi2_flash_write_disable(void)
{
    SPI2_FLASH_CS = 0;                              //ʹ������
    spi2_read_write_byte(W25X_WriteDisable);        //����д��ָֹ��
    SPI2_FLASH_CS = 1;                              //ȡ��Ƭѡ
}
//��ȡоƬID W25X16��ID:0XEF14
u16 spi2_flash_read_id(void)
{
    u16 temp = 0;
    SPI2_FLASH_CS = 0;
    spi2_read_write_byte(0x90);//���Ͷ�ȡID����
    spi2_read_write_byte(0x00);
    spi2_read_write_byte(0x00);
    spi2_read_write_byte(0x00);
    temp |= spi2_read_write_byte(0xFF) << 8;
    temp |= spi2_read_write_byte(0xFF);
    SPI2_FLASH_CS = 1;
    return temp;
}
//��ȡSPI FLASH
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void spi2_flash_read(u8* buffer,u32 read_addr,u16 num_byte_to_read)
{
    u16 i;
    SPI2_FLASH_CS=0;                            //ʹ������
    spi2_read_write_byte(W25X_ReadData);         //���Ͷ�ȡ����
    spi2_read_write_byte((u8)((read_addr) >> 16));  //����24bit��ַ
    spi2_read_write_byte((u8)((read_addr) >> 8));
    spi2_read_write_byte((u8)read_addr);
    for(i = 0; i < num_byte_to_read; i++)
    {
        buffer[i] = spi2_read_write_byte(0XFF);   //ѭ������
    }
    SPI2_FLASH_CS = 1;                            //ȡ��Ƭѡ
}
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
void spi2_flash_write_page(u8* buffer,u32 write_addr,u16 num_to_write)
{
    u16 i;
    spi2_flash_write_enable();                  //SET WEL
    SPI2_FLASH_CS = 0;                            //ʹ������
    spi2_read_write_byte(W25X_PageProgram);      //����дҳ����
    spi2_read_write_byte((u8)((write_addr) >> 16)); //����24bit��ַ
    spi2_read_write_byte((u8)((write_addr) >> 8));
    spi2_read_write_byte((u8)write_addr);
    for(i = 0; i < num_to_write; i++)spi2_read_write_byte(buffer[i]);//ѭ��д��
    SPI2_FLASH_CS = 1;                            //ȡ��Ƭѡ
    spi2_flash_wait_busy();                 //�ȴ�д�����
}
//�޼���дSPI FLASH
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ����
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void spi2_flash_write_no_check(u8* buffer,u32 write_addr,u16 num_to_write)
{
    u16 pageremain;
    pageremain = 256 - write_addr % 256; //��ҳʣ����ֽ���
    if(num_to_write <= pageremain)pageremain = num_to_write;//������256���ֽ�
    while(1)
    {
        spi2_flash_write_page(buffer, write_addr, pageremain);
        if(num_to_write == pageremain)break;//д�������
        else //NumByteToWrite>pageremain
        {
            buffer += pageremain;
            write_addr += pageremain;

            num_to_write -= pageremain;             //��ȥ�Ѿ�д���˵��ֽ���
            if(num_to_write > 256)pageremain = 256; //һ�ο���д��256���ֽ�
            else pageremain = num_to_write;         //����256���ֽ���
        }
    };
}
//дSPI FLASH
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
u8 spi2_flash_buf[4096];
void spi2_flash_write(u8* buffer,u32 write_addr,u16 num_to_write)
{
    u32 secpos;
    u16 secoff;
    u16 secremain;
    u16 i;

    secpos=write_addr / 4096;//������ַ 0~511 for w25x16
    secoff=write_addr % 4096;//�������ڵ�ƫ��
    secremain = 4096 - secoff;//����ʣ��ռ��С

    if(num_to_write <= secremain)
    {
        secremain = num_to_write;//������4096���ֽ�
    }
    while(1)
    {
        spi2_flash_read(spi2_flash_buf,secpos*4096,4096);//������������������
        for(i=0;i<secremain;i++)//У������
        {
            if(spi2_flash_buf[secoff+i] != 0XFF)break;//��Ҫ����
        }
        if(i<secremain)//��Ҫ����
        {
            spi2_flash_erase_sector(secpos);//�����������
            for(i=0;i<secremain;i++)        //����
            {
                spi2_flash_buf[i+secoff]=buffer[i];
            }
            spi2_flash_write_no_check(spi2_flash_buf,secpos*4096,4096);//д����������

        }else spi2_flash_write_no_check(buffer, write_addr, secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.
        if(num_to_write == secremain)break;//д�������
        else//д��δ����
        {
            secpos++;//������ַ��1
            secoff=0;//ƫ��λ��Ϊ0

            buffer += secremain;  //ָ��ƫ��
            write_addr += secremain;//д��ַƫ��
            num_to_write -= secremain;                  //�ֽ����ݼ�
            if(num_to_write > 4096)secremain = 4096;    //��һ����������д����
            else secremain = num_to_write;              //��һ����������д����
        }
    };
}
//��������оƬ
//��Ƭ����ʱ��:
//W25X16:25s
//W25X32:40s
//W25X64:40s
//�ȴ�ʱ�䳬��...
void spi2_flash_erase_chip(void)
{
    spi2_flash_write_enable();                  //SET WEL
    spi2_flash_wait_busy();
    SPI2_FLASH_CS = 0;                            //ʹ������
    spi2_read_write_byte(W25X_ChipErase);        //����Ƭ��������
    SPI2_FLASH_CS = 1;                            //ȡ��Ƭѡ
    spi2_flash_wait_busy();                     //�ȴ�оƬ��������
}
//����һ������
//Dst_Addr:������ַ 0~511 for w25x16
//����һ��ɽ��������ʱ��:150ms
void spi2_flash_erase_sector(u32 dst_addr)
{
    dst_addr *= 4096;
    spi2_flash_write_enable();                  //SET WEL
    spi2_flash_wait_busy();
    SPI2_FLASH_CS = 0;                            //ʹ������
    spi2_read_write_byte(W25X_SectorErase);      //������������ָ��
    spi2_read_write_byte((u8)((dst_addr) >> 16));  //����24bit��ַ
    spi2_read_write_byte((u8)((dst_addr) >> 8));
    spi2_read_write_byte((u8)dst_addr);
    SPI2_FLASH_CS = 1;                            //ȡ��Ƭѡ
    spi2_flash_wait_busy();                         //�ȴ��������
}
//�ȴ�����
void spi2_flash_wait_busy(void)
{
    while ((spi2_flash_read_sr() & 0x01) == 0x01);   // �ȴ�BUSYλ���
}
//�������ģʽ
void spi2_flash_power_dow(void)
{
    SPI2_FLASH_CS=0;                            //ʹ������
    spi2_read_write_byte(W25X_PowerDown);        //���͵�������
    SPI2_FLASH_CS=1;                            //ȡ��Ƭѡ
    delay_us(3);                               //�ȴ�TPD
}
//����
void spi2_flash_wakeup(void)
{
    SPI2_FLASH_CS=0;                            //ʹ������
    spi2_read_write_byte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB
    SPI2_FLASH_CS=1;                            //ȡ��Ƭѡ
    delay_us(3);                               //�ȴ�TRES1
}




