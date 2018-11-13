/*
 *  Author: Kaka Xie
 *  brief: fingerprint hardware operation
 */

#include "fingerprint.h"

void fp_com_init(void)
{
    uart2_dma_init(19200);
}

int fp_com_send(uint8_t *data, uint16_t len)
{
    return uart2_send(data, len);
}


int fp_capture_feature(uint16_t id, fp_permission_e permission, uint8_t cnt)
{
    uint8_t send_buf[8] = {0};
    uint8_t i = 0;
    if((id > 0x0fff) || (id == 0))
    {
        return -1;
    }
    send_buf[0] = FINGERPRINT_UART_FRAME_HEADER;
    send_buf[1] = cnt;
    send_buf[2] = id >> 8;
    send_buf[3] = id & 0xff;
    send_buf[4] = permission;
    send_buf[5] = 0;
    for(i = 1; i < 6; i++)
    {
        send_buf[6] ^= send_buf[i];
    }
    send_buf[7] = FINGERPRINT_UART_FRAME_TAIL;
    fp_com_send(send_buf, 8);
    return 0;
}

int fp_compare_feature(void)
{
    uint8_t send_buf[8] = {0};
    uint8_t i = 0;

    send_buf[0] = FINGERPRINT_UART_FRAME_HEADER;
    send_buf[1] = FINGERPRINT_UART_PROTOCOL_CMD_COPARE_1_TO_N;
    send_buf[2] = 0;
    send_buf[3] = 0;
    send_buf[4] = 0;
    send_buf[5] = 0;
    for(i = 1; i < 6; i++)
    {
        send_buf[6] ^= send_buf[i];
    }
    send_buf[7] = FINGERPRINT_UART_FRAME_TAIL;
    fp_com_send(send_buf, 8);

    return 0;
}

int fp_get_user_num(void)
{
    uint8_t send_buf[8] = {0};
    uint8_t i = 0;

    send_buf[0] = FINGERPRINT_UART_FRAME_HEADER;
    send_buf[1] = FINGERPRINT_UART_PROTOCOL_CMD_GET_USER_NUM;
    send_buf[2] = 0;
    send_buf[3] = 0;
    send_buf[4] = 0;
    send_buf[5] = 0;
    for(i = 1; i < 6; i++)
    {
        send_buf[6] ^= send_buf[i];
    }
    send_buf[7] = FINGERPRINT_UART_FRAME_TAIL;
    fp_com_send(send_buf, 8);
    return 0;
}

int fp_del_all_user(void)
{
    uint8_t send_buf[8] = {0};
    uint8_t i = 0;

    send_buf[0] = FINGERPRINT_UART_FRAME_HEADER;
    send_buf[1] = FINGERPRINT_UART_PROTOCOL_CMD_DEL_ALL_USER;
    send_buf[2] = 0;
    send_buf[3] = 0;
    send_buf[4] = 0;
    send_buf[5] = 0;
    for(i = 1; i < 6; i++)
    {
        send_buf[6] ^= send_buf[i];
    }
    send_buf[7] = FINGERPRINT_UART_FRAME_TAIL;
    fp_com_send(send_buf, 8);
    return 0;
}

int fp_del_user(uint16_t id)
{
    uint8_t send_buf[8] = {0};
    uint8_t i = 0;

    send_buf[0] = FINGERPRINT_UART_FRAME_HEADER;
    send_buf[1] = FINGERPRINT_UART_PROTOCOL_CMD_DEL_THE_USER;
    send_buf[2] = id >> 8;
    send_buf[3] = (uint8_t)(id & 0xff);
    send_buf[4] = 0;
    send_buf[5] = 0;
    for(i = 1; i < 6; i++)
    {
        send_buf[6] ^= send_buf[i];
    }
    send_buf[7] = FINGERPRINT_UART_FRAME_TAIL;
    fp_com_send(send_buf, 8);
    return 0;
}

