#include "./BSP/ADC/adc.h"
#include "./SYSTEM/delay/delay.h"

ADC_HandleTypeDef g_adc_handle;
ADC_HandleTypeDef ccd_adc_handle;

/* ADC��ͨ�� ��ʼ�� */
void adc_init(void)
{
    /* ��ص�ѹ����ʼ�� */
    ADC_ChannelConfTypeDef adc_ch_config;
    
    g_adc_handle.Instance = ADC1;
    g_adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    g_adc_handle.Init.ScanConvMode = ADC_SCAN_DISABLE;
    g_adc_handle.Init.ContinuousConvMode = DISABLE;
    g_adc_handle.Init.NbrOfConversion = 1;
    g_adc_handle.Init.DiscontinuousConvMode = DISABLE;
    g_adc_handle.Init.NbrOfDiscConversion = 0;
    g_adc_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    
    HAL_ADC_Init(&g_adc_handle);
    
    HAL_ADCEx_Calibration_Start(&g_adc_handle);
    
    adc_ch_config.Channel = ADC_CHANNEL_11;
    adc_ch_config.Rank = ADC_REGULAR_RANK_1;
    adc_ch_config.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(&g_adc_handle,&adc_ch_config);    
    
    /* CCD ADC ��ʼ�� */
    ccd_adc_handle.Instance = ADC2;
    ccd_adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    ccd_adc_handle.Init.ScanConvMode = ADC_SCAN_DISABLE;
    ccd_adc_handle.Init.ContinuousConvMode = DISABLE;
    ccd_adc_handle.Init.NbrOfConversion = 1;
    ccd_adc_handle.Init.DiscontinuousConvMode = DISABLE;
    ccd_adc_handle.Init.NbrOfDiscConversion = 0;
    ccd_adc_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    
    HAL_ADC_Init(&ccd_adc_handle);
    
    HAL_ADCEx_Calibration_Start(&ccd_adc_handle);
    
    adc_ch_config.Channel = ADC_CHANNEL_15;
    adc_ch_config.Rank = ADC_REGULAR_RANK_1;
    adc_ch_config.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(&ccd_adc_handle,&adc_ch_config);
}

/* ADC MSP��ʼ������ */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1)
    {
        GPIO_InitTypeDef gpio_init_struct;
        RCC_PeriphCLKInitTypeDef adc_clk_init = {0};
        
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_ADC1_CLK_ENABLE();
        
        gpio_init_struct.Pin = GPIO_PIN_1;
        gpio_init_struct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOC,&gpio_init_struct);
        
        adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;
        adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;
        HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);
    }
    if (hadc->Instance == ADC2)
    {
        GPIO_InitTypeDef gpio_init_struct;
        RCC_PeriphCLKInitTypeDef adc_clk_init = {0};
        
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_ADC2_CLK_ENABLE();
        
        gpio_init_struct.Pin = GPIO_PIN_5;
        gpio_init_struct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOC,&gpio_init_struct);
        
        adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;
        adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;
        HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);
    }
}

/**
 * @brief       ����ADCͨ������ʱ��
 * @param       adcx : adc���ָ��,ADC_HandleTypeDef
 * @param       ch   : ͨ����, ADC_CHANNEL_0~ADC_CHANNEL_17
 * @param       stime: ����ʱ��  0~7, ��Ӧ��ϵΪ:
 *   @arg       ADC_SAMPLETIME_1CYCLE_5, 1.5��ADCʱ������        ADC_SAMPLETIME_7CYCLES_5, 7.5��ADCʱ������
 *   @arg       ADC_SAMPLETIME_13CYCLES_5, 13.5��ADCʱ������     ADC_SAMPLETIME_28CYCLES_5, 28.5��ADCʱ������
 *   @arg       ADC_SAMPLETIME_41CYCLES_5, 41.5��ADCʱ������     ADC_SAMPLETIME_55CYCLES_5, 55.5��ADCʱ������
 *   @arg       ADC_SAMPLETIME_71CYCLES_5, 71.5��ADCʱ������     ADC_SAMPLETIME_239CYCLES_5, 239.5��ADCʱ������
 * @param       rank: ��ͨ���ɼ�ʱ��Ҫ���õĲɼ����,
                �����㶨��channle1��rank=1��channle2 ��rank=2��
                ��ô��Ӧ����DMA����ռ�ı�������AdcDMA[0] ��i��channle1��ת�������AdcDMA[1]����ͨ��2��ת������� 
                ��ͨ��DMA����Ϊ ADC_REGULAR_RANK_1
 *   @arg       ���1~16��ADC_REGULAR_RANK_1~ADC_REGULAR_RANK_16
 * @retval      ��
 */
void adc_channel_set(ADC_HandleTypeDef adc_handle, uint32_t ch, uint32_t rank, uint32_t stime)
{
    ADC_ChannelConfTypeDef adc_ch_conf;
    
    adc_ch_conf.Channel = ch;                            /* ͨ�� */
    adc_ch_conf.Rank = rank;                             /* ���� */
    adc_ch_conf.SamplingTime = stime;                    /* ����ʱ�� */
    HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);     /* ͨ������ */
}

/* ���ADCת����Ľ������ */
uint32_t adc_get_result(ADC_HandleTypeDef adc_handle,uint32_t ch)
{
    adc_channel_set(adc_handle,ch,ADC_REGULAR_RANK_1, ADC_SAMPLETIME_239CYCLES_5);    /* ����ͨ�������кͲ���ʱ�� */

    HAL_ADC_Start(&adc_handle);                            /* ����ADC */
    HAL_ADC_PollForConversion(&adc_handle, 1);            /* ��ѯת�� */
    return (uint16_t)HAL_ADC_GetValue(&adc_handle);        /* �������һ��ADC1�������ת����� */
}

/* ADCת�� & times��ȡƽ��ֵ */
uint32_t adc_get_result_average(ADC_HandleTypeDef adc_handle,uint32_t ch, uint8_t times)
{
    uint32_t temp_val = 0;
    uint8_t t;

    for (t = 0; t < times; t++)     /* ��ȡtimes������ */
    {
        temp_val += adc_get_result(adc_handle,ch);
        delay_ms(1);
    }

    return temp_val / times;        /* ����ƽ��ֵ */
}
