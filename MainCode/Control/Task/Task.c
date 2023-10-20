#include "Task.h"

/* 任务逻辑设计 */
void Task_Select(uint8_t Task_flag)
{
    if (Task_flag == 0)
        return;
    if (Task_flag == 1)
        Task1();
    if (Task_flag == 2)
        Task2();
    if (Task_flag == 3)
        Task3();
    if (Task_flag == 4)
        Task4();
    return;
}

/* 任务1 外部一圈 */
void Task1(void)
{
    // Servo(1875,1824);
    Servo(1875,1720);    
    delay_ms(500);
    Servo(1967,1867);
    delay_ms(500);
    // Servo(1915,1920);    
    // delay_ms(500);
    // Servo(1865,1922);
    // delay_ms(500);
    // Servo(1815,1920);
    // delay_ms(500);
    Servo(1785,1873);
    delay_ms(500);
    Servo(1785,1572);
    delay_ms(500);
    Servo(1965,1572);
    delay_ms(500);
    Servo(1965,1867);
    delay_ms(500);
    Servo(1875,1720);    
}

/* 任务2  */
void Task2(void)
{
    Task_flag2 = 1;
}

/* 任务3  */
void Task3(void)
{
    Task_flag3 = 1;
    dist_label = 0;
    OpenMV_Follow_KP_X = - 0.00000 ;
    OpenMV_Follow_KI_X = - 0.2 ;
    OpenMV_Follow_KD_X  = - 0.0000001 ;
    OpenMV_Follow_KP_Y = - 0.00000 ;
    OpenMV_Follow_KI_Y = - 0.25 ;
    OpenMV_Follow_KD_Y = - 0.0000001 ;
}

/* 任务4 */
void Task4(void)
{
    intex = 600;
    intey = 800;
    Task_flag4 = 1;
    dist_label = 0;
    OpenMV_Follow_KP_X = - 0.000000 ;
    OpenMV_Follow_KI_X = - 0.025 ;
    OpenMV_Follow_KD_X = - 0.0001 ;
    OpenMV_Follow_KP_Y = - 0.000000 ;
    OpenMV_Follow_KI_Y = - 0.045 ;
    OpenMV_Follow_KD_Y = - 0.00013 ;
}
// /* 任务0 & 其他 */
// void Task0()
// {

// }
