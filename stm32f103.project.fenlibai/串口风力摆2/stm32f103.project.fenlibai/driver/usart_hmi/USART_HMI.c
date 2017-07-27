#include "string.h"
#include "USART_HMI.h"

/* ������ָ��Ľ�����־���� */
char END_flag[4] = {0xff,0xff,0xff,'\0'};


/**
  * @brief ��HMI�����ַ�������ı���ʾ��״̬
  *       
  * @param �����ַ���        
  * @note  ֻ��Ҫ��������ʵ�壬�Զ����ͽ�����־
  *       
  */
void HMI_Send_Command(char * Command)
{
        HMI_Usart_SendString(HMI_USARTx,Command);
        HMI_Usart_SendString(HMI_USARTx,END_flag);
}




/**
  * @brief ����һ��HMI_Control_Typtdef �ṹ�������ָ�룬���ڸ��¿ؼ��Ĳ���
  *        �ú�����Ҫ�����������ֺ��ı��ؼ�
  * @param �������ռ�ṹ�����
  * @note ���ں����ڲ��Ĵ���û���Զ������ڴ棬����ÿ��charָ����ָ����ַ���
  *       ����̫��
  */
void HMI_Updata_Control_Property(HMI_Control_Typtdef* controlture)
{
        
        char buff[50]={0};       
        sprintf(buff,"%s",controlture->Control_Name);
        strcat(buff,".");
        strcat(buff,controlture->Control_Property);
        strcat(buff,"=");
        if(!strncmp(controlture->Control_Property,"txt",4))
        {
                strcat(buff,"\"");
                strcat(buff,controlture->Control_Param);
                strcat(buff,"\"");
                printf("%s",buff);
        }
        else
        {
                strcat(buff,controlture->Control_Param);
                printf("%s",buff);
        }
        HMI_Usart_SendString(HMI_USARTx,buff);
        HMI_Usart_SendString(HMI_USARTx,END_flag);

}


