#ifndef __HARDWARE_LC12S_WIRELESS_H__
#define __HARDWARE_LC12S_WIRELESS_H__
#include "stm32f10x.h"
#include "usart.h"
#include "fifo.h"

#define FRAME_HEADER                    0x5A
#define FRAME_FOOTER                    0xA5

typedef struct
{
    uint8_t start_flag;
    uint8_t end_flag;
    uint8_t data_len;
    uint8_t rcv_cnt;
    uint8_t rcv_buf[LC12S_RCV_SIZE];
}com_rcv_opt_t;

extern fifo_t *wireless_uart_fifo;

void lc12s_init(void);
int lc12s_com_send(uint8_t *data, uint16_t len);

#endif
