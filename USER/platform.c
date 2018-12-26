/*
 *  Author: Kaka Xie
 *  brief: platform
 */

#include "platform.h"
#include "stdio.h"

uint32_t get_tick(void)
{
    return OSTimeGet();
}


void int_to_str(uint32_t num, char * str)
{
    sprintf(str, "%d", num);
}
