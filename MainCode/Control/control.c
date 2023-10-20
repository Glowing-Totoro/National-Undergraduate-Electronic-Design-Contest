#include "control.h"

// openmv循线参数
float OpenMV_Move_X; // 控制量(m)
uint8_t nums_start;
uint16_t num_stop;
/* 5ms一次精准控制 */
void Min_Control_5ms(void)
{

    // printf("label:%d\r\n",dist_label);
    // printf("stop:%d",num_stop);

    if (key_scan(GPIOA, GPIO_PIN_15))
    {
        if (Shut == 0)
            Shut = 1;
        else
            Shut = 0;
    }
    /* OpenMV 寻找地图&激光 */
    OpenMV_For_Data();
    if (Task_flag2 == 1)
    {
        /* OpenMV 寻找路径 */
        OpenMV_For_Line();
    }
    else if (Task_flag4 == 1 || Task_flag3 == 1)
    {
        if (Shut == 0)
        {
            /* 标志位判断目标 控制逻辑实现 */
            if (label_enough_atlas != 0 && begin_control == 1 && num_stop <= 30)
            {
                /* !目标位置确定 & 误差判断 */
                Error_Find();

                if (dist_label == 0)
                    OpenMV_Follow(Actual_Place[0], Actual_Place[1], Target_Place1[0], Target_Place1[1]);
                else if (dist_label == 1)
                    OpenMV_Follow(Actual_Place[0], Actual_Place[1], Target_Place2[0], Target_Place2[1]);
                else if (dist_label == 2)
                    OpenMV_Follow(Actual_Place[0], Actual_Place[1], Target_Place3[0], Target_Place3[1]);
                else if (dist_label == 3)
                    OpenMV_Follow(Actual_Place[0], Actual_Place[1], Target_Place4[0], Target_Place4[1]);
                else if (dist_label == 4)
                    OpenMV_Follow(Actual_Place[0], Actual_Place[1], Target_Place1[0], Target_Place1[1]);
                else if (dist_label == 5)
                    OpenMV_Follow(Actual_Place[0], Actual_Place[1], Target_Place1[0], Target_Place1[1]);
            }
            if (num_stop >= 30)
            {
                begin_control = 0;
                // OpenMV_Follow(Actual_Place[0], Actual_Place[1], Actual_Place[0], Actual_Place[1]);
                // printf("finish\r\n");
            }
        }
        // printf("floow\r\n");
        // else
        // {

        // }
    }
}

void Error_Find(void)
{
    float distance1, target_x, target_y;
    // float distance2,next_target_x,next_target_y;
    if ((dist_label == 0) || (dist_label == 4))
    {
        target_x = Target_Place1[0];
        target_y = Target_Place1[1];
        // next_target_x = Target_Place2[0];
        // next_target_y = Target_Place2[1];
    }
    else if (dist_label == 1)
    {
        target_x = Target_Place2[0];
        target_y = Target_Place2[1];
        // next_target_x = Target_Place3[0];
        // next_target_y = Target_Place3[1];
    }
    else if (dist_label == 2)
    {
        target_x = Target_Place3[0];
        target_y = Target_Place3[1];
        // next_target_x = Target_Place4[0];
        // next_target_y = Target_Place4[1];
    }
    else if (dist_label == 3)
    {
        target_x = Target_Place4[0];
        target_y = Target_Place4[1];
        // next_target_x = Target_Place1[0];
        // next_target_y = Target_Place1[1];
    }
    else if (dist_label == 5)
    {
        target_x = Target_Place1[0];
        target_y = Target_Place1[0];
    }
    // printf("target1:%f,%f\r\n",Target_Place1[0],Target_Place1[1]);
    // printf("target:%f,%f\r\n",target_x,target_y);
    distance1 = (target_x - Actual_Place[0]) * (target_x - Actual_Place[0]) + (target_y - Actual_Place[1]) * (target_y - Actual_Place[1]);
    // printf("distance:%f\r\n",distance1);
    // distance2 = (next_target_x - Actual_Place[0])*(next_target_x - Actual_Place[0]) + (next_target_y - Actual_Place[1])*(next_target_y - Actual_Place[1]);
    if ((distance1 <= 5) && (dist_label != 0) && (dist_label != 5))
    {
        dist_label++;
        Integral_bias_X = 0;
        Integral_bias_Y = 0;
    }
    else if ((dist_label == 0) && (distance1 <= 4))
    {
        nums_start++;
        if (nums_start >= 20)
        {
            dist_label++;
            Integral_bias_X = 0;
            Integral_bias_Y = 0;
        }
    }
    else if (dist_label == 5)
    {
        num_stop++;
        Integral_bias_X = 0;
        Integral_bias_Y = 0;
    }
}

/**************************************************************************
Function: Data sliding filtering
Input   : data
Output  : Filtered data
函数功能：数据滑动滤波
入口参数：数据
返回  值：滤波后的数据
**************************************************************************/
float Mean_Filter_Left(float data)
{
    uint8_t i;
    float Sum_Data = 0;
    float Filter_Data;
    static float Speed_Buf[FILTERING_TIMES] = {0};
    for (i = 1; i < FILTERING_TIMES; i++)
    {
        Speed_Buf[i - 1] = Speed_Buf[i];
    }
    Speed_Buf[FILTERING_TIMES - 1] = data;

    for (i = 0; i < FILTERING_TIMES; i++)
    {
        Sum_Data += Speed_Buf[i];
    }
    Filter_Data = (int32_t)(Sum_Data / FILTERING_TIMES);
    return Filter_Data;
}
float Mean_Filter_Right(float data)
{
    uint8_t i;
    float Sum_Data = 0;
    float Filter_Data;
    static float Speed_Buf[FILTERING_TIMES] = {0};
    for (i = 1; i < FILTERING_TIMES; i++)
    {
        Speed_Buf[i - 1] = Speed_Buf[i];
    }
    Speed_Buf[FILTERING_TIMES - 1] = data;

    for (i = 0; i < FILTERING_TIMES; i++)
    {
        Sum_Data += Speed_Buf[i];
    }
    Filter_Data = (int32_t)(Sum_Data / FILTERING_TIMES);
    return Filter_Data;
}
/**************************************************************************
Function: Limit
Input   : IN;max;min
Output  : OUT
函数功能：各种限幅函数
入口参数: IN：输入参数  max：限幅最大值  min：限幅最小值
返回  值：限幅后的值
**************************************************************************/
float PWM_Limit(float IN, int max, int min)
{
    float OUT = IN;
    if (OUT > max)
        OUT = max;
    if (OUT < min)
        OUT = min;
    return OUT;
}
float target_limit_float(float insert, float low, float high)
{
    if (insert < low)
        return low;
    else if (insert > high)
        return high;
    else
        return insert;
}
int target_limit_int(int insert, int low, int high)
{
    if (insert < low)
        return low;
    else if (insert > high)
        return high;
    else
        return insert;
}
