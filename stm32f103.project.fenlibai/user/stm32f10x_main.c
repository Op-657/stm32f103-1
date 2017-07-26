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
#include "bsp_usart_hmi.h"
#include "HMI_Base_Fun.h"
u8 HMI_END_FLAG[4] = {0xff,0xff,0xff,'\0'};
extern u8 Current_Mode;//模式变量
extern float Offset_rotation;//设置偏移的角度
extern u8 R;
extern u8  radius_of_circle;


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
        USART3_Config();
        Key_GPIO_Init();
//        test_control_motor_gpio();

        while(1)
        {
                switch(Key_Scan())
                {
                        case 0:
                                Current_Mode = 0;
                                Usart_SendString(HMI_USARTx,"page main");
                                Usart_SendString(HMI_USARTx,(char *)HMI_END_FLAG);
                                break;
                        case 1:         //按键1被按下
                               Current_Mode++;
                               if(Current_Mode>=4)
                                        Current_Mode=0;
                               break;
                        case 2:         //按键2被按下
                                if(Current_Mode == 2)
                                {
                                        
                                        R += 10;
                                        if(R>60)
                                        {
                                                R = 60;
                                        }
                                }
                                if(Current_Mode == 4)
                                {
                                        radius_of_circle += 5;
                                        if(radius_of_circle>60)
                                        {
                                                radius_of_circle = 60;
                                        }
                                }
        
                                break;
                                
                        case 3:
                                if(Current_Mode == 2)
                                {
                                        
                                        R -= 10;
                                        if(R>60)
                                        {
                                                R = 60;
                                        }
                                }
                                if(Current_Mode == 4)
                                {
                                        radius_of_circle -= 5;
                                        if(radius_of_circle>60)
                                        {
                                                radius_of_circle = 60;
                                        }
                                }
                                break;
                        case 4:
                                if(Current_Mode == 2 )
                                {
                                      Offset_rotation += 10;
                                }
                                break;
                                
                        default:
                                Current_Mode = 0;
                                break;
                }                                                    
        }
}
