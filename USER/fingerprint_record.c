/*
 *  Author: Kaka Xie
 *  brief:  read and write fingerprint record
 */

#include "string.h"
#include "stdio.h"
#include "delay.h"
#include "fingerprint_record.h"
#include "mcu_flash.h"

OS_STK FP_RECORD_TASK_STK[FP_RECORD_TASK_STK_SIZE];

uint8_t fp_record_buf[FP_RECORD_SPACE] = {0};

uint16_t read_till_last_record(void)
{
    fp_record_t fp_record = {0};
    uint16_t cnt = 0;
    while(1)
    {
        flash_read(FP_RECORD_START_ADDR + FP_RECORD_SPACE * cnt, (u16*)fp_record_buf, FP_RECORD_SPACE);
        memcpy(&fp_record, fp_record_buf, sizeof(fp_record_t));
        if(fp_record.header == FP_RECORD_HEADER_YES)
        {
            cnt++;
        }
        else if(fp_record.header == FP_RECORD_HEADER_NO)
        {
            return cnt;
        }
        else
        {
            /*
            todo: error status ! !
            */
            printf("read_till_last_record err: fp_record.header = 0x%x !", fp_record.header);
            break;
        }
    }

    return 0;
}

void write_one_record(fp_record_t fp_record)
{
    uint16_t cnt = read_till_last_record();
    memcpy(fp_record_buf, &fp_record, sizeof(fp_record_t));
    flash_write(FP_RECORD_START_ADDR + FP_RECORD_SPACE * cnt, (u16*)&fp_record, FP_RECORD_SPACE);
    return ;
}


void write_read_fp_record_task(void *pdata)
{
    uint8_t init_flag = 0;
    uint16_t cnt = 0;
    fp_record_t fp_record = {0};
    while(1)
    {
        if(init_flag == 0)
        {
            init_flag = 1;
            fp_record.header = FP_RECORD_HEADER_YES;
            fp_record.fp_item.id = 0x5665;
            fp_record.fp_item.time.year = 2018;
            fp_record.fp_item.time.month = 12;
            fp_record.fp_item.time.day = 20;
            fp_record.fp_item.time.weekday = 4;
            fp_record.fp_item.time.hour = 17;
            fp_record.fp_item.time.min = 40;
            fp_record.fp_item.time.sec = 18;
            write_one_record(fp_record);
            cnt = read_till_last_record();
        }
        delay_ms(1000);
    }
}

