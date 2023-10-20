#ifndef __CONTROL_H
#define __CONTROL_H

#include "header.h"
#include "openmv.h"
#include "pid.h"

#define Pi                                  3.1415926523f   // 圆周率
#define Angle_To_Rad                        57.295779513f   // 角度制转弧度制，除以这个参数
#define FILTERING_TIMES                     2
#define Frequency                           200

extern float Move_X,Move_Z;
extern float OpenMV_Move_X;

void Min_Control_5ms(void);
void Key_Press_Pause(void);
float Mean_Filter_Left(float data);
float Mean_Filter_Right(float data);
float PWM_Limit(float IN,int max,int min);
float target_limit_float(float insert,float low,float high);
int target_limit_int(int insert,int low,int high);
void Error_Find(void);


#endif

