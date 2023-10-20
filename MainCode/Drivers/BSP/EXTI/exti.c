#include "header.h"

/* �ⲿ�жϳ�ʼ�����Զ��� */
void exti_init(void)
{
    /* �жϽṹ�嶨�� */
    GPIO_InitTypeDef gpio_init_struct;
    /* ����ʱ�� */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /* ���ýṹ��������ο�ԭ��ͼ */
    /* ����Ҫ��ѡ��ʵ��������ģʽ���� */
    gpio_init_struct.Pin = GPIO_PIN_9;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;
    /* ����ѡ�������������ȶ����� */
    gpio_init_struct.Pull = GPIO_PULLUP;
    /* ��ʼ���ṹ�� */
    HAL_GPIO_Init(GPIOE,&gpio_init_struct);
    gpio_init_struct.Pin = GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD,&gpio_init_struct);
    /* �����ⲿ�ж���λ���ȼ� */
    HAL_NVIC_SetPriority(EXTI9_5_IRQn,0,0);
    HAL_NVIC_SetPriority(EXTI15_10_IRQn,1,2);
    
    /* ʹ���ж� */
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/* �жϷ�����&�����ж�λ */
void EXTI9_5_IRQHandler(void)
{
    /* ����hal���Զ��庯��(ֱ�ӵ���Callback) */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
    /* �����жϱ�־λ */
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
}

void EXTI15_10_IRQHandler(void)
{
    /* ����hal���Զ��庯��(ֱ�ӵ���Callback) */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
    /* �����жϱ�־λ */
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);
}

/* �жϷ��������ú��� */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN)
{
//    /* ������� */
    delay_ms(20);
    /* �ж��жϴ������� */
    if(GPIO_PIN == GPIO_PIN_9)
    {
        /* �����������&ȷ���ж����� */
        if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_9) == 0)
        {
            /* �ж�ʵ�ֹ��� */
            Task1();
        }
    }
    /* �ж��жϴ������� */
    if(GPIO_PIN == GPIO_PIN_15)
    {
        /* �����������&ȷ���ж����� */
        if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_15) == 0)
        {
            /* �ж�ʵ�ֹ��� */
            Servo(1875,1720);
        }
    }
}
