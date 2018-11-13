/*
 *  Author: Kaka Xie
 *  brief: fingerprint process
 */


#include "fingerprint_task.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	
#include "led.h"
#include "fingerprint.h"

OS_STK FP_UART_COM_SEND_TASK_STK[FP_UART_COM_SEND_TASK_STK_SIZE];
OS_STK FP_UART_COM_RCV_TASK_STK[FP_UART_COM_RCV_TASK_STK_SIZE];



OS_EVENT *fp_uart_data_come_sem;
OS_EVENT *fp_com_get_feature_sem;
OS_EVENT *fp_com_read_feature_sem;
OS_EVENT *fp_com_set_feature_sem;


fp_rcv_buf_t fp_rcv_mem[FP_RCV_BUF_NUM][1];
fp_rcv_buf_t *fp_rcv_buf_head = NULL;
OS_MEM	*fp_rcv_mem_handle;

//OSQCreate
OS_EVENT	*fp_short_ack_queue_handle;
void* fp_short_ack_queue_p[FP_SHORT_ACK_QUEUE_NUM];

OS_EVENT	*fp_long_ack_queue_handle;
void* fp_long_ack_queue_p[FP_SHORT_ACK_QUEUE_NUM];


fp_short_ack_t fp_short_ack_mem[FP_SHORT_ACK_NUM][1];
fp_long_ack_t fp_long_ack_mem[FP_LONG_ACK_NUM][1];
OS_MEM	*fp_short_ack_mem_handle;
OS_MEM	*fp_long_ack_mem_handle;


//OSMemPut
//OS_ENTER_CRITICAL


int fp_rcv_buf_head_init(void)
{
    uint8_t err = 0;
    fp_rcv_buf_head = (fp_rcv_buf_t *)OSMemGet(fp_rcv_mem_handle, &err);
    if(fp_rcv_buf_head != NULL)
    {
        fp_rcv_buf_head->next = NULL;
        fp_rcv_buf_head->rcv_len = 0;
        memset(fp_rcv_buf_head->rcv_buf, 0, FP_RCV_SIZE);
    }
    else
    {
        /*
        todo: err process
        */
        return -1;
    }
    return 0;
}



uint16_t get_used_buf_size(void)
{
    uint16_t cnt = 0;
    fp_rcv_buf_t	*ptr = NULL;
    ptr = fp_rcv_buf_head;
    while(ptr->next != NULL)
    {
        ptr = ptr->next;
        cnt++;
    }
    return cnt;
}

int put_fp_rcv_buf(uint8_t *buf, uint16_t len)
{
    fp_rcv_buf_t *p = NULL;
    fp_rcv_buf_t *node = NULL;
    uint8_t err = 0;
    if(get_used_buf_size() < FP_RCV_BUF_NUM)
    {
        node = (fp_rcv_buf_t *)OSMemGet(fp_rcv_mem_handle, &err);
        if(node == NULL)
        {
            /*
            todo: err process
            */
            return -1;
        }
        node->next = NULL;
        node->rcv_len = len;
        memcpy(node->rcv_buf, buf, len);
    }
    else
    {
        /*
        todo: err process
        */
        return -1;
    }
    p = fp_rcv_buf_head;
    while(p->next != NULL)  //链表头不保存储数据
    {
        p = p->next;
    }
    p->next = node;
    return 0;
}

int find_the_buf_in_fp_buf(fp_rcv_buf_t *buf)
{
    return -1;
}

int free_one_fp_rcv_buf(fp_rcv_buf_t *buf)
{
    fp_rcv_buf_t *p = NULL;
    fp_rcv_buf_t *pre_p = NULL;
    p = fp_rcv_buf_head;
    pre_p = fp_rcv_buf_head;
    do
    {
        if(p != buf)
        {
            pre_p = p;
            p = p->next;
        }
        else
        {
            break;
        }
    }
    while(p != NULL);

    if(p == NULL)
    {
        return -1;  //can not find such buf
    }

    OSMemPut(fp_rcv_mem_handle, p);
    if(p->next == NULL)
    {
        pre_p->next = NULL;
    }
    else
    {
        pre_p->next = p->next;
    }
    return 0;
}

fp_rcv_buf_t *get_latest_fp_buf(void)
{
    fp_rcv_buf_t *p = NULL;
    fp_rcv_buf_t *node = NULL;
    p = fp_rcv_buf_head;
    if(p->next != NULL)
    {
        node = p->next;
    }
    return node;
}


