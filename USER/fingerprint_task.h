#ifndef __USER_FINGERPRINT__
#define __USER_FINGERPRINT__
#include "stm32f10x.h"
#include "ucos_ii.h"
#include "usart.h"
#include "fingerprint.h"
#include "sys.h"
#include <string.h>

#define FP_UART_COM_SEND_TASK_PRIO                      9
#define FP_UART_COM_SEND_TASK_STK_SIZE                  1024
extern OS_STK FP_UART_COM_SEND_TASK_STK[FP_UART_COM_SEND_TASK_STK_SIZE];

#define FP_UART_COM_RCV_TASK_PRIO                       10
#define FP_UART_COM_RCV_TASK_STK_SIZE                   1024
extern OS_STK FP_UART_COM_RCV_TASK_STK[FP_UART_COM_RCV_TASK_STK_SIZE];

extern fp_rcv_buf_t fp_rcv_mem[FP_RCV_BUF_NUM][1];
extern fp_short_ack_t fp_short_ack_mem[FP_SHORT_ACK_NUM][1];

/*os related*/
extern OS_MEM   *fp_rcv_mem_handle;
extern OS_MEM   *fp_short_ack_mem_handle;
extern OS_EVENT *fp_uart_data_come_sem;
extern OS_EVENT *fp_com_get_feature_sem;
extern OS_EVENT *fp_com_read_feature_sem;
extern OS_EVENT *fp_com_set_feature_sem;

#define FP_SHORT_ACK_QUEUE_NUM  FP_SHORT_ACK_NUM
#define FP_LONG_ACK_QUEUE_NUM   FP_LONG_ACK_NUM
extern OS_EVENT *fp_short_ack_queue_handle;
extern void* fp_short_ack_queue_p[FP_SHORT_ACK_QUEUE_NUM];

extern OS_EVENT *fp_long_ack_queue_handle;
extern void* fp_long_ack_queue_p[FP_SHORT_ACK_QUEUE_NUM];

extern OS_MEM   *fp_long_ack_mem_handle;
extern fp_long_ack_t fp_long_ack_mem[FP_LONG_ACK_NUM][1];

void fp_uart_com_send_task(void *pdata);
void fp_uart_com_rcv_task(void *pdata);

int put_fp_rcv_buf(uint8_t *buf, uint16_t len);
int free_one_fp_rcv_buf(fp_rcv_buf_t *buf);
int fp_rcv_buf_head_init(void);


uint8_t add_fp_by_press(uint16_t id, fp_permission_e permission);
uint8_t fp_del_all_fp(void);

#endif
