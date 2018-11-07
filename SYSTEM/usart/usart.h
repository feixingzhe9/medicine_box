#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define SEND_SIZE   255
#define RCV_SIZE    255

extern uint8_t uart_send_buf[SEND_SIZE];
extern uint8_t fp_uart_rcv_buf[RCV_SIZE];

void uart_init(u32 bound);
#endif

