#ifndef __OLED_H
#define __OLED_H                   
#include "header.h"







//-----------------OLED端口定义---------------- 
#define OLED_RST_Clr() PDout(7)=0   //RST
#define OLED_RST_Set() PDout(7)=1   //RST

#define OLED_RS_Clr() PDout(6)=0    //DC
#define OLED_RS_Set() PDout(6)=1    //DC

#define OLED_SCLK_Clr()  PBout(9)=0  //SCL
#define OLED_SCLK_Set()  PBout(9)=1   //SCL

#define OLED_SDIN_Clr()  PBout(8)=0   //SDA
#define OLED_SDIN_Set()  PBout(8)=1   //SDA

#define OLED_CMD  0    //写命令
#define OLED_DATA 1    //写数据





extern uint8_t OLED_GRAM[128][8];     

//OLED控制用函数
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);        
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);                                          
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void OLED_ShowNumber(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p);    
void OLED_Refresh_GRAM_Line(uint8_t line);

#endif  
     
