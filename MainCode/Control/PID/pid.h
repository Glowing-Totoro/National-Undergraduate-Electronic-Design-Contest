#ifndef __PID_H
#define __PID_H

#include "header.h"

extern float OpenMV_Follow_KP_X,OpenMV_Follow_KI_X,OpenMV_Follow_KD_X;
extern float OpenMV_Follow_KP_Y,OpenMV_Follow_KI_Y, OpenMV_Follow_KD_Y;
extern float Integral_bias_X,Integral_bias_Y;
extern float intex,intey;


int Incremental_PI_Left (float Encoder,float Target);
int Incremental_PI_Right (float Encoder,float Target);
float OpenMV_Distance_PID_X(float Current_Value,float Target_Value);
float OpenMV_Distance_PID_Y(float Current_Value,float Target_Value);
float OpenMV_Follow_PID_X(float Current_Value,float Target_Value );
float OpenMV_Follow_PID_Y(float Current_Value,float Target_Value );
#endif
