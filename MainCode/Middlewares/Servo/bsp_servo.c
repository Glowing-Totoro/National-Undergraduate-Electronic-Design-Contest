#include "bsp_servo.h"

float comply_x = 11;
float comply_y = 10;

void Servo(float pwmx, float pwmy)
{
	pwm_x = pwmx;
	pwm_y = pwmy;
}

void Move(uint8_t x_y, float angle)
{
	if (x_y == down) pwm_y -= (int) (comply_y * angle);
	else if (x_y == up) pwm_y += (int) (comply_y * angle);
	else if (x_y == left) pwm_x += (int) (comply_x * angle);
	else if (x_y == right) pwm_x -= (int) (comply_x * angle);
}


