#ifndef _HMI_BASE_FUN_H_
#include "bsp_usart_hmi.h"
/* ��1��Ҫʹ�øú����������ṩһ��ԭ��Ϊ��
 * void Usart_SendString( USART_TypeDef * pUSARTx, char *str);�ĺ����ӿ�
 * ��2�����ڳ�ԱControl_Param������ֱ�ӵ���sprintf()�������и�ֵ��
 *  ��Ӧ����һ���Ѿ������ڴ��char���飬д��ʵ�ʵ��ַ���������Control_Param
 *  ָ�������
 */

typedef struct{
        const char *Control_Name;
        const char *Control_Property;
        char *Control_Param;
}HMI_Control_Typtdef;

void HMI_Updata_Control_Property(HMI_Control_Typtdef controlture);



#endif /* HMI_Base_Fun.h */
