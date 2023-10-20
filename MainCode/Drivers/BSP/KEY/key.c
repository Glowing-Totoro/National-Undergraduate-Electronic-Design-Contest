#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"

void key_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();    
    __HAL_RCC_GPIOA_CLK_ENABLE();

    gpio_init_struct.Pin = GPIO_PIN_15|GPIO_PIN_14;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD,&gpio_init_struct);
    gpio_init_struct.Pin = GPIO_PIN_8;
    HAL_GPIO_Init(GPIOE,&gpio_init_struct);
    gpio_init_struct.Pin = GPIO_PIN_15;
    HAL_GPIO_Init(GPIOA,&gpio_init_struct);
}

uint8_t key_scan(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == 0)
    {
        delay_ms(10);       /* 消抖 */
        if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == 0)     /* 按键按下 */
        {
            while(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == 0);     /* 等待按键松开 */
            return 1;
        }
    }
    return 0;     /* 按键未按下 */
}
