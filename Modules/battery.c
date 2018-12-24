/*
 *  Author: Kaka Xie
 *  brief:  battery power percent processing
 */
#include "battery.h"
#include "adc.h"
#include "delay.h"

void battery_adc_init(void)
{
    adc_init();
}


uint16_t get_battery_voltage(void)
{
    uint16_t ref_adc_value;
    uint16_t adc_value;

    adc_value = get_adc(ADC3, 5);
//    delay_ms(10);
    ref_adc_value = get_adc(ADC1, 17);
//    return (uint16_t)(1000.0 * (float)adc_value * (47.0 + 13.0) * 3.565 / (4096.0 * 13.0));
//    return (uint16_t)((float)adc_value  * 1.2 * 1000 / (float)ref_adc_value   * 60.0 / 13.0);
    return (uint16_t)((float)adc_value  * 1.2 * 1000 * 60.0 / ((float)ref_adc_value * 13.0));
}

