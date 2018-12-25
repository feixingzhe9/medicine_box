#include "adc.h"

/**
 * @brief ADC��ʼ��
 * @param none
 * @retval none
 * @note  ��ʼ��PA.00ΪADC1_CH1������ת�����������ADCת��
 */
void adc_init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF | RCC_APB2Periph_ADC3 | RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC3ͨ��ʱ��


    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

    //PF7 ��Ϊģ��ͨ����������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    ADC_DeInit(ADC3);  //��λADC3,������ ADC3 ��ȫ���Ĵ�������Ϊȱʡֵ
    ADC_DeInit(ADC1);  //��λADC3,������ ADC3 ��ȫ���Ĵ�������Ϊȱʡֵ

    //	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 2;	//˳����й���ת����ADCͨ������Ŀ
    ADC_Init(ADC3, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���
    ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���
    ADC_TempSensorVrefintCmd(ENABLE);

    ADC_Cmd(ADC3, ENABLE);	//ʹ��ָ����ADC3
    ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC3

    ADC_ResetCalibration(ADC3);	//ʹ�ܸ�λУ׼
    ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼

    while(ADC_GetResetCalibrationStatus(ADC3));	//�ȴ���λУ׼����

    ADC_StartCalibration(ADC3);	 //����ADУ׼

    while(ADC_GetCalibrationStatus(ADC3));	 //�ȴ�У׼����


    while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����

    ADC_StartCalibration(ADC1);	 //����ADУ׼

    while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}


/**
 * @brief ���ADCֵ
 * @param ADCx: ADC1 / ADC2 / ADC3
 * @param ch:ͨ��ֵ ADC_Channel_0 ~ ADC_Channel_17
 * @retval ADCת��ֵ
 * @note
 */
uint16_t get_adc(ADC_TypeDef* ADCx, uint8_t ch)
{
    /* ADC�Ĺ���ͨ�������ã�һ�����У�����ʱ��239.5����	*/
    ADC_RegularChannelConfig(ADCx, ch, 1, ADC_SampleTime_239Cycles5 );

    ADC_SoftwareStartConvCmd(ADCx, ENABLE);					//��ʼת��
    while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC ));	//�ȴ�ת������
    return ADC_GetConversionValue(ADCx);						//����ADC1ת�����
}
