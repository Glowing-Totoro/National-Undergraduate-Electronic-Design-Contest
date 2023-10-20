#include "./BSP/TIMER/generaltimer.h"

TIM_HandleTypeDef tim1_pwm_handle;
TIM_HandleTypeDef g_timx_handle;

/* 定时器中断初始化函数 */
void btim_timx_int_init(uint16_t psc,uint16_t arr)
{
    g_timx_handle.Instance = TIM6;
    g_timx_handle.Init.Prescaler = psc;
    g_timx_handle.Init.Period = arr;
    g_timx_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    HAL_TIM_Base_Init(&g_timx_handle);
    HAL_TIM_Base_Start_IT(&g_timx_handle);
}

/* 定时器基础MSP初始化函数 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)
    {
        __HAL_RCC_TIM6_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM6_IRQn,0,2);
        HAL_NVIC_EnableIRQ(TIM6_IRQn);
    }
}

/* 定时器6中断服务函数 */
void TIM6_IRQHandler()
{
    HAL_TIM_IRQHandler(&g_timx_handle);
}
/* 定时器溢出中断中断回调函数 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)
    {
        Min_Control_5ms();
    }
}


/* 定时器初始化函数 */
void gtim_timx_pwm_chy_init(void)
{
    /* TIM1_CH1 CH4 舵机驱动 */
    TIM_OC_InitTypeDef tim1_oc_pwm_chy = {0};
		
	tim1_pwm_handle.Instance = TIM1;
    tim1_pwm_handle.Init.Prescaler = 72 - 1;
    tim1_pwm_handle.Init.Period = 20000 - 1;
    tim1_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim1_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    tim1_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_PWM_Init(&tim1_pwm_handle);
	
	tim1_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;
    /* 占空比500 - 2500 ->  0 - 1 */
    tim1_oc_pwm_chy.Pulse = 1875;
    tim1_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_HIGH;
		
	HAL_TIM_PWM_ConfigChannel(&tim1_pwm_handle,&tim1_oc_pwm_chy,TIM_CHANNEL_1);
    tim1_oc_pwm_chy.Pulse = 1772;
    HAL_TIM_PWM_ConfigChannel(&tim1_pwm_handle,&tim1_oc_pwm_chy,TIM_CHANNEL_4);
		    
    HAL_TIM_PWM_Start(&tim1_pwm_handle,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&tim1_pwm_handle,TIM_CHANNEL_4);
		
}

/* 定时器输出PWM MSP初始化函数 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        GPIO_InitTypeDef gpio_init_struct;
			
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_TIM1_CLK_ENABLE();

        gpio_init_struct.Pin = GPIO_PIN_8 ;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        gpio_init_struct.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(GPIOA,&gpio_init_struct);
	    gpio_init_struct.Pin = GPIO_PIN_11;
        HAL_GPIO_Init(GPIOA,&gpio_init_struct);
    }      
}