static int is_frame_valid(uint8_t *buf, uint16_t len)
{
    int i = 0;
    uint8_t check = 0;
    if(len < 8)
    {
        return -1;
    }

    if(len >= 8)
    {
        if((buf[0] == FINGERPRINT_UART_FRAME_HEADER) && (buf[len - 1] == FINGERPRINT_UART_FRAME_TAIL))
        {
            for(i = 1; i < len - 2; i++)
            {
                check ^= buf[i];
            }
            if(check == buf[len - 2])
            {
                return 0;
            }
        }
    }

    return -1;
}



int fp_uart_frame_proc(fp_rcv_buf_t *node)
{
    uint8_t err = 0;
    uint8_t cmd = 0;
    fp_short_ack_t *fp_short_ack = NULL;
    fp_long_ack_t *fp_long_ack = NULL;
    if(node)
    {
        if(is_frame_valid(node->rcv_buf, node->rcv_len) >= 0)
        {
            if(node->rcv_len == 8)
            {
                cmd = node->rcv_buf[1];
                switch(cmd)
                {
                    case FINGERPRINT_UART_PROTOCOL_CMD_CAP_IMG_GET_FEATURE:
                        fp_short_ack = (fp_short_ack_t *)OSMemGet(fp_short_ack_mem_handle, &err);
                        fp_short_ack->cmd = cmd;
                        fp_short_ack->q1 = node->rcv_buf[2];
                        fp_short_ack->q2 = node->rcv_buf[3];
                        fp_short_ack->result = node->rcv_buf[4];
                        OSQPost(fp_short_ack_queue_handle, (void *)fp_short_ack);
                        return 0;

                    case FINGERPRINT_UART_PROTOCOL_CMD_CAPTURE_1:
                    case FINGERPRINT_UART_PROTOCOL_CMD_CAPTURE_2:
//                    case FINGERPRINT_UART_PROTOCOL_CMD_CAPTURE_3:
//                    case FINGERPRINT_UART_PROTOCOL_CMD_CAPTURE_4:
//                    case FINGERPRINT_UART_PROTOCOL_CMD_CAPTURE_5:
                    case FINGERPRINT_UART_PROTOCOL_CMD_CAPTURE_6:
                        fp_short_ack = (fp_short_ack_t *)OSMemGet(fp_short_ack_mem_handle, &err);
                        fp_short_ack->cmd = cmd;
                        fp_short_ack->q1 = node->rcv_buf[2];
                        fp_short_ack->q2 = node->rcv_buf[3];
                        fp_short_ack->result = node->rcv_buf[4];
                        OSQPost(fp_short_ack_queue_handle, (void *)fp_short_ack);
                        return 0;

                    case FINGERPRINT_UART_PROTOCOL_CMD_GET_USER_NUM:
                        fp_short_ack = (fp_short_ack_t *)OSMemGet(fp_short_ack_mem_handle, &err);
                        fp_short_ack->cmd = cmd;
                        fp_short_ack->q1 = node->rcv_buf[2];
                        fp_short_ack->q2 = node->rcv_buf[3];
                        fp_short_ack->result = node->rcv_buf[4];
                        OSQPost(fp_short_ack_queue_handle, (void *)fp_short_ack);
                        return 0;

                    case FINGERPRINT_UART_PROTOCOL_CMD_COPARE_1_TO_N:
                        fp_short_ack = (fp_short_ack_t *)OSMemGet(fp_short_ack_mem_handle, &err);
                        fp_short_ack->cmd = cmd;
                        fp_short_ack->result = node->rcv_buf[4];
                        fp_short_ack->q1 = node->rcv_buf[2];
                        fp_short_ack->q2 = node->rcv_buf[3];
                        OSQPost(fp_short_ack_queue_handle, (void *)fp_short_ack);
                        return 0;

                    case FINGERPRINT_UART_PROTOCOL_CMD_DEL_ALL_USER:
                        fp_short_ack = (fp_short_ack_t *)OSMemGet(fp_short_ack_mem_handle, &err);
                        fp_short_ack->cmd = cmd;
                        fp_short_ack->q1 = node->rcv_buf[2];
                        fp_short_ack->q2 = node->rcv_buf[3];
                        fp_short_ack->result = node->rcv_buf[4];
                        OSQPost(fp_short_ack_queue_handle, (void *)fp_short_ack);
                        return 0;

                    case FINGERPRINT_UART_PROTOCOL_CMD_DEL_THE_USER:
                        fp_short_ack = (fp_short_ack_t *)OSMemGet(fp_short_ack_mem_handle, &err);
                        fp_short_ack->cmd = cmd;
                        fp_short_ack->q1 = node->rcv_buf[2];
                        fp_short_ack->q2 = node->rcv_buf[3];
                        fp_short_ack->result = node->rcv_buf[4];
                        OSQPost(fp_short_ack_queue_handle, (void *)fp_short_ack);
                        return 0;

                    case FINGERPRINT_UART_PROTOCOL_CMD_COMPARE_THE_FEATURE:
                        fp_short_ack = (fp_short_ack_t *)OSMemGet(fp_short_ack_mem_handle, &err);
                        fp_short_ack->cmd = cmd;
                        fp_short_ack->q1 = node->rcv_buf[2];
                        fp_short_ack->q2 = node->rcv_buf[3];
                        fp_short_ack->result = node->rcv_buf[4];
                        OSQPost(fp_short_ack_queue_handle, (void *)fp_short_ack);
                        return 0;

                    case FINGERPRINT_UART_PROTOCOL_CMD_SAVE_FEATURE:
                        fp_short_ack = (fp_short_ack_t *)OSMemGet(fp_short_ack_mem_handle, &err);
                        fp_short_ack->cmd = cmd;
                        fp_short_ack->q1 = node->rcv_buf[2];
                        fp_short_ack->q2 = node->rcv_buf[3];
                        fp_short_ack->result = node->rcv_buf[4];
                        OSQPost(fp_short_ack_queue_handle, (void *)fp_short_ack);
                        return 0;

                    default :
                        return -2;
                }
            }
            else if(node->rcv_len > 8)
            {
                if(is_frame_valid(node->rcv_buf, 8) >= 0)
                {
                    cmd = node->rcv_buf[1];
                    fp_short_ack = (fp_short_ack_t *)OSMemGet(fp_short_ack_mem_handle, &err);
                    fp_short_ack->cmd = cmd;
                    fp_short_ack->q1 = node->rcv_buf[2];
                    fp_short_ack->q2 = node->rcv_buf[3];
                    fp_short_ack->result = node->rcv_buf[4];
                    OSQPost(fp_short_ack_queue_handle, (void *)fp_short_ack);
                    if(is_frame_valid(&node->rcv_buf[8], node->rcv_len - 8) >= 0)
                    {
                        fp_long_ack = (fp_long_ack_t *)OSMemGet(fp_long_ack_mem_handle, &err);
                        if(fp_long_ack)
                        {
                            memcpy(fp_long_ack->data, &node->rcv_buf[9], node->rcv_len - 11);
                            fp_long_ack->len = node->rcv_len - 11;
                            OSQPost(fp_long_ack_queue_handle, (void *)fp_long_ack);
                        }
                    }
                }
            }
            else
            {
                /*
                todo: frame length error ! !
                */
            }
        }
    }
    return -1;
}



