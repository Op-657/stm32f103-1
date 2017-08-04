#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_
#include "stm32f10x.h"
#include "stdio.h"
#include "bsp_systick.h"

typedef struct{
        u8 start;
        u8 finish;
        u8 measuring;
        u8 updata_num;
        u16 capture_val;
}capture;

#define START_CONTROL_PORT              GPIOA
#define START_CONTROL_PIN               GPIO_Pin_3


#define TIM_CAPTURE_PORT                GPIOA
#define TIM_CAPTURE_PIN                 GPIO_Pin_2

#define ULTRASONIC_TIM_CLOCK_FUN        RCC_APB1PeriphClockCmd
#define ULTRASONIC_TIM_CLOCK_Periph     RCC_APB1Periph_TIM2
#define ULTRASONIC_TIM_IRQ              TIM2_IRQn

#define ULTRASONIC_TIM                  TIM2
#define ULTRASONIC_TIM_Period           36000
#define ULTRASONIC_TIM_Prescaler        1                



void ULTRASONIC_Init(void);
float Read_Distance(void);









#endif /* _ULTRASONIC_H_ */