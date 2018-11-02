#ifndef __USER_FINGERPRINT__
#define __USER_FINGERPRINT__
#include "stm32f10x.h"
#include "ucos_ii.h"
#include "usart.h"
#include "sys.h"

#define FINGERPRINT_TEST_TASK_PRIO              9
#define FINGERPRINT_TEST_STK_SIZE               1024
extern OS_STK FINGERPRINT_TEST_TASK_STK[FINGERPRINT_TEST_STK_SIZE];

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

void fingerprint_test_task(void *pdata);
int put_fp_rcv_buf(uint8_t *buf, uint16_t len);
int free_one_fp_rcv_buf(fp_rcv_buf_t *buf);

#endif
