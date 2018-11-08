/*
 *  Author: Kaka Xie
 *  brief: lc12s wireless module task process
 */
 
#include "lc12s_wireless_task.h"
#include "delay.h"

OS_STK LC12S_UART_COM_SEND_TASK_STK[LC12S_UART_COM_SEND_TASK_STK_SIZE];
OS_STK LC12S_UART_COM_RCV_TASK_STK[LC12S_UART_COM_RCV_TASK_STK_SIZE];


extern void lc12s_send_com_test(void);
void lc12s_send_task(void *pdata)
{
    while(1)
    {
        //lc12s_send_com_test();
        delay_ms(1000);
    }
}

void lc12s_rcv_task(void *pdata)
{
    while(1)
    {
        delay_ms(1000);
    }
}

