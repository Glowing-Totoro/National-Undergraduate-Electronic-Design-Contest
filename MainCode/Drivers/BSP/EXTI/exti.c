#include "header.h"

/* 外部中断初始化，自定义 */
void exti_init(void)
{
    /* 中断结构体定义 */
    GPIO_InitTypeDef gpio_init_struct;
    /* 开启时钟 */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /* 设置结构体参数，参考原理图 */
    /* 根据要求选择实现引脚与模式设置 */
    gpio_init_struct.Pin = GPIO_PIN_9;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;
    /* 这里选择上下拉更加稳定触发 */
    gpio_init_struct.Pull = GPIO_PULLUP;
    /* 初始化结构体 */
    HAL_GPIO_Init(GPIOE,&gpio_init_struct);
    gpio_init_struct.Pin = GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD,&gpio_init_struct);
    /* 设置外部中断两位优先级 */
    HAL_NVIC_SetPriority(EXTI9_5_IRQn,0,0);
    HAL_NVIC_SetPriority(EXTI15_10_IRQn,1,2);
    
    /* 使能中断 */
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/* 中断服务函数&消除中断位 */
void EXTI9_5_IRQHandler(void)
{
    /* 调用hal库自定义函数(直接调用Callback) */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
    /* 消除中断标志位 */
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
}

void EXTI15_10_IRQHandler(void)
{
    /* 调用hal库自定义函数(直接调用Callback) */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
    /* 消除中断标志位 */
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);
}

/* 中断服务函数调用函数 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN)
{
//    /* 软件消抖 */
    delay_ms(20);
    /* 判断中断触发引脚 */
    if(GPIO_PIN == GPIO_PIN_9)
    {
        /* 软件消抖结束&确认中断引脚 */
        if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_9) == 0)
        {
            /* 中断实现功能 */
            Task1();
        }
    }
    /* 判断中断触发引脚 */
    if(GPIO_PIN == GPIO_PIN_15)
    {
        /* 软件消抖结束&确认中断引脚 */
        if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_15) == 0)
        {
            /* 中断实现功能 */
            Servo(1875,1720);
        }
    }
}
