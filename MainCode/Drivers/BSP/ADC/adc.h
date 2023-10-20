#ifndef __ADC_H
#define __ADC_H

#include "./SYSTEM/sys/sys.h"

extern ADC_HandleTypeDef g_adc_handle;
extern ADC_HandleTypeDef ccd_adc_handle;

/* ADC��ʼ������ */
void adc_init(void);
/* ����ADCͨ�� */
void adc_channel_set(ADC_HandleTypeDef adc_handle, uint32_t ch, uint32_t rank, uint32_t stime);
/* ���ĳ��ͨ��ֵ  */
uint32_t adc_get_result(ADC_HandleTypeDef adc_handle,uint32_t ch);
/* �õ�ĳ��ͨ����������������ƽ��ֵ */
uint32_t adc_get_result_average(ADC_HandleTypeDef adc_handle,uint32_t ch, uint8_t times);

#endif
