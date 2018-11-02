#ifndef __MODULES_DISPLAY_TASK_H__
#define __MODULES_DISPLAY_TASK_H__
#include "stm32f10x.h"
#include "ucos_ii.h"

#define DIS_TEST_TASK_PRIO                      8
#define DIS_TEST_STK_SIZE                       1024
extern OS_STK DIS_TEST_TASK_STK[DIS_TEST_STK_SIZE];

void dis_test_task(void *pdata);
#endif