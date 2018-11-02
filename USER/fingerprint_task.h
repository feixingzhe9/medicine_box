#ifndef __USER_FINGERPRINT__
#define __USER_FINGERPRINT__
#include "stm32f10x.h"
#include "ucos_ii.h"
#include "usart.h"
#include "sys.h"

#define FP_UART_COM_SEND_TASK_PRIO                      9
#define FP_UART_COM_SEND_TASK_STK_SIZE                  1024
extern OS_STK FP_UART_COM_SEND_TASK_STK[FP_UART_COM_SEND_TASK_STK_SIZE];


#define FP_UART_COM_RCV_TASK_PRIO                       10
#define FP_UART_COM_RCV_TASK_STK_SIZE                   1024
extern OS_STK FP_UART_COM_RCV_TASK_STK[FP_UART_COM_RCV_TASK_STK_SIZE];

#define FINGERPRINT_UART_FRAME_HEADER   0xf5
#define FINGERPRINT_UART_FRAME_TAIL     0xf5

#define FP_RCV_BUF_NUM  10
typedef struct fp_rcv_buf_t
{
    uint8_t rcv_buf[RCV_SIZE];
    uint16_t rcv_len;
    struct fp_rcv_buf_t *next;
}fp_rcv_buf_t;


extern fp_rcv_buf_t fp_rcv_mem[10][1];
extern OS_MEM	*fp_rcv_mem_handle;
extern OS_EVENT *fp_uart_data_come_sem;

void fp_uart_com_send_task(void *pdata);
void fp_uart_com_rcv_task(void *pdata);

int put_fp_rcv_buf(uint8_t *buf, uint16_t len);
int free_one_fp_rcv_buf(fp_rcv_buf_t *buf);

#endif
