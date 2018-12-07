/*
 *  Author: Kaka Xie
 *  brief: lock task
 */

#include "lock_task.h"
#include "delay.h"
#include "lock.h"

OS_STK LOCK_TASK_STK[LOCK_TASK_STK_SIZE];

OS_EVENT *unlock_sem;

void start_to_unlock(void)
{
    OSSemPost(unlock_sem);
}

void unlock(void)
{
    lock_on();
    delay_ms(100);
    lock_off();
}

void lock_task(void *pdata)
{
    uint8_t err = 0;
    while(1)
    {
        OSSemPend(unlock_sem, 0, &err);
        unlock();
        delay_ms(500);
    }
}
