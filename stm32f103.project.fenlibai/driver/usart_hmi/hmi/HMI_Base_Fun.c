#include "string.h"
#include "HMI_Base_Fun.h"
#include "bsp_usart2.h"

/* 串口屏指令的结束标志常量 */
char END_flag[4] = {0xff,0xff,0xff,'\0'};

/**
 * @brief 接收一个HMI_Control_Typtdef 结构体变量，驱动串口
 *        向串口屏发送相应指令
 * @param 串口屏空间结构体变量
 * @note 由于函数内部的处理没有自动分配内存，所以每个char指针所指向的字符串
 *       不能太长
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
