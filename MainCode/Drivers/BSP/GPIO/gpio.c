#include "./BSP/GPIO/gpio.h"

void gpio_init(void)
{
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_AFIO_REMAP_SWJ_NOJTAG();
    
    /* oledÇý¶¯Òý½ÅPD6\PD7\PB8\PB9 */
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    
    gpio_init_struct.Pin = GPIO_PIN_6;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD,&gpio_init_struct);
    gpio_init_struct.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOD,&gpio_init_struct);
    gpio_init_struct.Pin = GPIO_PIN_8;
    HAL_GPIO_Init(GPIOB,&gpio_init_struct);
    gpio_init_struct.Pin = GPIO_PIN_9;
    HAL_GPIO_Init(GPIOB,&gpio_init_struct);    
    
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
}
