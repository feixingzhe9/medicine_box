#ifndef __MODULES_DISPLAY_TASK_H__
#define __MODULES_DISPLAY_TASK_H__
#include "stm32f10x.h"
#include "ucos_ii.h"

#define DIS_TEST_TASK_PRIO                      8
#define DIS_TEST_STK_SIZE                       1024
extern OS_STK DIS_TEST_TASK_STK[DIS_TEST_STK_SIZE];


#define DISPLAY_TASK_PRIO                      14
#define DISPLAY_STK_SIZE                       1024
extern OS_STK DISPLAY_TASK_STK[DISPLAY_STK_SIZE];

void display_task(void *pdata);
void dis_test_task(void *pdata);

void display_string(uint16_t start_x, uint16_t start_y, uint8_t* str, uint16_t len, uint8_t resolution, uint16_t color);
#endif
