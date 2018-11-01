#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define SEND_SIZE   1024
#define RCV_SIZE    1024

void uart_init(u32 bound);
#endif

