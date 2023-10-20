#include "header.h" // ȫ�ֿ�
/*
 ***************************ȫ�ֱ�������****************************************/
/* OpenMV����֡��ʽ */
uint16_t OpenMV_Data[17];
/* �����־λ */
 uint8_t Task_flag1,Task_flag2,Task_flag3,Task_flag4;
 uint8_t num_receive;
 /* Ŀ��λ�õ� */
 float Target_Place1_Out[2],Target_Place2_Out[2],Target_Place3_Out[2],Target_Place4_Out[2];
 float Target_Place1_In[2],Target_Place2_In[2],Target_Place3_In[2],Target_Place4_In[2]; 
 float Target_Place1[2],Target_Place2[2],Target_Place3[2],Target_Place4[2];
 /* ��ǰλ�õ� */
 float Actual_Place[2];
 /* ��ʼ�˶����Ʊ�־ */
 uint8_t begin_control;
 uint8_t Shut;
/******************************������******************************************/
int main(void)
{
    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz */
    delay_init(72);                     /* ��ʱ��ʼ�� */
    led_init();                         /* LED��ʼ�� */
    key_init();                         /* KEY��ʼ�� */
    usart_init(115200);                 /* ����1Ĭ�ϳ�ʼ�� */
    printf("Hello STM32\r\n");          /* ������ʾ */
    
    /* ͨ�ö�ʱ����ʼ�� */
    gtim_timx_pwm_chy_init();
    btim_timx_int_init(7100 - 1,50 - 1);
    /* gpio��ʼ�� */
    gpio_init();
    exti_init();
    /* oled��ʼ�� */
    // OLED_Init();
    // delay_ms(100);

    /* �Զ���λ */
    /* ����ִ�� */
    // while(1);
    Send_For_OpenMV(1);
    /* ����12����ʹ�ò��� */
    // Task_Select(1);
    // Task_Select(4);
    /* ����ѡ�� */
    // int8_t ii = 0;
    while(1)
    {
        if (key_scan(GPIOE,GPIO_PIN_8)) {Task_Select(4);break;}
//        if (key_scan(GPIOD,GPIO_PIN_15)) {Task_Select(3);break;}
        if (key_scan(GPIOD,GPIO_PIN_14)) {Task_Select(3);break;}
    }
    // Task_Select(4);
    /* oled��ʾ */
    while(1)
    {

        printf("%f,%f\r\n",Integral_bias_X,Integral_bias_Y);

        /* ������ʱ */
        delay_ms(50);
			
        /* �������ݴ�ӡ��ѯ */
        // for (int i = 0; i<=8; i++)
        // printf("openmv %d:%d\r\n",i,OpenMV_Data[i]);
        // printf("\n");
        // printf("%d,%d\n",target_x,target_y);
        // printf("x:%f\r\n",Move_X);
        // printf("y:%f\r\n",Move_Y);
        // printf("num_receive:%d\r\n",num_receive);

        // printf("OUT Target_Place1:%f,%f\r\n",Target_Place1_Out[0],Target_Place1_Out[1]);
        // printf("out Target_Place2:%f,%f\r\n",Target_Place2_Out[0],Target_Place2_Out[1]);
        // printf("out Target_Place3:%f,%f\r\n",Target_Place3_Out[0],Target_Place3_Out[1]);
        // printf("out Target_Place4:%f,%f\r\n",Target_Place4_Out[0],Target_Place4_Out[1]);

        // printf("in Target_Place1:%f,%f\r\n",Target_Place1_In[0],Target_Place1_In[1]);
        // printf("in Target_Place2:%f,%f\r\n",Target_Place2_In[0],Target_Place2_In[1]);
        // printf("in Target_Place3:%f,%f\r\n",Target_Place3_In[0],Target_Place3_In[1]);
        // printf("in Target_Place4:%f,%f\r\n",Target_Place4_In[0],Target_Place4_In[1]);

        // printf("Target_Place1:%f,%f\r\n",Target_Place1[0],Target_Place1[1]);
        // printf("Target_Place2:%f,%f\r\n",Target_Place2[0],Target_Place2[1]);
        // printf("Target_Place3:%f,%f\r\n",Target_Place3[0],Target_Place3[1]);
        // printf("Target_Place4:%f,%f\r\n",Target_Place4[0],Target_Place4[1]);
        // printf("Actual_Place:%f,%f\r\n",Actual_Place[0],Actual_Place[1]);
        // printf("n:%d\r\n",n);

        // /* ������·������ �ȴ�һ��ʱ���ٽ����ƶ� */
        // if (Task_flag4 == 1 && label_enough_atlas != 0 && label_receive_atlas_in != 0 && ii>=0)
        // {
        //     ii++;
        //     if (ii >= 1) {begin_control = 1;ii=-1;}
        // }
        
        /* ��ʱ�ĵ�����PID����(������) */
        // if (Task_flag3 == 1 && Task_flag1 == 1 && Task_flag2 == 1 && ii>70) {ii = 0; dist_label++;}

        // /* ѭ��������� */
        // if (dist_label >= 5) dist_label = 0;
    }
}




