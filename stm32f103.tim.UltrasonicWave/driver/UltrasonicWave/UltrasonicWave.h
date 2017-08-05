#ifndef _ULTRASONICWAVE_H_
#define _ULTRASONICWAVE_H_
#include "stm32f10x.h"
#include "stdio.h"
#include "bsp_systick.h"



typedef struct{
        u8 finish;
        u8 measuring;
        u8 updata_num;
        u16 capture_val;
}capture;

#define START_CONTROL_PORT                      GPIOA
#define START_CONTROL_PIN                       GPIO_Pin_3
#define START_CONTROL_PORT_CLOCK_Periph         RCC_APB2Periph_GPIOA


#define TIM_CAPTURE_PORT                        GPIOA
#define TIM_CAPTURE_PIN                         GPIO_Pin_2
#define TIM_CAPTURE_PORT_CLOCK_Periph           RCC_APB2Periph_GPIOA


#define ULTRASONIC_TIM_IRQHANDLER               TIM2_IRQHandler
#define ULTRASONIC_TIM_CLOCK_FUN                RCC_APB1PeriphClockCmd
#define ULTRASONIC_TIM_CLOCK_Periph             RCC_APB1Periph_TIM2
#define ULTRASONIC_TIM_IRQ                      TIM2_IRQn
        
#define ULTRASONIC_TIM                          TIM2
#define ULTRASONIC_TIM_Period                   (1000-1)
#define ULTRASONIC_TIM_Prescaler                71
#define ULTRASONIC_TIM_TIM_Channel              TIM_Channel_3

extern float distance;

void ULTRASONIC_Init(void);
void Read_Distance(void);









#endif /* _ULTRASONICWAVE_H_ */