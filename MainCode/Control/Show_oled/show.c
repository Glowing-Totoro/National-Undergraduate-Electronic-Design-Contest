#include "show.h"
/* 临时存放变量 */
float a;
float b;
/**************************************************************************
Function: OLED_Show
Input   : none
Output  : none
函数功能：显示屏显示函数
入口参数: 无 
返回  值：无
**************************************************************************/         
void Show(void)
{
    memset(OLED_GRAM,0, 128*8*sizeof(uint8_t));    //GRAM清零但不立即刷新，防止花屏
    /* 第一行：模式选择与状态显示 */
    OLED_ShowString(0,0,"Mode1:");
    OLED_ShowString(50,0,"Test");
    if (num_receive>=29)  OLED_ShowString(100,0,"Yes");
    else OLED_ShowString(100,0,"No");
    /* 第二行：整体速度(可以此为模板显示两侧速度) */
    OLED_ShowString(0,10,"Speed:");
    a = 1.00;
    uint32_t dymac1;
    if (a < 0){dymac1= -a*100;OLED_ShowString(50,10,"-");}
    else{dymac1 = a*100;OLED_ShowString(50,10,"+");}
    OLED_ShowNumber(58,10,dymac1/100,1,12);
    OLED_ShowString(64,10,".");
    OLED_ShowNumber(70,10,dymac1/10%10,1,12);
    OLED_ShowNumber(76,10,dymac1%10,1,12);    
    OLED_ShowString(84,10,"m/s");
    /* 第三行：行驶里程记录(可仿照记录两侧累计里程) */
    OLED_ShowString(0,20,"Distance:");
    b = 200;
    uint32_t dymac2;
    if (b < 0){dymac2= -b*10;OLED_ShowString(70,20,"-");}
    else{dymac2 = b*10;OLED_ShowString(70,20,"+");}
    OLED_ShowNumber(78,20,dymac2/10,4,12);
    OLED_ShowString(102,20,".");
    OLED_ShowNumber(106,20,dymac2%10,1,12);
    OLED_ShowString(113,20,"cm");
    /* 第四行：任务状态显示 */
    OLED_ShowString(0,30,"Label:");
    OLED_ShowString(50,30,"Loop1_Lb1");
    /* 第五行：自定义(如ccd模式下循迹位置) */
    // OLED_Show_CCD();
    OLED_ShowNumber(20,40,(int)Target_Place1[0],3,12);
    OLED_ShowNumber(50,40,(int)Target_Place1[1],3,12);
    OLED_ShowNumber(80,40,(int)Actual_Place[0],3,12);
    OLED_ShowNumber(100,40,(int)Actual_Place[1],3,12);
    /*第六行：电池电压等系统状态  */
    
    //==================刷新==================//
    OLED_Refresh_Gram();    
}

/**************************************************************************
Function: OLED_Show_CCD
Input   : none
Output  : none
函数功能：CCD模式显示函数，画点
入口参数: 无 
返回  值：无
**************************************************************************/         

void OLED_DrawPoint_Shu(uint8_t x,uint8_t y,uint8_t t)
{ 
    uint8_t i=0;
    OLED_DrawPoint(x,y,t);
    OLED_DrawPoint(x,y,t);
    for(i = 0;i<8; i++)
    {
        OLED_DrawPoint(x,y+i,t);
    }
}

//void OLED_Show_CCD(void)
//{ 
//uint8_t i,t;
//for(i = 0;i<128; i++)
//{
//    if(CCD_ADV[i]<CCD_Threshold) t=1; else t=0;
//    OLED_DrawPoint_Shu(i,0,t);
//}
//}


///**************************************************************************
//Function: Send data to APP
//Input   : none
//Output  : none
//函数功能：向APP发送数据
//入口参数：无
//返回  值：无
//**************************************************************************/
//void APP_Show(void)
//{    
//    static uint8_t flag;
//    int Velocity_Left_Show,Velocity_Right_Show,Voltage_Show;
//    Voltage_Show=(Voltage-1000)*2/3;            if(Voltage_Show<0)Voltage_Show=0;if(Voltage_Show>100) Voltage_Show=100;   //对电压数据进行处理
//    Velocity_Right_Show=MotorB.Velocity*1.1;     if(Velocity_Right_Show<0) Velocity_Right_Show=-Velocity_Right_Show;              //对编码器数据就行数据处理便于图形化
//    Velocity_Left_Show= MotorA.Velocity*1.1;      if(Velocity_Left_Show<0) Velocity_Left_Show=-Velocity_Left_Show;
//    flag=!flag;
//    if(PID_Send==1)            //发送PID参数,在APP调参界面显示
//    {
//         printf("{C%d:%d:%d:%d:%d:%d:%d:%d:%d}$",(int)RC_Velocity,(int)CCD_Move_X,(int)0,(int)0,(int)0,(int)0,(int)0,0,0);//打印到APP上面
//         PID_Send=0;        
//  }
//   else    if(flag==0)        // 发送电池电压，速度，角度等参数，在APP首页显示
//       printf("{A%d:%d:}$",(int)Voltage_Show,(int)0); //打印到APP上面
//     else                //发送小车姿态角，在波形界面显示
//        printf("{B%d:%d}$",(int)MotorA.Current_Encoder,(int)MotorB.Current_Encoder); //显示左右编码器的速度                                                                       //可按格式自行增加显示波形，最多可显示五个
//}


////上位机示波器
///**************************************************************************
//函数功能：虚拟示波器往上位机发送数据 关闭显示屏
//入口参数：无
//返回  值：无
//作    者：平衡小车之家
//**************************************************************************/
//void DataScope(void)
//{   
//    uint8_t i;            //计数变量
//    uint8_t Send_Count;    //串口需要发送的数据个数
//    float Vol;        //电压变量
//    Vol=(float)Voltage/100;
//    DataScope_Get_Channel_Data( MotorA.Velocity, 1 );         //显示左轮速度，单位mm/s
//    DataScope_Get_Channel_Data( MotorB.Velocity, 2 );        //显示右轮速度
//    DataScope_Get_Channel_Data( Vol, 3 );                   //显示电池电压 单位：V
////    DataScope_Get_Channel_Data(0, 5 ); //用您要显示的数据替换0就行了
////    DataScope_Get_Channel_Data(0 , 6 );//用您要显示的数据替换0就行了
////    DataScope_Get_Channel_Data(0, 7 );
////    DataScope_Get_Channel_Data( 0, 8 ); 
////    DataScope_Get_Channel_Data(0, 9 );  
////    DataScope_Get_Channel_Data( 0 , 10);
//    Send_Count = DataScope_Data_Generate(CHANNEL_NUMBER);//CHANNEL_NUMBER可改变通道数量，目前是4
//    for( i = 0 ; i < Send_Count; i++) 
//    {
//        while((USART1->SR&0X40)==0);  
//        USART1->DR = DataScope_OutPut_Buffer[i]; 
//    }
//}

/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回值：uint
**************************************************************************/
uint16_t myabs(int Input)
{
    int Output;
    if(Input > 0)
        Output = Input;
    else
        Output = -Input;
    return Output;
}

