#ifndef __USER_FINGERPRINT_RECORD_H__
#define __USER_FINGERPRINT_RECORD_H__
#include "stm32f10x.h"
#include "time.h"
#include "ucos_ii.h"



#define FP_RECORD_TASK_PRIO                   15
#define FP_RECORD_TASK_STK_SIZE               1024



#define FP_RECORD_START_ADDR    0X08060000      //flash共512K, 预留128K存储指纹开锁信息,512K - 128K = 384K, 384K = 0x60000
#define FP_RECORD_SPACE         64              //一条指纹开锁信息记录预留64个字节

#define IS_FP_RECORD_FLASH_ADDR(ADDRESS)        (((ADDRESS) >= FP_RECORD_START_ADDR) && ((ADDRESS) < 0x080FFFFF))

#define FP_RECORD_HEADER_YES    0x12345678
#define FP_RECORD_HEADER_NO     0xffffffff
#pragma pack(1)
typedef struct
{
    uint16_t id;
    uint8_t result;
    time_t time;
}fp_record_item_t;
#pragma pack()

typedef struct
{
    fp_record_item_t fp_item;
    uint32_t header;
}fp_record_t;


extern OS_STK FP_RECORD_TASK_STK[FP_RECORD_TASK_STK_SIZE];

void write_read_fp_record_task(void *pdata);
#endif

