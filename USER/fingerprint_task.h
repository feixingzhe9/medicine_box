#ifndef __USER_FINGERPRINT__
#define __USER_FINGERPRINT__
#include "stm32f10x.h"
#include "ucos_ii.h"
#include "usart.h"
#include "sys.h"

#define FP_UART_COM_SEND_TASK_PRIO                      9
#define FP_UART_COM_SEND_TASK_STK_SIZE                  1024
extern OS_STK FP_UART_COM_SEND_TASK_STK[FP_UART_COM_SEND_TASK_STK_SIZE];


#define FP_UART_COM_RCV_TASK_PRIO                       10
#define FP_UART_COM_RCV_TASK_STK_SIZE                   1024
extern OS_STK FP_UART_COM_RCV_TASK_STK[FP_UART_COM_RCV_TASK_STK_SIZE];

#define FINGERPRINT_UART_FRAME_HEADER   0xf5
#define FINGERPRINT_UART_FRAME_TAIL     0xf5


//#define FINGERPRINT_UART_PROTOCOL_CMD_LONG_FRAME        0x00

#define FINGERPRINT_UART_PROTOCOL_CMD_CAPTURE_1         0x01
#define FINGERPRINT_UART_PROTOCOL_CMD_CAPTURE_2         0x02
//#define FINGERPRINT_UART_PROTOCOL_CMD_CAPTURE_3         0x03
//#define FINGERPRINT_UART_PROTOCOL_CMD_CAPTURE_4         0x04
//#define FINGERPRINT_UART_PROTOCOL_CMD_CAPTURE_5         0x05
#define FINGERPRINT_UART_PROTOCOL_CMD_CAPTURE_6         0x03

#define FINGERPRINT_UART_PROTOCOL_CMD_COPARE_1_TO_1         0x0b
#define FINGERPRINT_UART_PROTOCOL_CMD_COPARE_1_TO_N         0x0c
#define FINGERPRINT_UART_PROTOCOL_CMD_GET_USER_NUM          0x09
#define FINGERPRINT_UART_PROTOCOL_CMD_DEL_ALL_USER          0x05
#define FINGERPRINT_UART_PROTOCOL_CMD_CAP_IMG_GET_FEATURE   0x23

#define FINGERPRINT_ACK_SUCCESS         0x00	//successful
#define FINGERPRINT_ACK_FAIL            0x01	//failure
#define FINGERPRINT_ACK_FULL            0x04	//fingerprint lib is full !
#define FINGERPRINT_ACK_NOUSER          0x05	//
#define FINGERPRINT_ACK_USER_OCCUPIED   0x06  //
#define FINGERPRINT_ACK_USER_EXIST      0x07 	//
#define FINGERPRINT_ACK_TIMEOUT         0x08	//

typedef enum
{
    FP_PERMISSION_1 = 1,
    FP_PERMISSION_2 = 2,
    FP_PERMISSION_3 = 3,
}fp_permission_e;

#define FP_RCV_BUF_NUM  10
typedef struct fp_rcv_buf_t
{
    uint8_t rcv_buf[RCV_SIZE];
    uint16_t rcv_len;
    struct fp_rcv_buf_t *next;
}fp_rcv_buf_t;


#pragma pack(1)
#define FP_SHORT_ACK_NUM    5
typedef struct
{
    uint8_t cmd;
    uint8_t q1;
    uint8_t q2;
    uint8_t result; //q3
}fp_short_ack_t;

#define FP_LONG_ACK_NUM     3
typedef struct
{
    uint8_t data[255];
    uint16_t len;
}fp_long_ack_t;

typedef struct
{
    uint8_t feature[193];
    uint16_t id;
}fp_feature_t;
#pragma pack()

extern fp_rcv_buf_t fp_rcv_mem[FP_RCV_BUF_NUM][1];
extern fp_short_ack_t fp_short_ack_mem[FP_SHORT_ACK_NUM][1];

/*os related*/
extern OS_MEM	*fp_rcv_mem_handle;
extern OS_MEM	*fp_short_ack_mem_handle;
extern OS_EVENT *fp_uart_data_come_sem;
extern OS_EVENT *fp_com_get_feature_sem;
extern OS_EVENT *fp_com_read_feature_sem;
extern OS_EVENT *fp_com_set_feature_sem;

#define FP_SHORT_ACK_QUEUE_NUM  FP_SHORT_ACK_NUM
#define FP_LONG_ACK_QUEUE_NUM   FP_LONG_ACK_NUM
extern OS_EVENT	*fp_short_ack_queue_handle;
extern void* fp_short_ack_queue_p[FP_SHORT_ACK_QUEUE_NUM];

extern OS_EVENT	*fp_long_ack_queue_handle;
extern void* fp_long_ack_queue_p[FP_SHORT_ACK_QUEUE_NUM];

extern OS_MEM	*fp_long_ack_mem_handle;
extern fp_long_ack_t fp_long_ack_mem[FP_LONG_ACK_NUM][1];

void fp_uart_com_send_task(void *pdata);
void fp_uart_com_rcv_task(void *pdata);

int put_fp_rcv_buf(uint8_t *buf, uint16_t len);
int free_one_fp_rcv_buf(fp_rcv_buf_t *buf);

#endif
