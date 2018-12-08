#ifndef __USER_LC12S_WIRELESS_TASK_H__
#define __USER_LC12S_WIRELESS_TASK_H__
#include "stm32f10x.h"
#include "ucos_ii.h"

typedef enum
{
    FRAME_CLASS_COMMON = 1,
    FRAME_CLASS_FP,
    FRAME_CLASS_DISPLAY,
    FRAME_CLASS_MAX
}frame_class_e;

typedef enum
{
    FRAME_HEART_BEAT = 1,
    FRAME_COMMON_UNLOCK,
    FRAME_MAX
}frame_common_type_e;

typedef enum
{
    FRAME_FP_DEL_ALL_USER = 1,
    FRAME_FP_ADD_USER_BY_PRESSING,
}frame_fp_type_e;

typedef enum
{
    FRAME_DISPLAY_SHOW_CONTENT = 1,
}frame_display_e;

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
