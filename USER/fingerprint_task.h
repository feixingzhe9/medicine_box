#ifndef __USER_FINGERPRINT__
#define __USER_FINGERPRINT__
#include "stm32f10x.h"
#include "ucos_ii.h"
#include "usart.h"

#define FINGERPRINT_TEST_TASK_PRIO              9
#define FINGERPRINT_TEST_STK_SIZE               1024
extern OS_STK FINGERPRINT_TEST_TASK_STK[FINGERPRINT_TEST_STK_SIZE];

typedef struct
{
    uint8_t rcv_buf[RCV_SIZE];
    uint16_t rcv_len;
    struct fp_rcv_buf_t *next;
}fp_rcv_buf_t;


extern fp_rcv_buf_t fp_rcv_mem[10][1];
extern OS_MEM  *fp_rcv_buf_list;

void fingerprint_test_task(void *pdata);

#endif
