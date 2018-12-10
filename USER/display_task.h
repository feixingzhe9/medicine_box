#ifndef __MODULES_DISPLAY_TASK_H__
#define __MODULES_DISPLAY_TASK_H__
#include "stm32f10x.h"
#include "ucos_ii.h"

typedef struct
{
    uint16_t start_x, start_y;
    uint8_t str[100];
    uint16_t str_len;
    uint8_t resolution;
    uint16_t str_color;
    uint8_t need_rectangle_flag;
    uint16_t rec_color;
    uint8_t need_update_flag;
    uint16_t period_ms;
    uint8_t layer;
}show_content_t;

#define DISPLAY_CONTENT_NUM_MAX     50
typedef struct
{
    uint8_t need_update_flag;
    uint16_t content_len;
    show_content_t content[DISPLAY_CONTENT_NUM_MAX];
}display_info_t;


#define DIS_TEST_TASK_PRIO                      8
#define DIS_TEST_STK_SIZE                       1024
extern OS_STK DIS_TEST_TASK_STK[DIS_TEST_STK_SIZE];


#define DISPLAY_TASK_PRIO                      14
#define DISPLAY_STK_SIZE                       1024
extern OS_STK DISPLAY_TASK_STK[DISPLAY_STK_SIZE];

void display_task(void *pdata);
void dis_test_task(void *pdata);

void display_string(uint16_t start_x, uint16_t start_y, uint8_t* str, uint16_t len, uint8_t resolution, uint16_t color);
void display_add_one_content(show_content_t content);
#endif
