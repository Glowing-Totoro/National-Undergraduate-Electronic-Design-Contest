#ifndef __SHOW_H
#define    __SHOW_H


#include "Header.h"


#define CHANNEL_NUMBER    3                //ʾ����ͨ���������������ʾͨ����


void Show(void);
void Car_Select_Show(void);

uint16_t myabs(int Input);
void DataScope(void);
void APP_Show(void);
void OLED_DrawPoint_Shu(uint8_t x,uint8_t y,uint8_t t);
void OLED_Show_CCD(void);


#endif
