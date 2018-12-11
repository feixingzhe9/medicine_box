/*
 *  Author: Kaka Xie
 *  brief: lc12s wireless module hardware operation
 */

#include "lc12s_wireless.h"


fifo_data_struct wireless_uart_fifo_data_in_ram[RCV_DATA_LEN_MAX];
fifo_t wireless_uart_fifo_in_ram;
fifo_t *wireless_uart_fifo = &wireless_uart_fifo_in_ram;

void lc12s_com_init(void)
{
    uart1_dma_init(9600);
}

void lc12s_fifo_init(void)
{
    init_fifo(wireless_uart_fifo, wireless_uart_fifo_data_in_ram, RCV_DATA_LEN_MAX);
}

void lc12s_gpio_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);   //使能端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;               // 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //IO口速度为50MHz
    GPIO_Init(GPIOD, &GPIO_InitStructure);                  //根据设定参数初始化GPIO
    GPIO_ResetBits(GPIOD,GPIO_Pin_3);

}

void lc12s_init(void)
{
    lc12s_gpio_init();
    lc12s_com_init();
    lc12s_fifo_init();
}

int lc12s_com_send(uint8_t *data, uint16_t len)
{
    return uart1_send(data, len);
}


void lc12s_send_com_test(void)
{
    uint8_t test_buf[100] = {0};
    int i = 0;
    for(i = 0; i < 100; i++)
    {
        test_buf[i] = i;
    }
    //lc12s_com_send("abcdefghijklmn", 10);
    lc12s_com_send(test_buf, sizeof(test_buf));

}
