#ifndef __HEADER_H
#define __HEADER_H

/* 头文件包含 */

/* 底层库 */
#include "./SYSTEM/sys/sys.h"
#include <string.h>
#include "stdlib.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "math.h"

/*************系统级外设库*************************/
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/EXTI/exti.h"
#include "./BSP/TIMER/generaltimer.h"
#include "./BSP/GPIO/gpio.h"
#include "./BSP/ADC/adc.h"


/**************板载驱动库***************************/
/* oled模块 */
#include "bsp_oled.h"
#include "oledfont.h"
/* 舵机驱动 */
#include "bsp_servo.h"

/**************核心控制逻辑************************/
#include "show.h"       /* 显示 */
#include "control.h"    /* 核心控制逻辑 */
#include "pid.h"        /* pid */
#include "openmv.h"     /* openmv */
#include "Task.h"

/* 通用变量 */
/* openmv数据包 */
extern uint16_t OpenMV_Data[17];
/* 任务标志位 */
extern uint8_t Task_flag1,Task_flag2,Task_flag3,Task_flag4;
/* 接受数据量 & 接受数据累加 */
extern uint8_t num_receive;
 /* 目标位置点 */
extern float Target_Place1_Out[2],Target_Place2_Out[2],Target_Place3_Out[2],Target_Place4_Out[2];
extern float Target_Place1_In[2],Target_Place2_In[2],Target_Place3_In[2],Target_Place4_In[2]; 
extern float Target_Place1[2],Target_Place2[2],Target_Place3[2],Target_Place4[2];
 /* 当前位置点 */
extern float Actual_Place[2];
extern uint8_t begin_control;
extern uint8_t Shut;

//位带操作
//这里只定义了 GPIO ODR和IDR这两个寄存器的位带别名区地址，其他寄存器的没有定义

//SRAM 位带区:    0X2000 0000~0X2010 0000
//SRAM 位带别名区:0X2200 0000~0X23FF FFFF

//外设 位带区:    0X4000 0000~0X4010 0000
//外设 位带别名区:0X4200 0000~0X43FF FFFF

// 把“位带地址+位序号”转换成别名地址的宏
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x02000000+((addr & 0x00FFFFFF)<<5)+(bitnum<<2)) 
/*
 *addr & 0xF0000000，取地址的高4位，看看是2还是4，用于区分SRAM和外设地址，
 *如果是2，+0x02000000则=0X2200 0000，即是SRAM，如果是4，+0x02000000则=0X4200 0000，即是外设
 *
 *addr & 0x000FFFFFF，屏蔽掉高两位，相当于-0X2000 0000或者-0X4000 0000，结果表示偏移位带区多少个字节
 *<<5  等于*8*4，因为位带区一个地址表示一个字节，一个字节有8个bit，一个bit可以膨胀成一个字，即4个字节
 *<<2 等于*4，因为一个位可以膨胀成一个字，即4个字节
 *
 *分解成两条公式应该就是这样：
 *SRAM位带别名地址
 *AliasAddr= 0x22000000+((A-0x20000000)*8+n)*4 =0x22000000+ (A-0x20000000)*8*4 +n*4
 *外设位带别名地址
 *AliasAddr= 0x22000000+((A-0x20000000)*8+n)*4 =0x22000000+ (A-0x20000000)*8*4 +n*4
 */

/* 直接操作寄存器的方法控制IO */
#define digitalHi(p,i)         {p->BSRR=i;}        //输出为高电平        
#define digitalLo(p,i)         {p->BRR=i;}         //输出低电平
#define digitalToggle(p,i)     {p->ODR ^=i;}       //输出反转状态

// 把一个地址转换成一个指针
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 

// 把位带别名区地址转换成指针
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))   

// GPIO ODR 和 IDR 寄存器地址映射 
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C   
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C   
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C   
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C   
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C   
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C      
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C      
  
#define GPIOA_IDR_Addr    (GPIOA_BASE+8)  //0x40010808   
#define GPIOB_IDR_Addr    (GPIOB_BASE+8)  //0x40010C08   
#define GPIOC_IDR_Addr    (GPIOC_BASE+8)  //0x40011008   
#define GPIOD_IDR_Addr    (GPIOD_BASE+8)  //0x40011408   
#define GPIOE_IDR_Addr    (GPIOE_BASE+8)  //0x40011808   
#define GPIOF_IDR_Addr    (GPIOF_BASE+8)  //0x40011A08   
#define GPIOG_IDR_Addr    (GPIOG_BASE+8)  //0x40011E08 



//单独操作 GPIO的某一个IO口，n(0,1,2...16),n表示具体是哪一个IO口
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出   
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入   
  
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出   
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入   
  
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出   
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入   
  
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出   
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入   
  
#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出   
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入  
  
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出   
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入  
  
#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出   
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入  

#endif 
