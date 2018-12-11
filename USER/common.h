#ifndef __USER_OS_COMMON_H__
#define __USER_OS_COMMON_H__
#include "stm32f10x.h"
#include "ucos_ii.h"

#include "fingerprint_task.h"
#include "led_task.h"
#include "display_task.h"
#include "lc12s_wireless_task.h"
#include "lock_task.h"

void os_user_config(void);
void user_init_depend_on_os_config(void);
extern OS_MEM *fp_rcv_mem_handle;
#endif

