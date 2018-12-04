#ifndef __MODULE_LOCK_H__
#define __MODULE_LOCK_H__
#include "sys.h"

void init_lock(void);
void lock_on(void);
void lock_off(void);
#endif
