#ifndef __ADC_H
#define __ADC_H

#include "./SYSTEM/sys/sys.h"

extern ADC_HandleTypeDef g_adc_handle;
extern ADC_HandleTypeDef ccd_adc_handle;

/* ADC初始化函数 */
void adc_init(void);
/* 配置ADC通道 */
void adc_channel_set(ADC_HandleTypeDef adc_handle, uint32_t ch, uint32_t rank, uint32_t stime);
/* 获得某个通道值  */
uint32_t adc_get_result(ADC_HandleTypeDef adc_handle,uint32_t ch);
/* 得到某个通道给定次数采样的平均值 */
uint32_t adc_get_result_average(ADC_HandleTypeDef adc_handle,uint32_t ch, uint8_t times);

#endif
