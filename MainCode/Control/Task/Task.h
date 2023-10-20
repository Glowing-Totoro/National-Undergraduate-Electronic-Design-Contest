#ifndef TASK_H
#define TASK_H

#include "bsp_servo.h"
#include "./SYSTEM/delay/delay.h"
#include "control.h"
#include "pid.h"
#include "openmv.h"


void Task_Select(uint8_t Task_flag);
void Task1(void);
void Task2(void);
void Task3(void);
void Task4(void);



#endif

