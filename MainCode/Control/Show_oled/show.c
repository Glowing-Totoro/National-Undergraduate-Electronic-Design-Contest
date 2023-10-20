#include "show.h"
/* ��ʱ��ű��� */
float a;
float b;
/**************************************************************************
Function: OLED_Show
Input   : none
Output  : none
�������ܣ���ʾ����ʾ����
��ڲ���: �� 
����  ֵ����
**************************************************************************/         
void Show(void)
{
    memset(OLED_GRAM,0, 128*8*sizeof(uint8_t));    //GRAM���㵫������ˢ�£���ֹ����
    /* ��һ�У�ģʽѡ����״̬��ʾ */
    OLED_ShowString(0,0,"Mode1:");
    OLED_ShowString(50,0,"Test");
    if (num_receive>=29)  OLED_ShowString(100,0,"Yes");
    else OLED_ShowString(100,0,"No");
    /* �ڶ��У������ٶ�(���Դ�Ϊģ����ʾ�����ٶ�) */
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
    /* �����У���ʻ��̼�¼(�ɷ��ռ�¼�����ۼ����) */
    OLED_ShowString(0,20,"Distance:");
    b = 200;
    uint32_t dymac2;
    if (b < 0){dymac2= -b*10;OLED_ShowString(70,20,"-");}
    else{dymac2 = b*10;OLED_ShowString(70,20,"+");}
    OLED_ShowNumber(78,20,dymac2/10,4,12);
    OLED_ShowString(102,20,".");
    OLED_ShowNumber(106,20,dymac2%10,1,12);
    OLED_ShowString(113,20,"cm");
    /* �����У�����״̬��ʾ */
    OLED_ShowString(0,30,"Label:");
    OLED_ShowString(50,30,"Loop1_Lb1");
    /* �����У��Զ���(��ccdģʽ��ѭ��λ��) */
    // OLED_Show_CCD();
    OLED_ShowNumber(20,40,(int)Target_Place1[0],3,12);
    OLED_ShowNumber(50,40,(int)Target_Place1[1],3,12);
    OLED_ShowNumber(80,40,(int)Actual_Place[0],3,12);
    OLED_ShowNumber(100,40,(int)Actual_Place[1],3,12);
    /*�����У���ص�ѹ��ϵͳ״̬  */
    
    //==================ˢ��==================//
    OLED_Refresh_Gram();    
}

/**************************************************************************
Function: OLED_Show_CCD
Input   : none
Output  : none
�������ܣ�CCDģʽ��ʾ����������
��ڲ���: �� 
����  ֵ����
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
//�������ܣ���APP��������
//��ڲ�������
//����  ֵ����
//**************************************************************************/
//void APP_Show(void)
//{    
//    static uint8_t flag;
//    int Velocity_Left_Show,Velocity_Right_Show,Voltage_Show;
//    Voltage_Show=(Voltage-1000)*2/3;            if(Voltage_Show<0)Voltage_Show=0;if(Voltage_Show>100) Voltage_Show=100;   //�Ե�ѹ���ݽ��д���
//    Velocity_Right_Show=MotorB.Velocity*1.1;     if(Velocity_Right_Show<0) Velocity_Right_Show=-Velocity_Right_Show;              //�Ա��������ݾ������ݴ������ͼ�λ�
//    Velocity_Left_Show= MotorA.Velocity*1.1;      if(Velocity_Left_Show<0) Velocity_Left_Show=-Velocity_Left_Show;
//    flag=!flag;
//    if(PID_Send==1)            //����PID����,��APP���ν�����ʾ
//    {
//         printf("{C%d:%d:%d:%d:%d:%d:%d:%d:%d}$",(int)RC_Velocity,(int)CCD_Move_X,(int)0,(int)0,(int)0,(int)0,(int)0,0,0);//��ӡ��APP����
//         PID_Send=0;        
//  }
//   else    if(flag==0)        // ���͵�ص�ѹ���ٶȣ��ǶȵȲ�������APP��ҳ��ʾ
//       printf("{A%d:%d:}$",(int)Voltage_Show,(int)0); //��ӡ��APP����
//     else                //����С����̬�ǣ��ڲ��ν�����ʾ
//        printf("{B%d:%d}$",(int)MotorA.Current_Encoder,(int)MotorB.Current_Encoder); //��ʾ���ұ��������ٶ�                                                                       //�ɰ���ʽ����������ʾ���Σ�������ʾ���
//}


////��λ��ʾ����
///**************************************************************************
//�������ܣ�����ʾ��������λ���������� �ر���ʾ��
//��ڲ�������
//����  ֵ����
//��    �ߣ�ƽ��С��֮��
//**************************************************************************/
//void DataScope(void)
//{   
//    uint8_t i;            //��������
//    uint8_t Send_Count;    //������Ҫ���͵����ݸ���
//    float Vol;        //��ѹ����
//    Vol=(float)Voltage/100;
//    DataScope_Get_Channel_Data( MotorA.Velocity, 1 );         //��ʾ�����ٶȣ���λmm/s
//    DataScope_Get_Channel_Data( MotorB.Velocity, 2 );        //��ʾ�����ٶ�
//    DataScope_Get_Channel_Data( Vol, 3 );                   //��ʾ��ص�ѹ ��λ��V
////    DataScope_Get_Channel_Data(0, 5 ); //����Ҫ��ʾ�������滻0������
////    DataScope_Get_Channel_Data(0 , 6 );//����Ҫ��ʾ�������滻0������
////    DataScope_Get_Channel_Data(0, 7 );
////    DataScope_Get_Channel_Data( 0, 8 ); 
////    DataScope_Get_Channel_Data(0, 9 );  
////    DataScope_Get_Channel_Data( 0 , 10);
//    Send_Count = DataScope_Data_Generate(CHANNEL_NUMBER);//CHANNEL_NUMBER�ɸı�ͨ��������Ŀǰ��4
//    for( i = 0 ; i < Send_Count; i++) 
//    {
//        while((USART1->SR&0X40)==0);  
//        USART1->DR = DataScope_OutPut_Buffer[i]; 
//    }
//}

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����ֵ��uint
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

