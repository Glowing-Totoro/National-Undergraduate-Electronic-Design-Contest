#include "openmv.h"

uint16_t len_mv;
uint16_t target_x, target_y, dist_label;
int16_t bc_x, bc_y;
float Move_X, Move_Y;
uint8_t atalas_out[4] = {0x09, 0x0a, 0x0d};
uint8_t atalas_in[4] = {0x05, 0x0a, 0x0d};
uint8_t light[4] = {0x07, 0x0a, 0x0d};
uint8_t label_receive_atlas_out, label_receive_atlas_in, label_enough_atlas;
uint8_t anyhowever = 1;
float x_inteval = 2, y_inteval = 2;
uint16_t n = 10;
float p = 0.47;
/* OpenMV Ѳ�ߴ���ƫ��ֵ */
void OpenMV_For_Line(void)
{
    /* ���ڽ������ݲ��� */
    if (g_usart4_rx_sta & 0x8000) /* ���յ�������? */
    {
        len_mv = g_usart4_rx_sta & 0x3fff;                            /* �õ��˴ν��յ������ݳ��� */
        if (g_usart4_rx_buf[0] == 0xb3 && g_usart4_rx_buf[1] == 0xb3) /* ������ЧУ�� */
        {
            /* ����㴫�� */
            for (int ii = 2; ii < 10; ii++)
            {
                if (g_usart4_rx_buf[ii + 8] != 0)
                {
                    OpenMV_Data[ii - 2] = g_usart4_rx_buf[ii] + 256;
                }
                else
                {
                    OpenMV_Data[ii - 2] = g_usart4_rx_buf[ii];
                }
            }
        }
        g_usart4_rx_sta = 0;
        /* �жϴ�ʱ����λ����ȷ��Ŀ��λ�� */
        if (dist_label == 0) // ��ʼ �ƶ�����ʼ��
        {
            target_x = OpenMV_Data[6];
            target_y = OpenMV_Data[7];
        }
        else if (dist_label == 1)
        {
            target_x = OpenMV_Data[4];
            target_y = OpenMV_Data[5];
        }
        else if (dist_label == 2)
        {
            target_x = OpenMV_Data[2];
            target_y = OpenMV_Data[3];
        }
        else if (dist_label == 3)
        {
            target_x = OpenMV_Data[0];
            target_y = OpenMV_Data[1];
        }
        else if (dist_label == 4) // ׼���س�ʼ��
        {
            target_x = OpenMV_Data[6];
            target_y = OpenMV_Data[7];
        }
        else if (dist_label == 5) // ���������
        {
            /* ���޸��������� */
            target_x = OpenMV_Data[6];
            target_y = OpenMV_Data[7];
        }
    }
    /* ������������� */
    if (target_x != 0 && target_y != 0)
    {
        if (Task_flag2 == 1)
        {
            /* ���뻷PID */
            if (dist_label == 0 || dist_label == 4 || dist_label == 5)
            {
                bc_x = 0;
                bc_y = -13;
            }
            else if (dist_label == 1)
            {
                bc_x = 3;
                bc_y = -13;
            }
            else if (dist_label == 2)
            {
                bc_x = 3;
                bc_y = -12;
            }
            else if (dist_label == 3)
            {
                bc_x = 0;
                bc_y = -12;
            }
            Move_X = OpenMV_Distance_PID_X(target_x, 80 + bc_x);
            Move_Y = OpenMV_Distance_PID_Y(target_y, 60 + bc_y);
        }
        else if (Task_flag3 == 1)
        {
            Move_X = OpenMV_Distance_PID_X(target_x, 80);
            Move_Y = OpenMV_Distance_PID_Y(target_y, 60 - 13);
        }
        /* ������Ч */
        Servo(Move_X, Move_Y);
    }
}
/* Ѱ��OpenMVʶ���ͼ���� */
void Send_For_OpenMV(uint8_t label)
{
    if (label != 0)
    {
        label_receive_atlas_out = 0;
        while (1)
        {
            HAL_UART_Transmit(&g_uart4_handle, atalas_out, 3, 1000); /* �������� */
            while (__HAL_UART_GET_FLAG(&g_uart4_handle, UART_FLAG_TC) != SET)
                ; /* �ȴ����ͽ��� */

            delay_ms(300);
            if (label_receive_atlas_out != 0)
            {
                break;
            }
        }
        while (label_enough_atlas == 0)
            ;
        label_enough_atlas = 0;
        label_receive_atlas_in = 0;
        while (1)
        {
            HAL_UART_Transmit(&g_uart4_handle, atalas_in, 3, 1000); /* �������� */
            while (__HAL_UART_GET_FLAG(&g_uart4_handle, UART_FLAG_TC) != SET)
                ; /* �ȴ����ͽ��� */
            delay_ms(300);
            if (label_receive_atlas_in != 0)
            {
                break;
            }
        }
    }
}

