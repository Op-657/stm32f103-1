#include "bsp_control.h"
#include "Motor_PID.h"
#include "math.h"
#include "motor.h"
#include "data_scope.h"
#include "bsp_usart.h"
#include "usart_mpu.h"
#define SWING_L         44 // �ڸ˵ĳ��ȣ����ڼ��㵥�ڵ����ڣ���λΪcm
#define SWING_T         1331.35 //���ڵ����ڣ���λΪms
#define SWING_H         70 //����ڵ�����ĸ߶�Ϊ70cm
#define PI              3.1415




void Mode0(void)
{
        
}

u8 Offset_rotation = 0;//����ƫ�ƵĽǶ�
u8 line_length=30;              //�ڶ���ֱ��һ��ľ���
u8  radius_of_circle = 30;//ģʽ����Բ�İ뾶

void Mode1(float pitch,float roll)
{
        static float Cycle_normalized=0;        //���ڹ�һ������
        static float Point_normalized=0;        //�Ƕȹ�һ������
        static unsigned int Time_step=0;        //ʱ�����������ڼ����һ����ֵ
        int32_t M1_PWM=0,M2_PWM=0;
        float set_x=0,set_y=0;
        float Angle_max=0;                       //�ڶ����ȵ����ֵ
        Time_step += 10; //10ms�ж�
        Cycle_normalized = Time_step/SWING_T;   //�����ڽ��й�һ������
        Point_normalized = Cycle_normalized*2*3.14159;  //��2pi ���й�һ������
        Angle_max = atan2(line_length,70.0)*57.2958; //�ڶ������Ƕ�
        
        set_x = Angle_max*cos(Offset_rotation * PI/180.0)*sin(Point_normalized);//������ֵ�ֽ⵽���X����
        set_y = Angle_max*sin(Offset_rotation * PI/180.0)*sin(Point_normalized);;//������ֵ�ֽ⵽���Y����
        
        //printf("set_x = %0.2f,set_y = %0.2f",set_x,set_y);
        PID_M1_SetPoint(set_x);//����x����ĵ��������ֵ
        PID_M1_SetKp(60);
        PID_M1_SetKi(0.6);
        PID_M1_SetKd(20000);
        
        PID_M2_SetPoint(set_y); //����y����ĵ��������ֵ
        PID_M2_SetKp(60);
        PID_M2_SetKi(0.6);
        PID_M2_SetKd(20000);
        M1_PWM = PID_M1_PosLocCalc(pitch);//����ʵ��ֵ������x��pwm�ķ���ʹ�С
        M2_PWM = PID_M2_PosLocCalc(roll);//����ʵ��ֵ������y��pwm�ķ���ʹ�С
        if(my_abs(M1_PWM)> 3200)
        {
                if(M1_PWM>0)
                {
                        M1_PWM = 3200;
                }
                else
                {
                        M1_PWM = -3200;
                }
        }
        
        if (my_abs(M2_PWM)> 3200)
        {
                if(M2_PWM>0)
                {
                        M2_PWM = 3200;
                }
                else
                {
                        M2_PWM = -3200;
                }
        }
        Set_MotorX_PWM(M1_PWM);
        Set_MotorY_PWM(M2_PWM);
        
}


void Mode2(float pitch,float roll)
{
                        
        int32_t M1_PWM=0,M2_PWM=0;
        //printf("mode2__runing\n");
        if(my_abs(pitch) < 45 && my_abs(roll) < 45)//����һ���Ƕȣ��������ڵĽǶ�С��45��ʱ����ʼ�ƶ�
        {
                
                PID_M1_SetPoint(0);
                PID_M1_SetKp(60);
                PID_M1_SetKi(0.6);
                PID_M1_SetKd(20000);
                
                PID_M2_SetPoint(0);
                PID_M2_SetKp(60); 
                PID_M2_SetKi(0.6);
                PID_M2_SetKd(20000);
                M1_PWM = PID_M1_PosLocCalc(pitch);
                M2_PWM = PID_M2_PosLocCalc(roll);
                if(my_abs(M1_PWM)> 3000)
                {
                        if(M1_PWM>0)
                        {
                                M1_PWM = 3000;
                        }
                        else
                        {
                                M1_PWM = -3600;
                        }
                }
                if (my_abs(M2_PWM)> 3000)
                {
                        if(M2_PWM>0)
                        {
                                M2_PWM = 3000;
                        }
                        else
                        {
                                M2_PWM = -3000;
                        }
                }
                Set_MotorX_PWM(M1_PWM);
                Set_MotorY_PWM(M2_PWM);
        }
        
        else
        {
                Set_motor1_Stop();
        }  
}



void Mode3(float pitch,float roll)
{
        static float Cycle_normalized=0;        //���ڹ�һ������
        static float Point_normalized=0;        //�Ƕȹ�һ������
        static unsigned int Time_step=0;        //ʱ�����������ڼ����һ����ֵ
        int32_t M1_PWM=0,M2_PWM=0;
        float set_x=0,set_y=0;
        Time_step += 10; //10ms�ж�
        Cycle_normalized = Time_step/SWING_T;   //�����ڽ��й�һ������
        Point_normalized = Cycle_normalized*2*3.14159;  //��2pi ���й�һ������
        
        set_x = radius_of_circle*sin(Point_normalized + PI/2.0);//������ֵ�ֽ⵽���X����
        set_y = radius_of_circle*sin(Point_normalized);//������ֵ�ֽ⵽���Y����
         
        //printf("set_x = %0.2f,set_y = %0.2f",set_x,set_y);
        PID_M1_SetPoint(set_x);//����x����ĵ��������ֵ
        PID_M1_SetKp(60);
        PID_M1_SetKi(0.6);
        PID_M1_SetKd(20000);
        
        PID_M2_SetPoint(set_y); //����y����ĵ��������ֵ
        PID_M2_SetKp(60);
        PID_M2_SetKi(0.6);
        PID_M2_SetKd(20000);
        M1_PWM = PID_M1_PosLocCalc(pitch);//����ʵ��ֵ������x��pwm�ķ���ʹ�С
        M2_PWM = PID_M2_PosLocCalc(roll);//����ʵ��ֵ������y��pwm�ķ���ʹ�С
        if(my_abs(M1_PWM)> 3200)
        {
                if(M1_PWM>0)
                {
                        M1_PWM = 3200;
                }
                else
                {
                        M1_PWM = -3200;
                }
        }
        
        if (my_abs(M2_PWM)> 3200)
        {
                if(M2_PWM>0)
                {
                        M2_PWM = 3200;
                }
                else
                {
                        M2_PWM = -3200;
                }
        }
        Set_MotorX_PWM(M1_PWM);
        Set_MotorY_PWM(M2_PWM);
        
}
        

