/*
 *  Author: Kaka Xie
 *  brief: fingerprint protocol process
 */
 

#include "fingerprint_task.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	
#include "led.h"
#

OS_STK FP_UART_COM_SEND_TASK_STK[FP_UART_COM_SEND_TASK_STK_SIZE];
OS_STK FP_UART_COM_RCV_TASK_STK[FP_UART_COM_RCV_TASK_STK_SIZE];
OS_MEM	*fp_rcv_mem_handle;

fp_rcv_buf_t fp_rcv_mem[FP_RCV_BUF_NUM][1];
OS_MEM  *fp_rcv_buf_list;
fp_rcv_buf_t *fp_rcv_buf_head = NULL;
OS_EVENT *fp_uart_data_come_sem;
extern int uart_send(uint8_t *data, uint16_t len);

//OSMemPut
//OS_ENTER_CRITICAL


int fp_rcv_buf_head_init(void)
{
    uint8_t err = 0;
    fp_rcv_buf_head = (fp_rcv_buf_t *)OSMemGet(fp_rcv_mem_handle, &err);
    if(fp_rcv_buf_head != NULL)
    {
        fp_rcv_buf_head->next = NULL;
        fp_rcv_buf_head->rcv_len = 0;
        memset(fp_rcv_buf_head->rcv_buf, 0, RCV_SIZE);
    }
    else
    {
        /*
        todo: err process
        */
        return -1;
    }
    return 0;
}



uint16_t get_used_buf_size(void)
{
    uint16_t cnt = 0;
    fp_rcv_buf_t	*ptr = NULL;
    ptr = fp_rcv_buf_head;
    while(ptr->next != NULL)
    {
        ptr = ptr->next;
        cnt++;
    }
    return cnt;
}

int put_fp_rcv_buf(uint8_t *buf, uint16_t len)
{
    fp_rcv_buf_t *p = NULL;
    fp_rcv_buf_t *node = NULL;
    uint8_t err = 0;
    if(get_used_buf_size() < FP_RCV_BUF_NUM)
    {
        node = (fp_rcv_buf_t *)OSMemGet(fp_rcv_mem_handle, &err);
        if(node == NULL)
        {
            /*
            todo: err process
            */
            return -1;
        }
        node->next = NULL;
        node->rcv_len = len;
        memcpy(node->rcv_buf, buf, len);
    }
    else
    {
        /*
        todo: err process
        */
        return -1;
    }
    p = fp_rcv_buf_head;
    while(p->next != NULL)  //链表头不存储数据
    {
        p = p->next;
    }
    p->next = node;
    return 0;
}

int find_the_buf_in_fp_buf(fp_rcv_buf_t *buf)
{
    return -1;
}

int free_one_fp_rcv_buf(fp_rcv_buf_t *buf)
{
    fp_rcv_buf_t *p = NULL;
    fp_rcv_buf_t *pre_p = NULL;
    p = fp_rcv_buf_head;
    pre_p = fp_rcv_buf_head;
    do
    {
        if(p != buf)
        {
            pre_p = p;
            p = p->next;
        }
        else
        {
            break;
        }
    }
    while(p != NULL);

    if(p == NULL)
    {
        return -1;  //can not find such buf
    }

    OSMemPut(fp_rcv_mem_handle, p);
    if(p->next == NULL)
    {
        pre_p->next = NULL;
    }
    else
    {
        pre_p->next = p->next;
    }
    return 0;
}

fp_rcv_buf_t *get_latest_fp_buf(void)
{
    fp_rcv_buf_t *p = NULL;
    fp_rcv_buf_t *node = NULL;
    p = fp_rcv_buf_head;
    if(p->next != NULL)
    {
        node = p->next;
    }
    return node;
}


static int is_frame_valid(uint8_t *buf, uint16_t len)
{
    int i = 0;
    uint8_t check = 0;
    if(len < 8)
    {
        return -1;
    }
    if(len == 8)
    {
        if((buf[0] == FINGERPRINT_UART_FRAME_HEADER) && (buf[7] == FINGERPRINT_UART_FRAME_TAIL))
        {
            for(i = 1; i < 6; i++)
            {
                check ^= buf[i];
            }
            if(check == buf[6])
            {
                return 0;
            }
        }
    }
    if(len > 8)
    {

    }

    return -1;
}


int fp_uart_frame_proc(fp_rcv_buf_t *node)
{
    uint8_t cmd = 0;
    if(node)
    {
        if(is_frame_valid(node->rcv_buf, node->rcv_len) >= 0)
        {
            if(node->rcv_len == 8)
            {
                cmd = node->rcv_buf[1];
            }
        }
    }
    return -1;
}

void fp_uart_com_send_task(void *pdata)
{
    uint8_t send_buf[8] = {0xf5, 0x09, 0x00, 0x00, 0x00, 0x00, };
    uint8_t head = FINGERPRINT_UART_FRAME_HEADER;
    uint8_t cmd = 0x09;
    uint8_t p1, p2, p3;
    uint8_t res = 0;
    uint8_t check = 0;
    uint8_t tail = FINGERPRINT_UART_FRAME_TAIL;
    p1 = 0;
    p2 = 0;
    p3 = 0;
    check = cmd ^ p1 ^ p2 ^ p3 ^ res;

    send_buf[0] = head;
    send_buf[1] = cmd;
    send_buf[2] = p1;
    send_buf[3] = p2;
    send_buf[4] = p3;
    send_buf[5] = res;
    send_buf[6] = check;
    send_buf[7] = tail;


    fp_rcv_buf_head_init();

    while(1)
    {
        LED0=0;
        uart_send(send_buf, 8);
        delay_ms(500);
        LED0=1;
        delay_ms(500);
    }
}


void fp_uart_com_rcv_task(void *pdata)
{
    uint8_t err = 0;
    fp_rcv_buf_t *fp_rcv_node;
    fp_rcv_buf_t node;
    while(1)
    {
        LED1 = 1;
        OSSemPend(fp_uart_data_come_sem, 0, &err);
        LED1 = 0;
        delay_ms(50);
        OS_ENTER_CRITICAL();
        fp_rcv_node = get_latest_fp_buf();
        memcpy(&node, fp_rcv_node, sizeof(fp_rcv_buf_t));
        free_one_fp_rcv_buf(fp_rcv_node);
        OS_EXIT_CRITICAL();
        fp_uart_frame_proc(&node);
    }
}

