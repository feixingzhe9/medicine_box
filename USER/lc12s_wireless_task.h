#ifndef __USER_LC12S_WIRELESS_TASK_H__
#define __USER_LC12S_WIRELESS_TASK_H__
#include "stm32f10x.h"
#include "ucos_ii.h"

typedef enum
{
    FRAME_HEART_BEAT = 1,
    FRAME_MAX
}wireless_frame_type_e;

#define LC12S_UART_COM_SEND_TASK_PRIO                   5
#define LC12S_UART_COM_SEND_TASK_STK_SIZE               1024

#define LC12S_UART_COM_RCV_TASK_PRIO                    4
#define LC12S_UART_COM_RCV_TASK_STK_SIZE                1024


extern OS_STK LC12S_UART_COM_SEND_TASK_STK[LC12S_UART_COM_SEND_TASK_STK_SIZE];
extern OS_STK LC12S_UART_COM_RCV_TASK_STK[LC12S_UART_COM_RCV_TASK_STK_SIZE];

extern OS_EVENT *wireless_com_data_come_sem;

void lc12s_send_task(void *pdata);
void lc12s_rcv_task(void *pdata);
#endif