void OpenMV_For_Data(void)
{
    /* ���ڽ������ݲ��� */
    if (g_usart4_rx_sta & 0x8000) /* ���յ�������? */
    {
        len_mv = g_usart4_rx_sta & 0x3fff;                            /* �õ��˴ν��յ������ݳ��� */
        if (g_usart4_rx_buf[0] == 0xb3 && g_usart4_rx_buf[1] == 0xb3) /* ������ЧУ�� */
        {
            /* ��ͼʶ�� & ��� */
            if (g_usart4_rx_buf[18] == 0x01 && label_enough_atlas == 0 && anyhowever == 1)
            {
                /* ����㴫�� */
                for (int ii = 2; ii < 10; ii++)
                {
                    if (g_usart4_rx_buf[ii + 8] != 0)
                    {
                        OpenMV_Data[ii - 2] = g_usart4_rx_buf[ii] + 256;
                    }
                    else
                    {
                        OpenMV_Data[ii - 2] = g_usart4_rx_buf[ii];
                    }
                }
                g_usart4_rx_buf[18] = 0;
                num_receive++;
                label_receive_atlas_out = 1;
                Target_Place1_Out[0] += OpenMV_Data[6];
                Target_Place1_Out[1] += OpenMV_Data[7];
                Target_Place2_Out[0] += OpenMV_Data[4];
                Target_Place2_Out[1] += OpenMV_Data[5];
                Target_Place3_Out[0] += OpenMV_Data[2];
                Target_Place3_Out[1] += OpenMV_Data[3];
                Target_Place4_Out[0] += OpenMV_Data[0];
                Target_Place4_Out[1] += OpenMV_Data[1];
                for (int i = 0; i < 8; i++)
                    OpenMV_Data[i] = 0;
                if (num_receive == 3)
                {
                    anyhowever = 0;
                    label_enough_atlas = 1;
                    Target_Place1_Out[0] /= num_receive;
                    Target_Place1_Out[1] /= num_receive;
                    Target_Place2_Out[0] /= num_receive;
                    Target_Place2_Out[1] /= num_receive;
                    Target_Place3_Out[0] /= num_receive;
                    Target_Place3_Out[1] /= num_receive;
                    Target_Place4_Out[0] /= num_receive;
                    Target_Place4_Out[1] /= num_receive;
                    num_receive = 0;
                }
            }
            /* �ڿ����� */
            else if (g_usart4_rx_buf[18] == 0x02 && label_enough_atlas == 0 && anyhowever == 0)
            {
                /* ����㴫�� */
                for (int ii = 2; ii < 10; ii++)
                {
                    if (g_usart4_rx_buf[ii + 8] != 0)
                    {
                        OpenMV_Data[ii - 2] = g_usart4_rx_buf[ii] + 256;
                    }
                    else
                    {
                        OpenMV_Data[ii - 2] = g_usart4_rx_buf[ii];
                    }
                }
                g_usart4_rx_buf[18] = 0;
                num_receive++;
                label_receive_atlas_in = 1;
                Target_Place1_In[0] += OpenMV_Data[6];
                Target_Place1_In[1] += OpenMV_Data[7];
                Target_Place2_In[0] += OpenMV_Data[4];
                Target_Place2_In[1] += OpenMV_Data[5];
                Target_Place3_In[0] += OpenMV_Data[2];
                Target_Place3_In[1] += OpenMV_Data[3];
                Target_Place4_In[0] += OpenMV_Data[0];
                Target_Place4_In[1] += OpenMV_Data[1];
                for (int i = 0; i < 8; i++)
                    OpenMV_Data[i] = 0;
                if (num_receive == 3)
                {
                    label_enough_atlas = 1;
                    Target_Place1_In[0] /= num_receive;
                    Target_Place1_In[1] /= num_receive;
                    Target_Place2_In[0] /= num_receive;
                    Target_Place2_In[1] /= num_receive;
                    Target_Place3_In[0] /= num_receive;
                    Target_Place3_In[1] /= num_receive;
                    Target_Place4_In[0] /= num_receive;
                    Target_Place4_In[1] /= num_receive;

                    if (distance_square(Target_Place1_Out[0], Target_Place1_Out[1], Target_Place2_Out[0], Target_Place2_Out[1]) * distance_square(Target_Place2_Out[0], Target_Place2_Out[1], Target_Place3_Out[0], Target_Place3_Out[1]) <= distance_square(Target_Place1_In[0], Target_Place1_In[1], Target_Place2_In[0], Target_Place2_In[1]) * distance_square(Target_Place2_In[0], Target_Place2_In[1], Target_Place3_In[0], Target_Place3_In[1]))
                    {
                        Target_Place1[0] = Target_Place1_Out[0];
                        Target_Place1[1] = Target_Place1_Out[1];
                        Target_Place2[0] = Target_Place2_Out[0];
                        Target_Place2[1] = Target_Place2_Out[1];
                        Target_Place3[0] = Target_Place3_Out[0];
                        Target_Place3[1] = Target_Place3_Out[1];
                        Target_Place4[0] = Target_Place4_Out[0];
                        Target_Place4[1] = Target_Place4_Out[1];
                    }
                    else
                    {

                        float dymac_x[4], dymac_y[4], dydist, dydist2;
                        uint8_t num_with = 0;
                        dymac_x[0] = Target_Place1_In[0];
                        dymac_x[1] = Target_Place2_In[0];
                        dymac_x[2] = Target_Place3_In[0];
                        dymac_x[3] = Target_Place4_In[0];
                        dymac_y[0] = Target_Place1_In[1];
                        dymac_y[1] = Target_Place2_In[1];
                        dymac_y[2] = Target_Place3_In[1];
                        dymac_y[3] = Target_Place4_In[1];
                        /* ��ʼ��ȷ�� */
                        dydist = distance_square(Target_Place1_Out[0], Target_Place1_Out[1], Target_Place1_In[0], Target_Place1_In[1]);
                        num_with = 0;
                        dydist2 = distance_square(Target_Place1_Out[0], Target_Place1_Out[1], Target_Place2_In[0], Target_Place2_In[1]);
                        if (dydist > dydist2)
                        {
                            dydist = dydist2;
                            num_with = 1;
                        }
                        dydist2 = distance_square(Target_Place1_Out[0], Target_Place1_Out[1], Target_Place3_In[0], Target_Place3_In[1]);
                        if (dydist > dydist2)
                        {
                            dydist = dydist2;
                            num_with = 2;
                        }
                        dydist2 = distance_square(Target_Place1_Out[0], Target_Place1_Out[1], Target_Place4_In[0], Target_Place4_In[1]);
                        if (dydist > dydist2)
                        {
                            dydist = dydist2;
                            num_with = 3;
                        }
                        Target_Place1[0] = p * Target_Place1_Out[0] + (1 - p) * dymac_x[num_with];
                        Target_Place1[1] = p * Target_Place1_Out[1] + (1 - p) * dymac_y[num_with];

                        dydist = distance_square(Target_Place2_Out[0], Target_Place2_Out[1], Target_Place1_In[0], Target_Place1_In[1]);
                        num_with = 0;
                        dydist2 = distance_square(Target_Place2_Out[0], Target_Place2_Out[1], Target_Place2_In[0], Target_Place2_In[1]);
                        if (dydist > dydist2)
                        {
                            dydist = dydist2;
                            num_with = 1;
                        }
                        dydist2 = distance_square(Target_Place2_Out[0], Target_Place2_Out[1], Target_Place3_In[0], Target_Place3_In[1]);
                        if (dydist > dydist2)
                        {
                            dydist = dydist2;
                            num_with = 2;
                        }
                        dydist2 = distance_square(Target_Place2_Out[0], Target_Place2_Out[1], Target_Place4_In[0], Target_Place4_In[1]);
                        if (dydist > dydist2)
                        {
                            dydist = dydist2;
                            num_with = 3;
                        }
                        Target_Place2[0] = p * Target_Place2_Out[0] + (1 - p) * dymac_x[num_with];
                        Target_Place2[1] = p * Target_Place2_Out[1] + (1 - p) * dymac_y[num_with];

                        dydist = distance_square(Target_Place3_Out[0], Target_Place3_Out[1], Target_Place1_In[0], Target_Place1_In[1]);
                        num_with = 0;
                        dydist2 = distance_square(Target_Place3_Out[0], Target_Place3_Out[1], Target_Place2_In[0], Target_Place2_In[1]);
                        if (dydist > dydist2)
                        {
                            dydist = dydist2;
                            num_with = 1;
                        }
                        dydist2 = distance_square(Target_Place3_Out[0], Target_Place3_Out[1], Target_Place3_In[0], Target_Place3_In[1]);
                        if (dydist > dydist2)
                        {
                            dydist = dydist2;
                            num_with = 2;
                        }
                        dydist2 = distance_square(Target_Place3_Out[0], Target_Place3_Out[1], Target_Place4_In[0], Target_Place4_In[1]);
                        if (dydist > dydist2)
                        {
                            dydist = dydist2;
                            num_with = 3;
                        }
                        Target_Place3[0] = p * Target_Place3_Out[0] + (1 - p) * dymac_x[num_with];
                        Target_Place3[1] = p * Target_Place3_Out[1] + (1 - p) * dymac_y[num_with];

                        dydist = distance_square(Target_Place4_Out[0], Target_Place4_Out[1], Target_Place1_In[0], Target_Place1_In[1]);
                        num_with = 0;
                        dydist2 = distance_square(Target_Place4_Out[0], Target_Place4_Out[1], Target_Place2_In[0], Target_Place2_In[1]);
                        if (dydist > dydist2)
                        {
                            dydist = dydist2;
                            num_with = 1;
                        }
                        dydist2 = distance_square(Target_Place4_Out[0], Target_Place4_Out[1], Target_Place3_In[0], Target_Place3_In[1]);
                        if (dydist > dydist2)
                        {
                            dydist = dydist2;
                            num_with = 2;
                        }
                        dydist2 = distance_square(Target_Place4_Out[0], Target_Place4_Out[1], Target_Place4_In[0], Target_Place4_In[1]);
                        if (dydist > dydist2)
                        {
                            dydist = dydist2;
                            num_with = 3;
                        }
                        Target_Place4[0] = p * Target_Place4_Out[0] + (1 - p) * dymac_x[num_with];
                        Target_Place4[1] = p * Target_Place4_Out[1] + (1 - p) * dymac_y[num_with];

                        printf("Cal Finished!\r\n");

                        /* ʶ��˳�����ƫ�� ������ */
                        // Target_Place1[0] = (Target_Place1_Out[0] + Target_Place1_In[0]) / 2;
                        // Target_Place1[1] = (Target_Place1_Out[1] + Target_Place1_In[1]) / 2;
                        // Target_Place2[0] = (Target_Place2_Out[0] + Target_Place2_In[0]) / 2;
                        // Target_Place2[1] = (Target_Place2_Out[1] + Target_Place2_In[1]) / 2;
                        // Target_Place3[0] = (Target_Place3_Out[0] + Target_Place3_In[0]) / 2;
                        // Target_Place3[1] = (Target_Place3_Out[1] + Target_Place3_In[1]) / 2;
                        // Target_Place4[0] = (Target_Place4_Out[0] + Target_Place4_In[0]) / 2;
                        // Target_Place4[1] = (Target_Place4_Out[1] + Target_Place4_In[1]) / 2;
                    }

                    HAL_UART_Transmit(&g_uart4_handle, light, 3, 1000); /* �������� */
                    while (__HAL_UART_GET_FLAG(&g_uart4_handle, UART_FLAG_TC) != SET)
                        ; /* �ȴ����ͽ��� */
                    delay_ms(5);
                    HAL_UART_Transmit(&g_uart4_handle, light, 3, 1000); /* �������� */
                    while (__HAL_UART_GET_FLAG(&g_uart4_handle, UART_FLAG_TC) != SET)
                        ; /* �ȴ����ͽ��� */
                    delay_ms(5);
                    HAL_UART_Transmit(&g_uart4_handle, light, 3, 1000); /* �������� */
                    while (__HAL_UART_GET_FLAG(&g_uart4_handle, UART_FLAG_TC) != SET)
                        ; /* �ȴ����ͽ��� */
                    delay_ms(5);
                    HAL_UART_Transmit(&g_uart4_handle, light, 3, 1000); /* �������� */
                    while (__HAL_UART_GET_FLAG(&g_uart4_handle, UART_FLAG_TC) != SET)
                        ; /* �ȴ����ͽ��� */
                    delay_ms(5);
                    HAL_UART_Transmit(&g_uart4_handle, light, 3, 1000); /* �������� */
                    while (__HAL_UART_GET_FLAG(&g_uart4_handle, UART_FLAG_TC) != SET)
                        ; /* �ȴ����ͽ��� */
                    delay_ms(5);
                    HAL_UART_Transmit(&g_uart4_handle, light, 3, 1000); /* �������� */
                    while (__HAL_UART_GET_FLAG(&g_uart4_handle, UART_FLAG_TC) != SET)
                        ; /* �ȴ����ͽ��� */
                }
            }
            /* ����ʶ�� & Ѳ�� */
            else if (g_usart4_rx_buf[18] == 0x03 && label_enough_atlas == 1)
            {
                led(1);
                begin_control = 1;
                /* ����㴫��(ֻ����һ������) */
                for (int ii = 2; ii < 4; ii++)
                {
                    if (g_usart4_rx_buf[ii + 8] != 0)
                    {
                        OpenMV_Data[ii - 2] = g_usart4_rx_buf[ii] + 256;
                    }
                    else
                    {
                        OpenMV_Data[ii - 2] = g_usart4_rx_buf[ii];
                    }
                }
                Actual_Place[0] = OpenMV_Data[0];
                Actual_Place[1] = OpenMV_Data[1];
                /* �����������Ŀ����߼� */
                // if (label_enough_atlas != 0)
                // {
                //     if (n == 0)
                //     {
                //         dist_label++;
                //         n = 10;
                //     }
                //     if (dist_label == 0)
                //         OpenMV_Follow(Actual_Place[0], Actual_Place[1], Target_Place1[0], Target_Place1[1]);
                //     else if (dist_label == 1)
                //         OpenMV_Follow(Actual_Place[0], Actual_Place[1], Target_Place1[0], Target_Place1[1]);
                //     else if (dist_label == 2)
                //         OpenMV_Follow(Actual_Place[0], Actual_Place[1], Target_Place1[0], Target_Place1[1]);
                //     else if (dist_label == 3)
                //         OpenMV_Follow(Actual_Place[0], Actual_Place[1], Target_Place1[0], Target_Place1[1]);
                //     else if (dist_label == 4)
                //         OpenMV_Follow(Actual_Place[0], Actual_Place[1], Target_Place1[0], Target_Place1[1]);
                //     else if (dist_label == 5)
                //         OpenMV_Follow(Actual_Place[0], Actual_Place[1], Target_Place1[0], Target_Place1[1]);
                // }
            }
        }
        g_usart4_rx_sta = 0;
    }
}

