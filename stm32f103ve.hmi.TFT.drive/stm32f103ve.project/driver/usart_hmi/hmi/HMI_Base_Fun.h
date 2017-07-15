#ifndef _HMI_BASE_FUN_H_
#include "bsp_usart_hmi.h"
/* （1）要使用该函数，必须提供一个原型为：
 * void Usart_SendString( USART_TypeDef * pUSARTx, char *str);的函数接口
 * （2）对于成员Control_Param，不能直接调用sprintf()函数进行赋值，
 *  而应该向一个已经分配内存的char数组，写入实际的字符串，再让Control_Param
 *  指向该数组
 */

typedef struct{
        const char *Control_Name;
        const char *Control_Property;
        char *Control_Param;
}HMI_Control_Typtdef;

void HMI_Updata_Control_Property(HMI_Control_Typtdef controlture);



#endif /* HMI_Base_Fun.h */
