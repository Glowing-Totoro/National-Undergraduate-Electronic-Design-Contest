#ifndef __BSP_SERVO_H
#define __BSP_SERVO_H
#include "header.h"


#define pwm_x TIM1->CCR1
#define pwm_y TIM1->CCR4

#define up      1
#define down    2
#define left    3
#define right   4

void Servo(float pwmx, float pwmy);
void Move(uint8_t x_y, float angle);


#endif


