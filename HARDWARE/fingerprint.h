#ifndef __HARDWARE_FINGERPRINT_H__
#define __HARDWARE_FINGERPRINT_H__
#include "stm32f10x.h"
#include "usart.h"


#define FINGERPRINT_UART_FRAME_HEADER   0xf5
#define FINGERPRINT_UART_FRAME_TAIL     0xf5

#define FINGERPRINT_FEATURE_DATA_LEN    193

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
#define FINGERPRINT_UART_PROTOCOL_CMD_DEL_THE_USER          0x04
#define FINGERPRINT_UART_PROTOCOL_CMD_CAP_IMG_GET_FEATURE   0x23
#define FINGERPRINT_UART_PROTOCOL_CMD_COMPARE_THE_FEATURE   0x44
#define FINGERPRINT_UART_PROTOCOL_CMD_SAVE_FEATURE          0x41

#define FINGERPRINT_ACK_SUCCESS         0x00	//successful
#define FINGERPRINT_ACK_FAIL            0x01	//failure
#define FINGERPRINT_ACK_FULL            0x04	//fingerprint lib is full !
#define FINGERPRINT_ACK_NO_USER         0x05	//no such user
#define FINGERPRINT_ACK_USER_OCCUPIED   0x06    //
#define FINGERPRINT_ACK_USER_EXIST      0x07 	//
#define FINGERPRINT_ACK_TIMEOUT         0x08	//operation timeout

typedef enum
{
    FP_PERMISSION_1 = 1,
    FP_PERMISSION_2 = 2,
    FP_PERMISSION_3 = 3,
}fp_permission_e;


#define FP_RCV_BUF_NUM  10
typedef struct fp_rcv_buf_t
{
    uint8_t rcv_buf[FP_RCV_SIZE];
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

void fp_en(void);
void fp_com_init(void);

int fp_capture_feature(uint16_t id, fp_permission_e permission, uint8_t cnt);
int fp_compare_feature(void);
int fp_get_user_num(void);
int fp_del_all_user(void);
int fp_del_user(uint16_t id);
int fp_cap_img_and_get_feature(void);
int fp_compare_the_feature_with_cap_feature(uint8_t *feature, uint16_t len);
int fp_save_feature(uint8_t *feature, uint16_t len, uint16_t id, fp_permission_e permission);
int fp_uart_frame_proc(fp_rcv_buf_t *node);
#endif
