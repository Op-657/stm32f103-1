/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   MPU6050����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:���� F103-ָ���� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */


#include "mpu6050.h"
#include "bsp_usart.h"
#include "bsp_i2c.h"


/**
  * @brief   д���ݵ�MPU6050�Ĵ���
  * @param   
  * @retval  
  */
void MPU6050_WriteReg(u8 reg_add,u8 reg_dat)
{
	i2c_Start();
	i2c_SendByte(MPU6050_SLAVE_ADDRESS);
	i2c_WaitAck();
	i2c_SendByte(reg_add);
	i2c_WaitAck();
	i2c_SendByte(reg_dat);
	i2c_WaitAck();
	i2c_Stop();
}


/**
  * @brief   ��MPU6050�Ĵ�����ȡ����
  * @param   
  * @retval  
  */
void MPU6050_ReadData(u8 reg_add,unsigned char*Read,u8 num)
{
	unsigned char i;
	
	i2c_Start();
	i2c_SendByte(MPU6050_SLAVE_ADDRESS);
	i2c_WaitAck();
	i2c_SendByte(reg_add);
	i2c_WaitAck();
	
	i2c_Start();
	i2c_SendByte(MPU6050_SLAVE_ADDRESS+1);
	i2c_WaitAck();
	
	for(i=0;i<(num-1);i++){
		*Read=i2c_ReadByte(1);
		Read++;
	}
	*Read=i2c_ReadByte(0);
	i2c_Stop();
}


/**
  * @brief   ��ʼ��MPU6050оƬ
  * @param   
  * @retval  
  */
void MPU6050_Init(void)
{
        int i=0,j=0;
        //�ڳ�ʼ��֮ǰҪ��ʱһ��ʱ�䣬��û����ʱ����ϵ�����ϵ����ݿ��ܻ����
        for(i=0;i<1000;i++)
        {
                for(j=0;j<1000;j++);
               
        }
        MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	    //�������״̬
        MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //�����ǲ����ʣ�1KHz
        MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	    //��ͨ�˲��������ã���ֹƵ����1K��������5K
        MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x00);   //���ü��ٶȴ�����������2Gģʽ�����Լ�
        MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //�������Լ켰������Χ0x00��ʾ��-250-+250��/�룩������ֵ��0x18(���Լ죬2000deg/s)
}



/**
  * @brief   ��ȡMPU6050��ID
  * @param   
  * @retval  
  */
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
        MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //��������ַ
        printf("%d\n",Re);
	if(Re != 0x68)
	{
		printf("MPU6050 dectected error!\r\n��ⲻ��MPU6050ģ�飬����ģ���뿪����Ľ���");
                printf("%d",Re);
                
		return 0;
	}
	else
	{
		printf("MPU6050 ID = %d\r\n",Re);
                printf("%d",Re);
                
		return 1;
	}
		
}
/**
  * @brief   ��ȡMPU6050�ļ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadAcc(MPU6050_AccTypeDef *AccTypeDef)
{
    u8 buf[6];
    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    AccTypeDef->AccX = (buf[0] << 8) | buf[1];
    AccTypeDef->AccY = (buf[2] << 8) | buf[3];
    AccTypeDef->AccZ = (buf[4] << 8) | buf[5];
}

/**
  * @brief   ��ȡMPU6050�ĽǼ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadGyro(MPU6050_GyroTypeDef *GyroTypeDef)
{
    u8 buf[6];
    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    GyroTypeDef->GyroX = (float)((buf[0] << 8) | buf[1]);
    GyroTypeDef->GyroY = (float)((buf[2] << 8) | buf[3]);
    GyroTypeDef->GyroZ = (float)((buf[4] << 8) | buf[5]);
}


/**
  * @brief   ��ȡMPU6050��ԭʼ�¶�����
  * @param   
  * @retval  
  */
void MPU6050ReadTemp(short *tempData)
{
        u8 buf[2];
        MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
        *tempData = (buf[0] << 8) | buf[1];
}


