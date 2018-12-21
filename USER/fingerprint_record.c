/*
 *  Author: Kaka Xie
 *  brief:  read and write fingerprint record
 */

#include "string.h"
#include "stdio.h"
#include "delay.h"
#include "fingerprint_record.h"
#include "mcu_flash.h"
#include "flash.h"

OS_STK FP_RECORD_TASK_STK[FP_RECORD_TASK_STK_SIZE];


#if 0   //mcu FLASH
uint8_t fp_record_buf[FP_RECORD_SPACE] = {0};

void fp_record_flash_read(uint32_t addr, uint16_t *buffer, uint16_t num_to_read)
{
    if(IS_FP_RECORD_FLASH_ADDR(addr))
    {
        flash_read(addr, buffer, num_to_read);
    }
}

void fp_record_flash_write(uint32_t addr, uint16_t *buffer, uint16_t num_to_read)
{
    if(IS_FP_RECORD_FLASH_ADDR(addr))
    {
        flash_write(addr, buffer, num_to_read);
    }
}



uint16_t read_till_last_record(void)
{
    fp_record_t fp_record = {0};
    uint16_t cnt = 0;
    while(1)
    {
        fp_record_flash_read(FP_RECORD_START_ADDR + FP_RECORD_SPACE * cnt, (u16*)fp_record_buf, FP_RECORD_SPACE);
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
//    memset(fp_record_buf, 0, FP_RECORD_SPACE);
    memcpy(fp_record_buf, &fp_record, sizeof(fp_record_t));
    fp_record_flash_write(FP_RECORD_START_ADDR + FP_RECORD_SPACE * cnt, (uint16_t*)fp_record_buf, FP_RECORD_SPACE);
    return ;
}


void fp_record_flash_erase_all(void)
{
    uint16_t record_num;

    record_num = read_till_last_record();
    for(uint16_t i = 0; i < record_num * FP_RECORD_SPACE / STM_SECTOR_SIZE + 1; i++)
    {
        flash_erase(FP_RECORD_START_ADDR + i * STM_SECTOR_SIZE);
    }
}
#else


uint8_t init_fp_flash(void)
{
    uint8_t fp_record_read_buf[64] = {0};
    fp_record_main_index_t main_index;
    spi2_flash_read_data(0, fp_record_read_buf, sizeof(fp_record_read_buf));
    memcpy(&main_index, fp_record_read_buf, sizeof(fp_record_main_index_t));
    if(main_index.init_flag != FP_RECORD_MAIN_INDEX_INIT_FLAG)
    {
        
    }
}



#endif




void write_read_fp_record_task(void *pdata)
{
    uint8_t init_flag = 0;
    uint16_t cnt = 0;
    fp_record_t fp_record = {0};
    uint8_t write_test_data[10] = {1,2,3,4,5,6,7,8,9,10};
    uint8_t read_test_data[200] = {0};
    while(1)
    {
        if(init_flag == 0)
        {
            init_flag = 1;
#if 0

            fp_record.header = FP_RECORD_HEADER_YES;
            fp_record.fp_item.id = 0x5665;
            fp_record.fp_item.time.year = 2018;
            fp_record.fp_item.time.month = 12;
            fp_record.fp_item.time.day = 21;
            fp_record.fp_item.time.weekday = 5;
            fp_record.fp_item.time.hour = 10;
            fp_record.fp_item.time.min = 30;
            fp_record.fp_item.time.sec = 12;
            write_one_record(fp_record);
            cnt = read_till_last_record();
//            fp_record_flash_erase_all();
#endif


            spi2_flash_write_data(16, write_test_data, sizeof(write_test_data));
            delay_ms(20);
            spi2_flash_read_data(0, read_test_data, sizeof(read_test_data));
            spi2_flash_erase_sector(0);
            spi2_flash_read_data(0, read_test_data, sizeof(read_test_data));
        }
        delay_ms(1000);
    }
}

