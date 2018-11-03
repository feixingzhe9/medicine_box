/*
 *  Author: Kaka Xie
 *  brief: os user configuration
 */

#include "os_common.h"
#include "fingerprint_task.h"

#include "fingerprint_task.h"
#include "led_task.h"
#include "display_task.h"

static void task_create(void)
{
    OSTaskCreate(fp_uart_com_send_task,     (void *)0,  (OS_STK*)&FP_UART_COM_SEND_TASK_STK[FP_UART_COM_SEND_TASK_STK_SIZE - 1],    FP_UART_COM_SEND_TASK_PRIO);
    OSTaskCreate(fp_uart_com_rcv_task,      (void *)0,  (OS_STK*)&FP_UART_COM_RCV_TASK_STK[FP_UART_COM_RCV_TASK_STK_SIZE - 1],      FP_UART_COM_RCV_TASK_PRIO);
    OSTaskCreate(led1_task,                 (void *)0,  (OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],                                   LED1_TASK_PRIO);
    OSTaskCreate(led2_task,                 (void *)0,  (OS_STK*)&LED2_TASK_STK[LED2_STK_SIZE-1],                                   LED2_TASK_PRIO);
    OSTaskCreate(dis_test_task,             (void *)0,  (OS_STK*)&DIS_TEST_TASK_STK[DIS_TEST_STK_SIZE - 1],                         DIS_TEST_TASK_PRIO);
}

static void sem_create(void)
{
    fp_uart_data_come_sem = OSSemCreate(0);
    fp_com_get_feature_sem = OSSemCreate(0);
    fp_com_read_feature_sem = OSSemCreate(0);
    fp_com_set_feature_sem = OSSemCreate(0);
}


static int mem_create(void)
{
    uint8_t err = 0;
    fp_rcv_mem_handle = OSMemCreate((void *)&fp_rcv_mem[0][0], sizeof(fp_rcv_mem) / sizeof(fp_rcv_mem[0]), sizeof(fp_rcv_buf_t), &err);
    if(fp_rcv_mem_handle == 0)
    {
        /*
        todo: err process
        */
        return -1;
    }

    fp_short_ack_mem_handle = OSMemCreate((void *)&fp_short_ack_mem[0][0], sizeof(fp_short_ack_mem) / sizeof(fp_short_ack_mem[0]), sizeof(fp_short_ack_t), &err);
    if(fp_short_ack_mem_handle == NULL)
    {
        /*
        todo: err process
        */
        return -1;
    }


    return 0;
}

static int queue_create(void)
{
    fp_short_ack_queue_handle = OSQCreate(&fp_short_ack_queue_p[0], FP_SHORT_ACK_QUEUE_NUM);
    if(fp_short_ack_queue_handle == NULL)
    {
        /*
        todo: err process
        */
        return -1;
    }

    return 0;
}

void os_user_config(void)
{
    sem_create();
    mem_create();
    queue_create();
    task_create();
}