/**
  * @brief   ��ȡMPU6050���¶����ݣ�ת�������϶�
  * @param   
  * @retval  
  */
void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	u8 buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
        temp3= (buf[0] << 8) | buf[1];	
	*Temperature=((double) temp3/340.0)+36.53; 

}
/**
  * @brief   ��ȡmpu6050�ļĴ�����X����ٶȵ�ԭʼֵ
  * @param   
  * @return  ����ֵһ��16λ����ֵ  
  */
u16 Get_MPU_Reg_Acc_X(void)
{
        unsigned char temp[2]={0,0};
        u16 result=0;
        MPU6050_ReadData(MPU6050_ACC_XOUT_H,temp,2);
        result = temp[0];
        result = (result<<8)|temp[1];
        return result;
}
/**
  * @brief   ��ȡmpu6050�ļĴ�����Y����ٶȵ�ԭʼֵ
  * @param   
  * @return  ����ֵһ��16λ����ֵ  
  */
u16 Get_MPU_Reg_Acc_Y(void)
{
        unsigned char temp[2]={0,0};
        u16 result=0;
        MPU6050_ReadData(MPU6050_ACC_YOUT_H,temp,2);
        result = temp[0];
        result = (result<<8)|temp[1];
        return result;
}
/**
  * @brief   ��ȡmpu6050�ļĴ�����Z����ٶȵ�ԭʼֵ
  * @param   
  * @return  ����ֵһ��16λ����ֵ  
  */
u16 Get_MPU_Reg_Acc_Z(void)
{
        unsigned char temp[2]={0,0};
        short result=0;
        MPU6050_ReadData(MPU6050_ACC_ZOUT_H,temp,2);
        result = temp[0];
        result = (result<<8)|temp[1];
        return result;
}
/**
  * @brief   ��ȡmpu6050�ļĴ�����X����ٶȵ�ԭʼֵ
  * @param   
  * @return  ����ֵһ��16λ����ֵ  
  */
u16 Get_MPU_Reg_Gyro_X(void)
{
        unsigned char temp[2]={0,0};
        u16 result=0;
        MPU6050_ReadData(MPU6050_GYRO_XOUT_H,temp,2);
        result = temp[0];
        result = (result<<8)|temp[1];
        return result;
}
/**
  * @brief   ��ȡmpu6050�ļĴ�����Y����ٶȵ�ԭʼֵ
  * @param   
  * @return  ����ֵһ���޷���λ����ֵ  
  */
u16 Get_MPU_Reg_Gyro_Y(void)
{
        unsigned char temp[2]={0,0};
        u16 result=0;
        MPU6050_ReadData(MPU6050_GYRO_YOUT_H,temp,2);
        result = temp[0];
        result = (result<<8)|temp[1];
        return result;
}
/**
  * @brief   ��ȡmpu6050�ļĴ�����Z����ٶȵ�ԭʼֵ
  * @param   
  * @return  ����ֵһ���޷���16λ����ֵ  
  */
u16 Get_MPU_Reg_Gyro_Z(void)
{
        unsigned char temp[2]={0,0};
        u16 result=0;
        MPU6050_ReadData(MPU6050_GYRO_ZOUT_H,temp,2);
        result = temp[0];
        result = (result<<8)|temp[1];
        return result;
}




/**
  * @brief   ��ȡmpu6050X�᷽��ļ��ٶ�
  * @param   
  * @return  ����һ�������� 
  */
float Get_MPU_Acc_X(void)
{
        short temp=0;
        float result=0;
        temp = (short)Get_MPU_Reg_Acc_X();
        switch(MPU6050_Set_RA_Acc)
        {
                case 0:
                        result =  (float)((temp/32768.0)*19.6);
                        break;
                case 1:
                        result =  (float)((temp/32768.0)*39.2);
                        break;
                case 2:
                        result =  (float)((temp/32768.0)*78.4);
                        break;
                case 3:
                        result =  (float)((temp/32768.0)*156.8);
                        break;
                default:
                        result =  (float)((temp/32768.0)*19.6);
                        break;
        }               
        return result;
}