int fp_cap_img_and_get_feature(void)
{
    uint8_t send_buf[8] = {0};
    uint8_t i = 0;

    send_buf[0] = FINGERPRINT_UART_FRAME_HEADER;
    send_buf[1] = FINGERPRINT_UART_PROTOCOL_CMD_CAP_IMG_GET_FEATURE;
    send_buf[2] = 0;
    send_buf[3] = 0;
    send_buf[4] = 0;
    send_buf[5] = 0;
    for(i = 1; i < 6; i++)
    {
        send_buf[6] ^= send_buf[i];
    }
    send_buf[7] = FINGERPRINT_UART_FRAME_TAIL;
    fp_com_send(send_buf, 8);
    return 0;
}

int fp_compare_the_feature_with_cap_feature(uint8_t *feature, uint16_t len)
{
    uint8_t send_buf[220] = {0};
    uint8_t i = 0;

    if(len > 193)
    {
        return -1;
    }
    send_buf[0] = FINGERPRINT_UART_FRAME_HEADER;
    send_buf[1] = FINGERPRINT_UART_PROTOCOL_CMD_COMPARE_THE_FEATURE;
    send_buf[2] = (uint8_t)((len + 3) >> 8);
    send_buf[3] = (uint8_t)((len + 3) & 0xff);
    send_buf[4] = 0;
    send_buf[5] = 0;
    for(i = 1; i < 6; i++)
    {
        send_buf[6] ^= send_buf[i];
    }
    send_buf[7] = FINGERPRINT_UART_FRAME_TAIL;
    //fp_com_send(send_buf, 8);

#if 0
    send_buf[0 + 8] = FINGERPRINT_UART_FRAME_HEADER;
    send_buf[1 + 8] = 0;
    send_buf[2 + 8] = 0;
    send_buf[3 + 8] = 0;
    for(i = 0; i < len; i++)
    {
        send_buf[i + 4 + 8] = feature[i];
    }
    for(i = 1; i < len + 4; i++)
    {
        send_buf[len + 1 + 8] ^= send_buf[i + 8];
    }
    send_buf[len + 2 + 8] = FINGERPRINT_UART_FRAME_TAIL;
    fp_com_send(send_buf, len + 6 + 8);
#else
    //delay_ms(10);
    send_buf[0 + 8] = FINGERPRINT_UART_FRAME_HEADER;
    send_buf[1 + 8] = 0;
    send_buf[2 + 8] = 0;
    send_buf[3 + 8] = 0;
    for(i = 0; i < len; i++)
    {
        send_buf[i + 4 + 8] = feature[i];
    }
    for(i = 1; i < len + 4; i++)
    {
        send_buf[len + 1 + 3 + 8] ^= send_buf[i + 8];
    }
    send_buf[len + 2 + 3 + 8] = FINGERPRINT_UART_FRAME_TAIL;
    fp_com_send(send_buf, len + 6 + 8);
#endif
    return 0;
}


int fp_save_feature(uint8_t *feature, uint16_t len, uint16_t id, fp_permission_e permission)
{
    uint8_t send_buf[220] = {0};
    uint8_t i = 0;

    if(len > 193)
    {
        return -1;
    }
    send_buf[0] = FINGERPRINT_UART_FRAME_HEADER;
    send_buf[1] = FINGERPRINT_UART_PROTOCOL_CMD_SAVE_FEATURE;
    send_buf[2] = (uint8_t)((len + 3) >> 8);
    send_buf[3] = (uint8_t)((len + 3) & 0xff);
    send_buf[4] = 0;
    send_buf[5] = 0;
    for(i = 1; i < 6; i++)
    {
        send_buf[6] ^= send_buf[i];
    }
    send_buf[7] = FINGERPRINT_UART_FRAME_TAIL;
    //fp_com_send(send_buf, 8);

    send_buf[0 + 8] = FINGERPRINT_UART_FRAME_HEADER;
    send_buf[1 + 8] = (uint8_t)(id >> 8);
    send_buf[2 + 8] = (uint8_t)(id & 0xff);
    send_buf[3 + 8] = permission;
    for(i = 0; i < len; i++)
    {
        send_buf[i + 4 + 8] = feature[i];
    }
    for(i = 1; i < len + 4; i++)
    {
        send_buf[len + 1 + 3 + 8] ^= send_buf[i + 8];
    }
    send_buf[len + 2 + 3 + 8] = FINGERPRINT_UART_FRAME_TAIL;
    fp_com_send(send_buf, len + 6 + 8);

    return 0;
}


