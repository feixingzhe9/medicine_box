#ifndef __USER_LED_TASK_H__
#define __USER_LED_TASK_H__
#include "stm32f10x.h"
#include "ucos_ii.h"


#define LED1_TASK_PRIO                  7
#define LED1_STK_SIZE                   64
extern OS_STK LED1_TASK_STK[LED1_STK_SIZE];


#define LED2_TASK_PRIO                  6
#define LED2_STK_SIZE                   64
extern OS_STK LED2_TASK_STK[LED2_STK_SIZE];

void led1_task(void *pdata);
void led2_task(void *pdata);

#endif
