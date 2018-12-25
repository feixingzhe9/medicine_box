/*
 *  Author: Kaka Xie
 *  brief: lc12s wireless module task process
 */

#include "lc12s_wireless_task.h"
#include "delay.h"
#include "lc12s_wireless.h"

OS_STK LC12S_UART_COM_SEND_TASK_STK[LC12S_UART_COM_SEND_TASK_STK_SIZE];
OS_STK LC12S_UART_COM_RCV_TASK_STK[LC12S_UART_COM_RCV_TASK_STK_SIZE];

OS_EVENT *wireless_com_data_come_sem;


uint32_t my_id = 0x12345678;
uint32_t rcv_id = 0;

uint8_t cal_check_sum(uint8_t *data, uint8_t len)
{
    uint8_t i = 0;
    uint8_t sum = 0;
    for(i = 0; i < len; i++)
    {
        sum += data[i];
    }
    return sum;
}


extern void lc12s_send_com_test(void);

static void send_heart_beat(uint32_t cnt)
{
    uint8_t send_buf[14] = {0};
    send_buf[0] = FRAME_HEADER;
    send_buf[1] = 14;
    send_buf[2] = FRAME_CLASS_COMMON;
    send_buf[3] = FRAME_HEART_BEAT;
    send_buf[4] = my_id >> 24;
    send_buf[5] = (my_id >> 16) & 0xff;
    send_buf[6] = (my_id >> 8) & 0xff;
    send_buf[7] = my_id & 0xff;
    send_buf[8] = (uint8_t)(cnt >> 24);
    send_buf[9] = (uint8_t)((cnt >> 16) & 0xff);
    send_buf[10] = (uint8_t)((cnt >> 8) & 0xff);
    send_buf[11] = (uint8_t)(cnt  & 0xff);
    send_buf[12] = cal_check_sum(send_buf, 12);
    send_buf[13] = FRAME_FOOTER;
    lc12s_com_send(send_buf, 14);
}

static void fp_ack_del_all_user(uint8_t status, uint32_t serial_num)
{
    uint8_t send_buf[15] = {0};
    send_buf[0] = FRAME_HEADER;
    send_buf[1] = 15;
    send_buf[2] = FRAME_CLASS_FP;
    send_buf[3] = FRAME_FP_DEL_ALL_USER;
    send_buf[4] = my_id >> 24;
    send_buf[5] = (my_id >> 16) & 0xff;
    send_buf[6] = (my_id >> 8) & 0xff;
    send_buf[7] = my_id & 0xff;
    send_buf[8] = (uint8_t)(serial_num >> 24);
    send_buf[9] = (uint8_t)((serial_num >> 16) & 0xff);
    send_buf[10] = (uint8_t)((serial_num >> 8) & 0xff);
    send_buf[11] = (uint8_t)(serial_num  & 0xff);
    send_buf[12] = status;
    send_buf[13] = cal_check_sum(send_buf, 13);
    send_buf[14] = FRAME_FOOTER;
    lc12s_com_send(send_buf, 15);
}



static void display_ack_show_content(uint8_t status, uint32_t serial_num)
{
    uint8_t send_buf[15] = {0};
    send_buf[0] = FRAME_HEADER;
    send_buf[1] = 15;
    send_buf[2] = FRAME_CLASS_DISPLAY;
    send_buf[3] = FRAME_DISPLAY_SHOW_CONTENT;
    send_buf[4] = my_id >> 24;
    send_buf[5] = (my_id >> 16) & 0xff;
    send_buf[6] = (my_id >> 8) & 0xff;
    send_buf[7] = my_id & 0xff;
    send_buf[8] = (uint8_t)(serial_num >> 24);
    send_buf[9] = (uint8_t)((serial_num >> 16) & 0xff);
    send_buf[10] = (uint8_t)((serial_num >> 8) & 0xff);
    send_buf[11] = (uint8_t)(serial_num  & 0xff);
    send_buf[12] = status;
    send_buf[13] = cal_check_sum(send_buf, 13);
    send_buf[14] = FRAME_FOOTER;
    lc12s_com_send(send_buf, 15);
}


