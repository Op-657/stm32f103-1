#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "motor.h"
#include "bsp_BaseTim.h"
#include "bsp_pwm_output.h"
#include "test.h"
#include "Motor_PID.h"
#include "bsp_led.h" 
#include "usart_mpu.h"
#include "bsp_usart2.h"
#include "data_scope.h"
#include "bsp_key.h"
#include "bsp_usart_to_hmi.h"
#include "USART_HMI.h"
#include "string.h"


/***�жϵ�ʹ���ܽ�
* �жϷ���Ϊ 2  2λ��ռ  2λ�����ȼ�(ֵԽС�����ȼ�Խ�ߣ�
* ����                    �������ȼ�           �����ȼ�
* USart1                  3                    3
* Usart2(mpu)             3                    0                    
* Usart3(������)          3                    2
* tim7(����Ƶ��)          3                    1
*/

void HMI_Control_Init(void);

u8 HMI_END_FLAG[4] = {0xff,0xff,0xff,'\0'};

extern u8 Current_Mode;//ģʽ����
extern u8 Offset_rotation;//����ƫ�ƵĽǶ�
extern u8 line_length;//��ֱ�ߵĳ��ȵĳ��ȵ�һ��
extern u8  radius_of_circle;//��Բ�İ뾶
HMI_Control_Typtdef HMI_length,HMI_angle,HMI_R;
char data_buff[10];
int main(void)
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        systick_init();
        usart_init();
        Base_Tim_Init();
        Motor_Init();
        PID_M1_Init();
        PID_M2_Init();
        USART_Config();
        Key_GPIO_Init();
        HMI_USART_Init();
        HMI_Control_Init();
        Current_Mode = 4;
        HMI_Send_Command("page mode3");
        printf("usart1 test\n");
//        test_control_motor_gpio();
        while(1)
        {
/* 
* ��������Ԥ��
* ����                            ����
* k1(PB5)               �л������ڵ�ģʽ�ʹ���������ʾҳ��
* k2(PB6)               �������ڵ�ģʽΪ2ʱ�����Ӱ��ߵĳ��ȣ�����Ϊ10������Ϊ60
                        �������ڵ�ģʽΪ4ʱ�����ӻ�Բ�İ뾶�ĳ��ȣ�����Ϊ10������Ϊ60
* k3(A11)               �������ڵ�ģʽΪ2ʱ�����°��ߵĳ��ȣ�����Ϊ10������Ϊ30            
                        �������ڵ�ģʽΪ4ʱ����С��Բ�İ뾶�ĳ��ȣ�����Ϊ10������Ϊ60
* k4(A12)               �������ڵ�ģʽΪ2ʱ��ѭ����0��180�����ӻ�ֱ�ߵ�ƫ��(�����ģ���е�x���������ʱ��Ƕȣ�������Ϊ30 
*/                
//                get_angle();
                switch(Key_Scan())//ɨ�谴������û�а�������ʱ������ֵΪ0�����а������£�������Ӧ�İ���
                {
                        case 1:         //����1������
                               Current_Mode++;
                               if(Current_Mode>=6)
                                        Current_Mode=0;
                               switch(Current_Mode)
                               {
                                       case 0://�л�����һ��ҳ��
                                         Usart_SendString(HMI_USARTx,"page mian");
                                         Usart_SendString(HMI_USARTx,(char *)HMI_END_FLAG);
                                         
                                         break;
                                       case 1:
                                         Usart_SendString(HMI_USARTx,"page mode1");
                                         Usart_SendString(HMI_USARTx,(char *)HMI_END_FLAG);
                                         break;
                                       case 2:
                                         Usart_SendString(HMI_USARTx,"page mode2");
                                         Usart_SendString(HMI_USARTx,(char *)HMI_END_FLAG);
                                         HMI_Updata_Control_Property(&HMI_length);
                                         HMI_Updata_Control_Property(&HMI_angle);
                                         break;
                                       case 3:
                                         Usart_SendString(HMI_USARTx,"page mode3");
                                         Usart_SendString(HMI_USARTx,(char *)HMI_END_FLAG);
                                         break;
                                       case 4:
                                         Usart_SendString(HMI_USARTx,"page mode4");
                                         Usart_SendString(HMI_USARTx,(char *)HMI_END_FLAG);
                                         HMI_Updata_Control_Property(&HMI_R);
                                         break; 
                                       case 5:
                                         Usart_SendString(HMI_USARTx,"page last");
                                         Usart_SendString(HMI_USARTx,(char *)HMI_END_FLAG);
                                         break;
                                       default:
                                               break;
                               }
                               break;                    
                        case 2:         //����2������
                                printf("key2 ring\n");
                                if(Current_Mode == 2)
                                {         
                                        line_length += 10;
                                        
                                        if(line_length>60)
                                        {
                                                line_length = 60;
                                        }
                                        sprintf(data_buff,"%d",line_length);
                                        HMI_length.Control_Param = data_buff;
                                        HMI_Updata_Control_Property(&HMI_length);       
                                }
                                if(Current_Mode == 4)
                                {
                                        radius_of_circle += 5;
                                        if(radius_of_circle>60)
                                        {
                                                radius_of_circle = 60;
                                        }
                                        sprintf(data_buff,"%d",radius_of_circle);
                                        HMI_R.Control_Param = data_buff;
                                        HMI_Updata_Control_Property(&HMI_R); 
                                }
        
                                break;
                                
                        case 3:
                                printf("key3 ring\n");
                                if(Current_Mode == 2)
                                {
                                        
                                        line_length = line_length - 10;
                                        if(line_length<30)
                                        {
                                                line_length = 30;
                                        }
                                        sprintf(data_buff,"%d",line_length);
                                        HMI_length.Control_Param= data_buff;
                                        HMI_Updata_Control_Property(&HMI_length); 
                                }
                                if(Current_Mode == 4)
                                {
                                        radius_of_circle -= 5;
                                        if(radius_of_circle<30)
                                        {
                                                radius_of_circle = 30;
                                        }
                                        sprintf(data_buff,"%d",radius_of_circle);
                                        HMI_R.Control_Param = data_buff;
                                        HMI_Updata_Control_Property(&HMI_R); 
                                }
                                break;
                        case 4:
                                printf("key4 ring\n");
                                if(Current_Mode == 2 )
                                {
                                      Offset_rotation += 30;
                                      if(Offset_rotation >180)
                                      {
                                              Offset_rotation = 30;
                                      }
                                      sprintf(data_buff,"%d",Offset_rotation);
                                      HMI_angle.Control_Param= data_buff;
                                      HMI_Updata_Control_Property(&HMI_angle);
                                }

                                break;
                                
                        default:
                                break;
                }

        }
}

void HMI_Control_Init(void)
{
        HMI_length.Control_Name="length";
        HMI_length.Control_Property="txt";
        HMI_length.Control_Param = "35";
        
        HMI_angle.Control_Name = "angle";
        HMI_angle.Control_Property= "txt";
        HMI_angle.Control_Param = "0";
        
        HMI_R.Control_Name = "R";
        HMI_R.Control_Property= "txt";
        HMI_R.Control_Param = "30";
}
