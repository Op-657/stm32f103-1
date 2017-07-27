#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_usart_to_hmi.h"
#include "USART_HMI.h"
char temp1[50];
int main(void)
{
        HMI_Control_Typtdef txt;
        txt.Control_Name="txt";
        txt.Control_Param ="txt";
        txt.Control_Property="txt";
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        systick_init();
        usart_init();
        HMI_USART_Init();
	
        while(1)
        {
                printf("[%s][%d]", __func__, __LINE__);
                HMI_Updata_Control_Property(txt);
                delay_ms(1000);
                
                HMI_Send_Command("txt.txt=\"hello\"");
                delay_ms(1000);
        }
}
