/*
 *  Author: Kaka Xie
 *  brief: usart operation
 */

#include "sys.h"
#include "usart.h"

#if SYSTEM_SUPPORT_UCOS
#include "includes.h"           //ucos ʹ��
#endif


#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
    int handle;

};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
    x = x;
}
//�ض���fputc����
int fputc(int ch, FILE *f)
{
    //while((USART1->SR&0X40)==0);//ѭ������,ֱ���������
    //USART1->DR = (u8) ch;
    return ch;
}
#endif


uint8_t fp_uart_send_buf[FP_SEND_SIZE];
uint8_t fp_uart_rcv_buf[FP_RCV_SIZE];
void uart2_dma_init(u32 bound)
{
    USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    GPIO_InitTypeDef	GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    }

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);



    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);

    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);  //
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
    //USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    //DMA configuration,  USART2 TX
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);    // �����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&fp_uart_send_buf[0];    // �ڴ��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                  // �ڴ� to ����
    DMA_InitStructure.DMA_BufferSize = FP_SEND_SIZE;             // �����С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    // �����ַ������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;             // �ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // ���ݿ��8λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // ���ݿ��
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                     // ѭ������ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                 // DMA���ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                        // MEM��MEM�����ֹ
    DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);                     // ����DMA�������ж�
    DMA_Init(DMA1_Channel7, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel7, DISABLE);

    //USART2 RX
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);    // �����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&fp_uart_rcv_buf[0];    // �ڴ��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                  // �ڴ� to ����
    DMA_InitStructure.DMA_BufferSize = FP_RCV_SIZE;             // �����С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    // �����ַ������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;             // �ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // ���ݿ��8λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // �ڴ����ݿ��
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                     // ѭ������ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                 // DMA���ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                        // MEM��MEM�����ֹ
    //DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);                     // ����DMA�������ж�
    DMA_Init(DMA1_Channel6, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel6, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


}


int uart2_send(uint8_t *data, uint16_t len)
{
    memcpy(fp_uart_send_buf, data, len);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    DMA1_Channel7->CNDTR = len;
    DMA_Cmd(DMA1_Channel7, ENABLE);

    return 0;
}




uint8_t lc12s_send_buf[LC12S_SEND_SIZE];
uint8_t lc12s_uart_rcv_buf[LC12S_RCV_SIZE];
void uart1_dma_init(u32 bound)
{
    USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    GPIO_InitTypeDef	GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    }

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);



    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);  //
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    //DMA configuration,  COM1-TX
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);    // �����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&lc12s_send_buf[0];    // �ڴ��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                  // �ڴ� to ����
    DMA_InitStructure.DMA_BufferSize = LC12S_SEND_SIZE;             // �����С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    // �����ַ������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;             // �ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // ���ݿ��8λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // �ڴ����ݿ��
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                     // ѭ������ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                 // DMA���ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                        // MEM��MEM�����ֹ
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);                     // ����DMA�������ж�
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel4, DISABLE);

    //COM1-RX
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);    // �����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&lc12s_uart_rcv_buf[0];    // �ڴ��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                  // �ڴ� to ����
    DMA_InitStructure.DMA_BufferSize = LC12S_RCV_SIZE;             // �����С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    // �����ַ������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;             // �ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // ���ݿ��8λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // ���ݿ��
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                     // ѭ������ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                 // DMA���ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                        // MEM��MEM�����ֹ
    //DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);                     // ����DMA�������ж�
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel5, ENABLE);


    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


}


int uart1_send(uint8_t *data, uint16_t len)
{
    memcpy(lc12s_send_buf, data, len);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    DMA1_Channel4->CNDTR = len;
    DMA_Cmd(DMA1_Channel4, ENABLE);

    return 0;
}


