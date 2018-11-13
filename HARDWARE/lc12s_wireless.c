/*
 *  Author: Kaka Xie
 *  brief: lc12s wireless module hardware operation
 */

#include "lc12s_wireless.h"

void lc12s_com_init(void)
{
    uart1_dma_init(9600);
}


int lc12s_com_send(uint8_t *data, uint16_t len)
{
    return uart1_send(data, len);
}


void lc12s_send_com_test(void)
{
    uint8_t test_buf[100] = {0};
    int i = 0;
    for(i = 0; i < 100; i++)
    {
        test_buf[i] = i;
    }
    lc12s_com_send("abcdefghijklmn", 10);
    //lc12s_com_send(test_buf, sizeof(test_buf));

}