void OpenMV_Follow(float current_x, float current_y, float target_x, float target_y)
{
    /* ϸ�ֶ��� & ϸ������ */
    uint16_t num_inteval;
    static float inteval_x, inteval_y, temp_target_x, temp_target_y;
    /* ��·��Ŀ��ı� ����·��ϸ�ִ��� */
    static float last_target_x, last_target_y;
    if (Task_flag3 != 0)
    {
        /* ϸ�ֶ��� */
        num_inteval = 15;
        /* ϸ��ֵ���� */
        inteval_x = (target_x - current_x) / num_inteval;
        if (inteval_x >= 1)
            inteval_x = 1;
        if (inteval_x <= -1)
            inteval_x = -1;
        inteval_y = (target_y - current_y) / num_inteval;
        if (inteval_y >= 0.75)
            inteval_y = 0.75;
        if (inteval_y <= -0.75)
            inteval_y = -0.75;

        temp_target_x = current_x + inteval_x;
        temp_target_y = current_y + inteval_y;
    }
    else if (Task_flag4 != 0)
    {
        if (((target_x - last_target_x) * (target_x - last_target_x) + (target_y - last_target_y) * (target_y - last_target_y) >= 600))
        {
            /* ϸ�ֶ��� */
            num_inteval = 25;
            /* ϸ��ֵ���� */
            inteval_x = (target_x - current_x) / num_inteval;
            inteval_y = (target_y - current_y) / num_inteval;
            /* ��ʱĿ�����(�׶θ���) �ع鵱ǰֵ */
            temp_target_x = current_x;
            temp_target_y = current_y;
            /* ���µ�ǰĿ�� */
            last_target_x = target_x;
            last_target_y = target_y;
        }

        /* ��˼· �ݹ��ж���ʱĿ��� */
        float current_distance, target_distance, last_target_distance;
        uint16_t num_break = 0;
        current_distance = distance_square(target_x, target_y, current_x, current_y);
        target_distance = distance_square(target_x, target_y, temp_target_x, temp_target_y);
        last_target_distance = 100000;

        if (current_distance <= target_distance)
        {
            num_break = 0;
            while (1)
            {
                num_break++;
                if (num_break > 100)
                {
                    printf("Error Here:%d\r\n", num_break);
                }
                temp_target_x += inteval_x;
                temp_target_y += inteval_y;
                target_distance = distance_square(target_x, target_y, temp_target_x, temp_target_y);
                if ((target_distance + 0.0001) > last_target_distance)
                {
                    break;
                }
                last_target_distance = target_distance;
                if (target_distance <= current_distance)
                {
                    break;
                }
            }
        }
    }

    // /* ǰ�汾˼· �Ѹ� */
    // /* ����ʱĿ������Сʱȷ����ʱĿ��� */
    // if ((temp_target_x-current_x)*(temp_target_x-current_x) + (temp_target_y-current_y)*(temp_target_y-current_y) <= 5)
    // {
    //     temp_target_x += inteval_x;
    //     temp_target_y += inteval_y;
    //     printf("next level!\r\n");
    // }

    /* ����ʵʩ */
    Move_X = OpenMV_Follow_PID_X(current_x, temp_target_x);
    Move_Y = OpenMV_Follow_PID_Y(current_y, temp_target_y);
    Servo(Move_X, Move_Y);

    /* ��pid�������(������) */
    // uint16_t x_target, y_target;
    // x_target = x_linearity(current_x, target_x);
    // y_target = y_linearity(current_y, target_y);
    // n--;
    // if (current_x < target_x)
    // {
    //     pwm_x = Limited(pwm_x+x_target,X_Limited_Min,X_Limited_Max);
    // }
    // if (current_y < target_y)
    // {
    //     pwm_y = Limited(pwm_y+y_target,Y_Limited_Min,Y_Limited_Max);
    // }

    // /* ���ܵ���ջ�(����) */
    // Move_X = OpenMV_Follow_PID_X(current_x,target_x);
    // Move_Y = OpenMV_Follow_PID_Y(current_y,target_y);
    // Servo(Move_X, Move_Y);
}

/* ����ǰ��Ŀ��x�������PWMƫ�� */
int x_linearity(float current_x, float target_x)
{
    int inteval;
    inteval = ((target_x - current_x) / n) * x_inteval;
    return inteval;
}

/* ����ǰ��Ŀ��y�������PWMƫ�� */
int y_linearity(float current_y, float target_y)
{
    int inteval;
    inteval = ((target_y - current_y) / n) * y_inteval;
    return inteval;
}

int Limited(uint16_t target, float Limited_Min, float Limited_Max)
{
    int OutPut;
    OutPut = target;
    if (target > Limited_Max)
        OutPut = Limited_Max;
    else if (target < Limited_Min)
        OutPut = Limited_Min;
    return OutPut;
}

float distance_square(float target_x, float target_y, float current_x, float current_y)
{
    float distance;
    distance = (target_x - current_x) * (target_x - current_x) + (target_y - current_y) * (target_y - current_y);
    return distance;
}
