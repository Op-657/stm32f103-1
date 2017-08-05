#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "UltrasonicWave.h"
int main(void)
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        systick_init();
        usart_init();
        ULTRASONIC_Init();
        printf("test");
        while(1)
        {
                
                delay_ms(60);
                Read_Distance();
        }
}
