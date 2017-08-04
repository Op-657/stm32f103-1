#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "ultrasonic.h"
int main(void)
{
        float temp;
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        systick_init();
        usart_init();
        ULTRASONIC_Init();
        while(1)
        {
                delay_ms(60);
                temp = Read_Distance();
                printf("distance = %0.2fcm\n",temp);

        }
}
