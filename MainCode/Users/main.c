#include "header.h" // 全局库
/*
 ***************************全局变量定义****************************************/
/* OpenMV数据帧格式 */
uint16_t OpenMV_Data[17];
/* 任务标志位 */
 uint8_t Task_flag1,Task_flag2,Task_flag3,Task_flag4;
 uint8_t num_receive;
 /* 目标位置点 */
 float Target_Place1_Out[2],Target_Place2_Out[2],Target_Place3_Out[2],Target_Place4_Out[2];
 float Target_Place1_In[2],Target_Place2_In[2],Target_Place3_In[2],Target_Place4_In[2]; 
 float Target_Place1[2],Target_Place2[2],Target_Place3[2],Target_Place4[2];
 /* 当前位置点 */
 float Actual_Place[2];
 /* 开始运动控制标志 */
 uint8_t begin_control;
 uint8_t Shut;
/******************************主函数******************************************/
int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    delay_init(72);                     /* 延时初始化 */
    led_init();                         /* LED初始化 */
    key_init();                         /* KEY初始化 */
    usart_init(115200);                 /* 串口1默认初始化 */
    printf("Hello STM32\r\n");          /* 开机显示 */
    
    /* 通用定时器初始化 */
    gtim_timx_pwm_chy_init();
    btim_timx_int_init(7100 - 1,50 - 1);
    /* gpio初始化 */
    gpio_init();
    exti_init();
    /* oled初始化 */
    // OLED_Init();
    // delay_ms(100);

    /* 自动复位 */
    /* 动作执行 */
    // while(1);
    Send_For_OpenMV(1);
    /* 任务12调试使用部分 */
    // Task_Select(1);
    // Task_Select(4);
    /* 任务选择 */
    // int8_t ii = 0;
    while(1)
    {
        if (key_scan(GPIOE,GPIO_PIN_8)) {Task_Select(4);break;}
//        if (key_scan(GPIOD,GPIO_PIN_15)) {Task_Select(3);break;}
        if (key_scan(GPIOD,GPIO_PIN_14)) {Task_Select(3);break;}
    }
    // Task_Select(4);
    /* oled显示 */
    while(1)
    {

        printf("%f,%f\r\n",Integral_bias_X,Integral_bias_Y);

        /* 简易延时 */
        delay_ms(50);
			
        /* 部分数据打印查询 */
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

        // /* 接受完路径数据 等待一段时间再进行移动 */
        // if (Task_flag4 == 1 && label_enough_atlas != 0 && label_receive_atlas_in != 0 && ii>=0)
        // {
        //     ii++;
        //     if (ii >= 1) {begin_control = 1;ii=-1;}
        // }
        
        /* 延时四点坐标PID控制(已弃用) */
        // if (Task_flag3 == 1 && Task_flag1 == 1 && Task_flag2 == 1 && ii>70) {ii = 0; dist_label++;}

        // /* 循环跟随控制 */
        // if (dist_label >= 5) dist_label = 0;
    }
}