/**
  * @brief   ��ȡmpu6050Y�᷽��ļ��ٶ�
  * @param   
  * @return  ����һ�������� 
  */
float Get_MPU_Acc_Y(void)
{
        short temp=0;
        float result=0;
        temp = (short)Get_MPU_Reg_Acc_Y();
        switch(MPU6050_Set_RA_Acc)
        {
                case 0:
                        result =  (float)((temp/32768.0)*19.6);
                        break;
                case 1:
                        result =  (float)((temp/32768.0)*39.2);
                        break;
                case 2:
                        result =  (float)((temp/32768.0)*78.4);
                        break;
                case 3:
                        result =  (float)((temp/32768.0)*156.8);
                        break;
                default:
                        result =  (float)((temp/32768.0)*19.6);
                        break;
        }               
        return result;
}

/**
  * @brief   ��ȡmpu6050Z�᷽��ļ��ٶ�
  * @param   
  * @return  ����һ�������� 
  */
float Get_MPU_Acc_Z(void)
{
        short temp=0;
        float result=0;
        temp = (short)Get_MPU_Reg_Acc_Z();
        switch(MPU6050_Set_RA_Acc)
        {
                case 0:
                        result =  (float)((temp/32768.0)*19.6);
                        break;
                case 1:
                        result =  (float)((temp/32768.0)*39.2);
                        break;
                case 2:
                        result =  (float)((temp/32768.0)*78.4);
                        break;
                case 3:
                        result =  (float)((temp/32768.0)*156.8);
                        break;
                default:
                        result =  (float)((temp/32768.0)*19.6);
                        break;
        }               
        return result;
}

float Get_MPU_Gyro_X(void)
{
        short temp=0;
        float result=0;
        temp = (short)Get_MPU_Reg_Gyro_X();
        switch(MPU6050_Set_RA_Acc)
        {
                case 0:
                        result =  (float)((temp/32768.0)*250.0);
                        break;
                case 1:
                        result =  (float)((temp/32768.0)*500.0);
                        break;
                case 2:
                        result =  (float)((temp/32768.0)*1000.0);
                        break;
                case 3:
                        result =  (float)((temp/32768.0)*2000.0);
                        break;
                default:
                        result =  (float)((temp/32768.0)*19.6);
                        break;
        }               
        return result;
}

float Get_MPU_Gyro_Y(void)
{
        short temp=0;
        float result=0;
        temp = (short)Get_MPU_Reg_Gyro_Y();
        switch(MPU6050_Set_RA_Acc)
        {
                case 0:
                        result =  (float)((temp/32768.0)*250.0);
                        break;
                case 1:
                        result =  (float)((temp/32768.0)*500.0);
                        break;
                case 2:
                        result =  (float)((temp/32768.0)*1000.0);
                        break;
                case 3:
                        result =  (float)((temp/32768.0)*2000.0);
                        break;
                default:
                        result =  (float)((temp/32768.0)*19.6);
                        break;
        }               
        return result;
}
float Get_MPU_Gyro_Z(void)
{
        short temp=0;
        float result=0;
        temp = (short)Get_MPU_Reg_Gyro_Z();
        switch(MPU6050_Set_RA_Acc)
        {
                case 0:
                        result =  (float)((temp/32768.0)*250.0);
                        break;
                case 1:
                        result =  (float)((temp/32768.0)*500.0);
                        break;
                case 2:
                        result =  (float)((temp/32768.0)*1000.0);
                        break;
                case 3:
                        result =  (float)((temp/32768.0)*2000.0);
                        break;
                default:
                        result =  (float)((temp/32768.0)*19.6);
                        break;
        }               
        return result;
}



