#ifndef __USER_LOCK_TASK__
#define __USER_LOCK_TASK__
#include "stm32f10x.h"
#include "ucos_ii.h"

#define LOCK_TASK_PRIO                   12
#define LOCK_TASK_STK_SIZE               1024

extern OS_STK LOCK_TASK_STK[LOCK_TASK_STK_SIZE];

extern OS_EVENT *unlock_sem;

void lock_task(void *pdata);
void start_to_unlock(void);

#endif