static void fp_ack_add_fp_by_press(uint8_t status, uint32_t serial_num)
{
    uint8_t send_buf[15] = {0};
    uint8_t len = 15;
    send_buf[0] = FRAME_HEADER;
    send_buf[1] = 15;
    send_buf[2] = FRAME_CLASS_FP;
    send_buf[3] = FRAME_FP_ADD_USER_BY_PRESSING;
    send_buf[4] = my_id >> 24;
    send_buf[5] = (my_id >> 16) & 0xff;
    send_buf[6] = (my_id >> 8) & 0xff;
    send_buf[7] = my_id & 0xff;
    send_buf[8] = (uint8_t)(serial_num >> 24);
    send_buf[9] = (uint8_t)((serial_num >> 16) & 0xff);
    send_buf[10] = (uint8_t)((serial_num >> 8) & 0xff);
    send_buf[11] = (uint8_t)(serial_num  & 0xff);
    send_buf[12] = status;
    send_buf[13] = cal_check_sum(send_buf, len - 2);
    send_buf[14] = FRAME_FOOTER;
    lc12s_com_send(send_buf, 15);
}


static void fp_ack_unlock(uint32_t serial_num)
{
    uint8_t send_buf[14] = {0};
    uint8_t len = 14;
    send_buf[0] = FRAME_HEADER;
    send_buf[1] = len;
    send_buf[2] = FRAME_CLASS_COMMON;
    send_buf[3] = FRAME_COMMON_UNLOCK;
    send_buf[4] = my_id >> 24;
    send_buf[5] = (my_id >> 16) & 0xff;
    send_buf[6] = (my_id >> 8) & 0xff;
    send_buf[7] = my_id & 0xff;
    send_buf[8] = (uint8_t)(serial_num >> 24);
    send_buf[9] = (uint8_t)((serial_num >> 16) & 0xff);
    send_buf[10] = (uint8_t)((serial_num >> 8) & 0xff);
    send_buf[11] = (uint8_t)(serial_num  & 0xff);
    send_buf[12] = cal_check_sum(send_buf, len - 2);
    send_buf[13] = FRAME_FOOTER;
    lc12s_com_send(send_buf, len);
}


static void send_test(void)
{
    lc12s_com_send("123456789", 9);
}

void lc12s_send_task(void *pdata)
{
    uint32_t cnt = 0;
    while(1)
    {
//        send_heart_beat(cnt++);
//        send_test();
        delay_ms(1000);
    }
}


uint8_t check_frame_sum(uint8_t *data, uint8_t data_len)
{
    uint8_t sum = 0;
    uint8_t i = 0;
    for( i = 0; i < data_len - 1; i++)
    {
        sum += data[i];
    }
    return (sum == data[data_len - 1]);

}

uint32_t heart_beat_cnt = 0;
uint32_t heart_beat_cnt_2 = 0;
static int frame_proc(uint8_t *frame, uint16_t len)
{
    uint8_t frame_class = frame[0];
    uint8_t frame_type = frame[1];
    volatile uint32_t get_serial_num = (frame[6] << 24) | (frame[7] << 16) | (frame[8] << 8) | frame[9];
    static uint32_t serial_num = 0;
    uint32_t get_mcu_id = (frame[2] << 24) | (frame[3] << 16) | (frame[4] << 8) | frame[5];;
    if(len > LC12S_RCV_SIZE - 4)
    {
        return -2;  //parameter error
    }

    switch(frame_class)
    {
        case FRAME_CLASS_COMMON:
        {
            switch(frame_type)
            {
                case FRAME_HEART_BEAT:
                    heart_beat_cnt_2++;
                    rcv_id = frame[4];
                    rcv_id |= frame[3] << 8;
                    rcv_id |= frame[2] << 16;
                    rcv_id |= frame[1] << 24;

                    heart_beat_cnt = frame[8];
                    heart_beat_cnt |= frame[7] << 8;
                    heart_beat_cnt |= frame[6] << 16;
                    heart_beat_cnt |= frame[5] << 24;
                    break;

                case FRAME_COMMON_UNLOCK:
                    if(get_serial_num != serial_num)
                    {
                        serial_num = get_serial_num;
                        start_to_unlock();
                    }
                    fp_ack_unlock(get_serial_num);
                    break;

                default :
                    break;
            }
            break;
        }

        case FRAME_CLASS_FP:
        {
            switch(frame_type)
            {
                case FRAME_FP_DEL_ALL_USER:
                {

                    if(get_mcu_id == my_id)
                    {
                        if(get_serial_num != serial_num)
                        {
                            serial_num = get_serial_num;
                            /*
                            todo: add delete all user function
                            */
                            fp_del_all_fp();
                        }
                        fp_ack_del_all_user(1, get_serial_num);
                        break;
                    }
                }

                case FRAME_FP_ADD_USER_BY_PRESSING:
                {
                    uint16_t id = frame[10] << 8 | frame[11];
                    uint8_t permission = frame[12];
                    uint8_t status = 0;
                    if(get_mcu_id == my_id)
                    {
                        if(get_serial_num != serial_num)
                        {
                            serial_num = get_serial_num;
                            status = add_fp_by_press(id, (fp_permission_e)permission);
                        }
                        fp_ack_add_fp_by_press(status, get_serial_num);
                        printf("hehe");
                        break;
                    }
                }
            }
            break;
        }

        case FRAME_CLASS_DISPLAY:
        {
            switch(frame_type)
            {
                case FRAME_DISPLAY_SHOW_CONTENT:
                {
                    show_content_t content = {0};
                    uint8_t str[480 / 8] = {0};
                    uint8_t str_len = len - 22;
                    uint16_t start_x = (frame[10] << 8) | frame[11];
                    uint16_t start_y = (frame[12] << 8) | frame[13];
                    char_resolution_high_e resulotion = (char_resolution_high_e)frame[18];
                    uint16_t color = (frame[16] << 8) | frame[17];
                    uint8_t layer = frame[19];
                    uint16_t period = (frame[20] << 8) | frame[21];
                    uint16_t i;
                    if(get_mcu_id == my_id)
                    {
                        for(i = 0; i < str_len; i++)
                        {
                            str[i] = frame[22 + i];
                        }
                        if(get_serial_num != serial_num)
                        {
                            serial_num = get_serial_num;
//                            display_string(start_x, start_y, str, str_len, resulotion, color);
                            memcpy(content.str, str, str_len);
                            content.start_x = start_x;
                            content.start_y = start_y;
                            content.need_update_flag = 1;
                            content.str_len = str_len;
                            content.resolution = resulotion;
                            content.str_color = color;
                            content.period_ms = period * 10;
#if DISPLAY_FUNCTION_1
                            display_add_one_content(content);
#endif
                        }
                        display_ack_show_content(1, serial_num);

                        break;
                    }
                }
            }
            break;
        }
    }

    return -1;
}

