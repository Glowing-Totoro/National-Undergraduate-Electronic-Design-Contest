#include "pid.h"

float Integral_bias_X,Integral_bias_Y;
float intex = 600;
float intey = 600;
/* �ٶȻ�PID��������(����ʽ) */
float Velocity_KP = 100,Velocity_KI = 150;
/* ת��OpenMV��������(λ��ʽ) */
float OpenMV_KP = 0.01,OpenMV_KI = 0.000,OpenMV_KD = 0.03;
/* ���뻷OpenMV��������(����ʽ) */
float OpenMV_Dist_KP_X = 0.000001,OpenMV_Dist_KI_X = 0.07 ,OpenMV_Dist_KD_X = 0.000;
float OpenMV_Dist_KP_Y = - 0.0000001,OpenMV_Dist_KI_Y = 0.02 ,OpenMV_Dist_KD_Y = 0.000;
/* ����OpenMV��������(����ʽ) */
// float OpenMV_Follow_KP_X = 0.0000,OpenMV_Follow_KI_X = 0.01 ,OpenMV_Follow_KD_X = 0.000;
// float OpenMV_Follow_KP_Y = 0.0000000,OpenMV_Follow_KI_Y = - 0.02 ,OpenMV_Follow_KD_Y = 0.000;

/* �ĵ�ʽ��pid���� */
// float OpenMV_Follow_KP_X = - 0.00001,OpenMV_Follow_KI_X = - 0.008 ,OpenMV_Follow_KD_X = - 0.000002;
// float OpenMV_Follow_KP_Y = - 0.000012,OpenMV_Follow_KI_Y = - 0.0094 ,OpenMV_Follow_KD_Y = - 0.0000025;

/* ��㸴��pid���� */
//  float OpenMV_Follow_KP_X = - 0.0001,OpenMV_Follow_KI_X = - 0.03 ,OpenMV_Follow_KD_X = - 0.000;
//  float OpenMV_Follow_KP_Y = - 0.00012,OpenMV_Follow_KI_Y = - 0.05 ,OpenMV_Follow_KD_Y = - 0.000;
// /* �����Ǿɵ�˼· */
/* ���²��� */
// float OpenMV_Follow_KP_X = - 0.000000,OpenMV_Follow_KI_X = - 0.07 ,OpenMV_Follow_KD_X = - 0.0000001;
// float OpenMV_Follow_KP_Y = - 0.000000,OpenMV_Follow_KI_Y = - 0.09 ,OpenMV_Follow_KD_Y = - 0.00000012;
/* �µ�˼· */
// float OpenMV_Follow_KP_X = - 0.00000,OpenMV_Follow_KI_X = - 0.2 ,OpenMV_Follow_KD_X  = - 0.0000001;
// float OpenMV_Follow_KP_Y = - 0.00000,OpenMV_Follow_KI_Y = - 0.25, OpenMV_Follow_KD_Y = - 0.0000001;
float OpenMV_Follow_KP_X,OpenMV_Follow_KI_X,OpenMV_Follow_KD_X;
float OpenMV_Follow_KP_Y,OpenMV_Follow_KI_Y, OpenMV_Follow_KD_Y;
/* ���Էֶ� */
// float OpenMV_Follow_KP_X = - 0.000002,OpenMV_Follow_KI_X = - 0.04 ,OpenMV_Follow_KD_X = - 0.000001;
// float OpenMV_Follow_KP_Y = - 0.000002,OpenMV_Follow_KI_Y = - 0.0205,OpenMV_Follow_KD_Y = - 0.0000013;
/* KD�Ӵ������Ŷ� */
/* 25 */
// float OpenMV_Follow_KP_X = - 0.0000000,OpenMV_Follow_KI_X = - 0.1 ,OpenMV_Follow_KD_X = - 0.000001;
// float OpenMV_Follow_KP_Y = - 0.0000000,OpenMV_Follow_KI_Y = - 0.13 ,OpenMV_Follow_KD_Y = - 0.0000013;



/* �޷� */
// float X_Limited_Min = 1785,X_Limited_Max = 1965;
// float Y_Limited_Min = 1622,Y_Limited_Max = 1911;
float X_Limited_Min = 1755,X_Limited_Max = 1995;
float Y_Limited_Min = 1542,Y_Limited_Max = 1897;

/**************************************************************************
Function: OpenMV_Distance_PID
Input   : Current_Value;Target_Value
Output  : OutPut
�������ܣ�OpenMV����PID
��ڲ���: ��ǰOpenMV����ֵ��Ŀ��ֵ
**************************************************************************/	 	
float OpenMV_Distance_PID_X(float Current_Value,float Target_Value )
{
    /* δʶ�����������Ĭ�ϼ��� */
    static float Bias,OutPut,Integral_bias,Last_Bias;
    Bias=Target_Value-Current_Value;                         // ����ƫ��
    Integral_bias+=Bias;                                     // ƫ�����
    if(Integral_bias>5000) Integral_bias=5000;
    else if(Integral_bias<-5000) Integral_bias=-5000;
    OutPut+=OpenMV_Dist_KI_X*Bias+OpenMV_Dist_KD_X*Integral_bias+OpenMV_Dist_KP_X*(Bias-Last_Bias);
    Last_Bias=Bias;                                          // ������һ��ƫ��
//    if(MotorA.Motor_Pwm == 0 && MotorB.Motor_Pwm == 0)       // ����ر� ��������
//    Integral_bias = 0;
    if (OutPut > X_Limited_Max)   OutPut = X_Limited_Max;
    else if (OutPut < X_Limited_Min)  OutPut = X_Limited_Min;
    return OutPut;                                       
}

