#ifndef __OPENMV_H
#define __OPENMV_H

#include "header.h"

void OpenMV_For_Line(void);
void Send_For_OpenMV(uint8_t label);
void OpenMV_For_Data(void);
void OpenMV_Follow(float current_x,float current_y,float target_x,float target_y );
int x_linearity(float current_x,float target_x);
int y_linearity(float current_y,float target_y);
int Limited(uint16_t target, float Limited_Min,float Limited_Max);
float distance_square(float target_x,float target_y,float current_x,float current_y);


extern uint16_t target_x,target_y,dist_label;
extern float Move_X,Move_Y;
extern uint8_t label_receive_atlas_out, label_receive_atlas_in, label_enough_atlas;
extern float X_Limited_Min,X_Limited_Max;
extern float Y_Limited_Min,Y_Limited_Max;
extern uint16_t n;

#endif
