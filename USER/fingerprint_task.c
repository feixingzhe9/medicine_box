/*
 *  Author: Kaka Xie
 *  brief: fingerprint protocol process
 */
 

#include "fingerprint_task.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	
#include "led.h"

OS_STK FINGERPRINT_TEST_TASK_STK[FINGERPRINT_TEST_STK_SIZE];

fp_rcv_buf_t fp_rcv_mem[10][1];
OS_MEM  *fp_rcv_buf_list;

extern int uart_send(uint8_t *data, uint16_t len);
void fingerprint_test_task(void *pdata)
{
    uint8_t send_buf[8] = {0xf5, 0x09, 0x00, 0x00, 0x00, 0x00, };
    uint8_t head = 0xf5;
    uint8_t cmd = 0x09;
    uint8_t p1, p2, p3;
    uint8_t res = 0;
    uint8_t check = 0;
    uint8_t tail = 0xf5;
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

    while(1)
    {
        LED0=0;
        uart_send(send_buf, 8);
        delay_ms(500);
        LED0=1;
        delay_ms(500);
    }
}

