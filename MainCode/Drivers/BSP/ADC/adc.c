#include "./BSP/ADC/adc.h"
#include "./SYSTEM/delay/delay.h"

ADC_HandleTypeDef g_adc_handle;
ADC_HandleTypeDef ccd_adc_handle;

/* ADC单通道 初始化 */
void adc_init(void)
{
    /* 电池电压检测初始化 */
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
    
    /* CCD ADC 初始化 */
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

/* ADC MSP初始化函数 */
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
 * @brief       设置ADC通道采样时间
 * @param       adcx : adc句柄指针,ADC_HandleTypeDef
 * @param       ch   : 通道号, ADC_CHANNEL_0~ADC_CHANNEL_17
 * @param       stime: 采样时间  0~7, 对应关系为:
 *   @arg       ADC_SAMPLETIME_1CYCLE_5, 1.5个ADC时钟周期        ADC_SAMPLETIME_7CYCLES_5, 7.5个ADC时钟周期
 *   @arg       ADC_SAMPLETIME_13CYCLES_5, 13.5个ADC时钟周期     ADC_SAMPLETIME_28CYCLES_5, 28.5个ADC时钟周期
 *   @arg       ADC_SAMPLETIME_41CYCLES_5, 41.5个ADC时钟周期     ADC_SAMPLETIME_55CYCLES_5, 55.5个ADC时钟周期
 *   @arg       ADC_SAMPLETIME_71CYCLES_5, 71.5个ADC时钟周期     ADC_SAMPLETIME_239CYCLES_5, 239.5个ADC时钟周期
 * @param       rank: 多通道采集时需要设置的采集编号,
                假设你定义channle1的rank=1，channle2 的rank=2，
                那么对应你在DMA缓存空间的变量数组AdcDMA[0] 就i是channle1的转换结果，AdcDMA[1]就是通道2的转换结果。 
                单通道DMA设置为 ADC_REGULAR_RANK_1
 *   @arg       编号1~16：ADC_REGULAR_RANK_1~ADC_REGULAR_RANK_16
 * @retval      无
 */
void adc_channel_set(ADC_HandleTypeDef adc_handle, uint32_t ch, uint32_t rank, uint32_t stime)
{
    ADC_ChannelConfTypeDef adc_ch_conf;
    
    adc_ch_conf.Channel = ch;                            /* 通道 */
    adc_ch_conf.Rank = rank;                             /* 序列 */
    adc_ch_conf.SamplingTime = stime;                    /* 采样时间 */
    HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);     /* 通道配置 */
}

/* 获得ADC转换后的结果函数 */
uint32_t adc_get_result(ADC_HandleTypeDef adc_handle,uint32_t ch)
{
    adc_channel_set(adc_handle,ch,ADC_REGULAR_RANK_1, ADC_SAMPLETIME_239CYCLES_5);    /* 设置通道，序列和采样时间 */

    HAL_ADC_Start(&adc_handle);                            /* 开启ADC */
    HAL_ADC_PollForConversion(&adc_handle, 1);            /* 轮询转换 */
    return (uint16_t)HAL_ADC_GetValue(&adc_handle);        /* 返回最近一次ADC1规则组的转换结果 */
}

/* ADC转换 & times次取平均值 */
uint32_t adc_get_result_average(ADC_HandleTypeDef adc_handle,uint32_t ch, uint8_t times)
{
    uint32_t temp_val = 0;
    uint8_t t;

    for (t = 0; t < times; t++)     /* 获取times次数据 */
    {
        temp_val += adc_get_result(adc_handle,ch);
        delay_ms(1);
    }

    return temp_val / times;        /* 返回平均值 */
}
