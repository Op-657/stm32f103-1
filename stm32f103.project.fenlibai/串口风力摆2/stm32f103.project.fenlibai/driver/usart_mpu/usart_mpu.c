#include "usart_mpu.h"
#include "stdio.h"
AccTypedef Acc = {0,0,0};
GyroTypedef Gyro= {0,0,0};
AngleTypedef Angle= {0,0,0};
float  Temperature=0;
u8 updata_flag = 0;
u8 Re_buf[11] = {0};
u8 Re_temp[11] = {0};
void get_up_all_data(void)
{
        if(updata_flag == 1)
        {
                switch(Re_buf[1])
                {
                        case 0x51: //��ʶ������Ǽ��ٶȰ�
                           Acc.Acc_X = ((short)(Re_buf[3]<<8 | Re_buf[2]))/32768.0*16;      //X����ٶ�
                           Acc.Acc_Y= ((short)(Re_buf[5]<<8 | Re_buf[4]))/32768.0*16;      //Y����ٶ�
                           Acc.Acc_Z = ((short)(Re_buf[7]<<8 | Re_buf[6]))/32768.0*16;      //Z����ٶ�
                           Temperature = ((short)(Re_buf[9]<<8 | Re_buf[8]))/340.0+36.25;      //�¶�
                           break;
                        
                        case 0x52: //��ʶ������ǽ��ٶȰ�
                           Gyro.Gyro_X = ((short)(Re_buf[3]<<8| Re_buf[2]))/32768.0*2000;      //X����ٶ�
                           Gyro.Gyro_Y = ((short)(Re_buf[5]<<8| Re_buf[4]))/32768.0*2000;      //Y����ٶ�
                           Gyro.Gyro_Z = ((short)(Re_buf[7]<<8| Re_buf[6]))/32768.0*2000;      //Z����ٶ�
                           Temperature = ((short)(Re_buf[9]<<8| Re_buf[8]))/340.0+36.25;      //�¶�
                           break;
                        
                        case 0x53: //��ʶ������ǽǶȰ�
                           Angle.roll = ((short)(Re_buf[3]<<8| Re_buf[2]))/32768.0*180;   //X���ת�ǣ�x �ᣩ
                           Angle.pitch = ((short)(Re_buf[5]<<8| Re_buf[4]))/32768.0*180;   //Y�ḩ���ǣ�y �ᣩ
                           Angle.yaw = ((short)(Re_buf[7]<<8| Re_buf[6]))/32768.0*180;   //Z��ƫ���ǣ�z �ᣩ
                           Temperature = ((short)(Re_buf[9]<<8| Re_buf[8]))/340.0+36.25;   //�¶�
                           //printf("X��Ƕȣ�%.2f   Y��Ƕȣ�%.2f   Z��Ƕȣ�%.2f\r\n",angle[0],angle[1],angle[2]);
                           break;
                        
                         default:  
                           break;
                }
        updata_flag  = 0;
        }
}

void get_angle(void)
{

        if(Re_buf[1] == 0x53)
        {                
                Angle.roll = ((short)(Re_buf[3]<<8| Re_buf[2]))/32768.0*180;   //X���ת�ǣ�x �ᣩ
                Angle.pitch = ((short)(Re_buf[5]<<8| Re_buf[4]))/32768.0*180;   //Y�ḩ���ǣ�y �ᣩ
                Angle.yaw = ((short)(Re_buf[7]<<8| Re_buf[6]))/32768.0*180;   //Z��ƫ���ǣ�z �ᣩ
                Temperature = ((short)(Re_buf[9]<<8| Re_buf[8]))/340.0+36.25;   //�¶� 
                printf("Angle.roll:%0.2f, Angle.pitch:%0.2f\n",Angle.roll,Angle.pitch);
        }
}
        