com_rcv_opt_t wireless_rcv_com_opt = {0};

void lc12s_rcv_task(void *pdata)
{
    uint8_t err = 0;
    while(1)
    {
        fifo_data_struct data_tmp;

//        OSSemPend(wireless_com_data_come_sem, 0, &err);
//        OSSemSet(wireless_com_data_come_sem, 0, &err);

        while(is_fifo_empty(wireless_uart_fifo) == FALSE)
        {
            get_data_from_fifo(wireless_uart_fifo, &data_tmp);
            wireless_rcv_com_opt.rcv_buf[wireless_rcv_com_opt.rcv_cnt] = data_tmp;
            if(wireless_rcv_com_opt.start_flag == TRUE)
            {
                if(wireless_rcv_com_opt.rcv_cnt == 1)
                {
                    wireless_rcv_com_opt.data_len = data_tmp;
                }
                if(wireless_rcv_com_opt.rcv_cnt == wireless_rcv_com_opt.data_len - 1)
                {
                    if(wireless_rcv_com_opt.rcv_buf[wireless_rcv_com_opt.rcv_cnt] == FRAME_FOOTER)
                    {
//                        uint8_t ctype = wireless_rcv_com_opt.rcv_buf[2];
                        wireless_rcv_com_opt.end_flag = TRUE;
                        wireless_rcv_com_opt.start_flag = FALSE;
                        wireless_rcv_com_opt.rcv_cnt = 0;
                        if(check_frame_sum(wireless_rcv_com_opt.rcv_buf, wireless_rcv_com_opt.data_len - 1))
                        {
                            frame_proc(&wireless_rcv_com_opt.rcv_buf[2], wireless_rcv_com_opt.data_len - 4);
                        }

                        //printf("o\n");
                        break;
                    }
                    else
                    {
                        wireless_rcv_com_opt.end_flag = FALSE;
                        wireless_rcv_com_opt.start_flag = FALSE;
                        wireless_rcv_com_opt.rcv_cnt = 0;
                    }
                }
            }
            else
            {
                if(data_tmp == FRAME_HEADER)
                {
                    wireless_rcv_com_opt.start_flag = TRUE;
                    wireless_rcv_com_opt.end_flag = FALSE;
                }
                wireless_rcv_com_opt.rcv_cnt = 0;
            }

            if(wireless_rcv_com_opt.rcv_cnt++ >= sizeof(wireless_rcv_com_opt.rcv_buf) - 1)
            {
                wireless_rcv_com_opt.start_flag = FALSE;
                wireless_rcv_com_opt.end_flag = FALSE;
                wireless_rcv_com_opt.rcv_cnt = 0;
            }
        }

        delay_ms(50);
    }
}

