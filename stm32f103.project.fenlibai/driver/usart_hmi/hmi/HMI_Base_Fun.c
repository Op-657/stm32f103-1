#include "string.h"
#include "HMI_Base_Fun.h"
#include "bsp_usart2.h"

/* ������ָ��Ľ�����־���� */
char END_flag[4] = {0xff,0xff,0xff,'\0'};

/**
 * @brief ����һ��HMI_Control_Typtdef �ṹ���������������
 *        �򴮿���������Ӧָ��
 * @param �������ռ�ṹ�����
 * @note ���ں����ڲ��Ĵ���û���Զ������ڴ棬����ÿ��charָ����ָ����ַ���
 *       ����̫��
 */
void HMI_Updata_Control_Property(HMI_Control_Typtdef controlture)
{
        
        char buff[50]={0};       
        sprintf(buff,"%s",controlture.Control_Name);
        strcat(buff,".");
        strcat(buff,controlture.Control_Property);
        strcat(buff,"=");
        if(!strncmp(controlture.Control_Property,"txt",4))
        {
                strcat(buff,"\"");
                strcat(buff,controlture.Control_Param);
                strcat(buff,"\"");
                printf("%s",buff);
        }
        else
        {
                strcat(buff,controlture.Control_Param);
                printf("%s",buff);
        }
        Usart_SendString(HMI_USARTx,buff);
        Usart_SendString(HMI_USARTx,END_flag);

}