//------ test code for fingerprint uart protocol ------//
void fp_uart_com_send_task(void *pdata)
{
    uint8_t err = 0;
    int i = 0;
    uint8_t test_cmd = 0;
    uint8_t test_cap_cnt[6] = {1,2,2,2,2,3};
    fp_short_ack_t *fp_short_ack = NULL;
    fp_long_ack_t *fp_long_ack = NULL;
    fp_feature_t fp_feature;


    test_cmd = 5;
    while(1)
    {
        //LED0=0;
        OSSemSet(fp_uart_data_come_sem, 0, &err);
        //uart_send(send_buf, 8);
        switch(test_cmd)
        {
            case 1:     // capture feature
                for(i = 0; i < 6; i++)
                {
                    fp_capture_feature(0x025, FP_PERMISSION_2, test_cap_cnt[i]);
                    fp_short_ack = (fp_short_ack_t *)OSQPend(fp_short_ack_queue_handle, 0, &err);
                    if((fp_short_ack->result == FINGERPRINT_ACK_SUCCESS) && (fp_short_ack->cmd == i))
                    {
                        delay_ms(100);  //test code: get right ack
                    }
                    OSMemPut(fp_short_ack_mem_handle, fp_short_ack);
                }
                test_cmd = 2;
                break;

            case 2:     //compare test: 1 to N
                fp_compare_feature();
                fp_short_ack = (fp_short_ack_t *)OSQPend(fp_short_ack_queue_handle, 0, &err);
                if((fp_short_ack->result >= FP_PERMISSION_1) && (fp_short_ack->result <= FP_PERMISSION_3) && (fp_short_ack->cmd == FINGERPRINT_UART_PROTOCOL_CMD_COPARE_1_TO_N))
                {
                    delay_ms(100);  //test code: get right ack
                }
                else if(fp_short_ack->result == FINGERPRINT_ACK_NO_USER)
                {
                    /*
                    todo: no such user
                    */
                }
                else if(fp_short_ack->result == FINGERPRINT_ACK_TIMEOUT)
                {
                    /*
                    todo: timeout
                    */
                }
                OSMemPut(fp_short_ack_mem_handle, fp_short_ack);
                break;

            case 3:     //get user number
                fp_get_user_num();
                fp_short_ack = (fp_short_ack_t *)OSQPend(fp_short_ack_queue_handle, 0, &err);
                if((fp_short_ack->result == FINGERPRINT_ACK_SUCCESS) && (fp_short_ack->cmd == FINGERPRINT_UART_PROTOCOL_CMD_GET_USER_NUM))
                {
                    delay_ms(100);  //test code: get right ack
                }
                OSMemPut(fp_short_ack_mem_handle, fp_short_ack);
                test_cmd = 1;
                break;

            case 4:     //delete all user
                fp_del_all_user();
                fp_short_ack = (fp_short_ack_t *)OSQPend(fp_short_ack_queue_handle, 0, &err);
                if((fp_short_ack->result == FINGERPRINT_ACK_SUCCESS) && (fp_short_ack->cmd == FINGERPRINT_UART_PROTOCOL_CMD_DEL_ALL_USER))
                {
                    delay_ms(100);  //test code: get right ack
                }
                OSMemPut(fp_short_ack_mem_handle, fp_short_ack);
                test_cmd = 3;
                break;

            case 5:     //capture fingerprint image and get feature
                fp_cap_img_and_get_feature();
                fp_short_ack = (fp_short_ack_t *)OSQPend(fp_short_ack_queue_handle, 0, &err);
                if((fp_short_ack->result == FINGERPRINT_ACK_SUCCESS) && (fp_short_ack->cmd == FINGERPRINT_UART_PROTOCOL_CMD_CAP_IMG_GET_FEATURE))
                {
                    fp_long_ack = (fp_long_ack_t *)OSQPend(fp_long_ack_queue_handle, 0, &err);
                    if(fp_long_ack->len > 100)
                    {
                        memcpy(fp_feature.feature, &fp_long_ack->data[3], 193);
                        delay_ms(50);
                        test_cmd = 7;
                    }
                    OSMemPut(fp_long_ack_mem_handle, fp_long_ack);
                    delay_ms(100);  //test code: get right ack
                }
                OSMemPut(fp_short_ack_mem_handle, fp_short_ack);
                break;

            case 6:     //compare the specified feature with captured feature
                fp_compare_the_feature_with_cap_feature(fp_feature.feature, FINGERPRINT_FEATURE_DATA_LEN);
                fp_short_ack = (fp_short_ack_t *)OSQPend(fp_short_ack_queue_handle, 0, &err);
                if((fp_short_ack->result == FINGERPRINT_ACK_SUCCESS) && (fp_short_ack->cmd == FINGERPRINT_UART_PROTOCOL_CMD_COMPARE_THE_FEATURE))
                {
                    delay_ms(100);  //test code: get right ack
                }
                OSMemPut(fp_short_ack_mem_handle, fp_short_ack);
                test_cmd = 5;
                break;

            case 7:     //save feature
                fp_save_feature(fp_feature.feature, FINGERPRINT_FEATURE_DATA_LEN, 0x0666, FP_PERMISSION_3);
                fp_short_ack = (fp_short_ack_t *)OSQPend(fp_short_ack_queue_handle, 0, &err);
                if((fp_short_ack->result == FINGERPRINT_ACK_SUCCESS) && (fp_short_ack->cmd == FINGERPRINT_UART_PROTOCOL_CMD_SAVE_FEATURE))
                {
                    delay_ms(100);  //test code: get right ack
                }
                OSMemPut(fp_short_ack_mem_handle, fp_short_ack);
                test_cmd = 2;
                break;

            case 8:     //delete specified user
                fp_del_user(0x0025);
                fp_short_ack = (fp_short_ack_t *)OSQPend(fp_short_ack_queue_handle, 0, &err);
                if((fp_short_ack->result == FINGERPRINT_ACK_SUCCESS) && (fp_short_ack->cmd == FINGERPRINT_UART_PROTOCOL_CMD_DEL_THE_USER))
                {
                    delay_ms(100);  //test code: ack OK
                }
                OSMemPut(fp_short_ack_mem_handle, fp_short_ack);
                test_cmd = 3;
                break;

            default:
                break;
        }

        delay_ms(100);
        //LED0=1;
        delay_ms(100);
    }
}


void fp_uart_com_rcv_task(void *pdata)
{
    uint8_t err = 0;
    fp_rcv_buf_t *fp_rcv_node = NULL;
    fp_rcv_buf_t node;

    while(1)
    {
        LED1 = 1;
        OSSemPend(fp_uart_data_come_sem, 0, &err);
        LED1 = 0;
        delay_ms(50);
        OS_ENTER_CRITICAL();
        fp_rcv_node = get_latest_fp_buf();
        memcpy(&node, fp_rcv_node, sizeof(fp_rcv_buf_t));
        free_one_fp_rcv_buf(fp_rcv_node);
        OS_EXIT_CRITICAL();
        fp_uart_frame_proc(&node);
    }
}