float OpenMV_Distance_PID_Y(float Current_Value,float Target_Value )
{

    /* δʶ�����������Ĭ�ϼ��� */
    static float Bias,OutPut,Integral_bias,Last_Bias;
    Bias=Target_Value-Current_Value;                         // ����ƫ��
    Integral_bias+=Bias;                                     // ƫ�����
    if(Integral_bias>intex) Integral_bias=intex;
    else if(Integral_bias<-intex) Integral_bias=-intex;
    OutPut+=OpenMV_Dist_KI_Y*Bias+OpenMV_Dist_KD_Y*Integral_bias+OpenMV_Dist_KP_Y*(Bias-Last_Bias);
    Last_Bias=Bias;                                          // ������һ��ƫ��
//    if(MotorA.Motor_Pwm == 0 && MotorB.Motor_Pwm == 0)       // ����ر� ��������
//    Integral_bias = 0;
    if (OutPut > Y_Limited_Max)   OutPut = Y_Limited_Max;
    else if (OutPut < Y_Limited_Min)  OutPut = Y_Limited_Min;
  
    return OutPut;                                       
}
float OpenMV_Follow_PID_X(float Current_Value,float Target_Value )
{
    /* δʶ�����������Ĭ�ϼ��� */
    static float Bias,Last_Bias;
    static float OutPut = 1875;
    // static float distance;
    Bias=Target_Value-Current_Value;                         // ����ƫ��
    Integral_bias_X+=Bias;                                     // ƫ�����
    if(Integral_bias_X>500) Integral_bias_X=500;
    else if(Integral_bias_X<-500) Integral_bias_X=-500;
    OutPut+=OpenMV_Follow_KI_X*Bias+OpenMV_Follow_KD_X*Integral_bias_X+OpenMV_Follow_KP_X*(Bias-Last_Bias);
    Last_Bias=Bias;                                          // ������һ��ƫ��
//    if(MotorA.Motor_Pwm == 0 && MotorB.Motor_Pwm == 0)       // ����ر� ��������
//    Integral_bias = 0;
    // distance = (Target_Value - Current_Value)*(Target_Value - Current_Value);
    //  if (distance < 4) Integral_bias = 0;
    if (OutPut > X_Limited_Max)   OutPut = X_Limited_Max;
    else if (OutPut < X_Limited_Min)  OutPut = X_Limited_Min;
    return OutPut;                                       
}

float OpenMV_Follow_PID_Y(float Current_Value,float Target_Value )
{
    /* δʶ�����������Ĭ�ϼ��� */
    static float Bias,Last_Bias;
    static float OutPut = 1772;
    // static float distance;
    Bias=Target_Value-Current_Value;                         // ����ƫ��
    Integral_bias_Y+=Bias;                                     // ƫ�����
    if(Integral_bias_Y>intey) Integral_bias_Y=intey;
    else if(Integral_bias_Y<-intey) Integral_bias_Y=-intey;
    OutPut+=OpenMV_Follow_KI_Y*Bias+OpenMV_Follow_KD_Y*Integral_bias_Y+OpenMV_Follow_KP_Y*(Bias-Last_Bias);
    Last_Bias=Bias;                                          // ������һ��ƫ��
//    if(MotorA.Motor_Pwm == 0 && MotorB.Motor_Pwm == 0)       // ����ر� ��������
//    Integral_bias = 0;
    // distance = (Target_Value - Current_Value)*(Target_Value - Current_Value);
    //  if (distance < 4) Integral_bias = 0;
    if (OutPut > Y_Limited_Max)   OutPut = Y_Limited_Max;
    else if (OutPut < Y_Limited_Min)  OutPut = Y_Limited_Min;
  
    return OutPut;                                       
}

/**************************************************************************
�������ܣ�����PI������
��ڲ���������������ֵ��Ŀ���ٶ�
����  ֵ�����PWM����ֵ
����ֻʹ��PI����
pwm += Kp[e(k)-e(k-1)]+Ki*e(k)
**************************************************************************/
/* �����ٶȻ�PI */
int Incremental_PI_Left (float Encoder,float Target)
{
    static float Bias,Pwm,Last_bias;
    Bias=Target-Encoder;                                    //����ƫ��
    Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;     //����ʽPI������
    Last_bias=Bias;                                         //������һ��ƫ�� 
    return Pwm;                                             //�������
}
/* �����ٶȻ�PI */
int Incremental_PI_Right (float Encoder,float Target)
{
    static float Bias,Pwm,Last_bias;
    Bias=Target-Encoder;                                    //����ƫ��
    Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;     //����ʽPI������
    Last_bias=Bias;                                         //������һ��ƫ�� 
    return Pwm;                                             //�������
}
