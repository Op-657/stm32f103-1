#ifndef _HMI_USART_H_
#define _HMI_USART_H_
#include "bsp_usart_to_hmi.h"
/*ע��:
 * ��1��Ҫʹ�øú����������ṩһ��ԭ��Ϊ��
 * void Usart_SendString( USART_TypeDef * pUSARTx, char *str);�ĺ����ӿ�
 * ��2�����ڳ�ԱControl_Param������ֱ�ӵ���sprintf()�������и�ֵ��
 *  ��Ӧ����һ���Ѿ������ڴ��char���飬д��ʵ�ʵ��ַ���������Control_Param
 *  ָ�������
 */

typedef struct{
        /* �ؼ����� */
	const char *Control_Name;
		/* �ؼ������� */
        const char *Control_Property;
		/* �ؼ������Բ��� */
        char *Control_Param;
}HMI_Control_Typtdef;

void HMI_Updata_Control_Property(HMI_Control_Typtdef* controlture);
void HMI_Send_Command(char * Command);



#endif /* HMI_USART_H_ */
