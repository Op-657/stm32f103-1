#include "ultrasonic.h"



static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_InitStructure.NVIC_IRQChannel = ULTRASONIC_TIM_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void ULTRASONIC_Init(void)
{
        
        TIM_TimeBaseInitTypeDef TIM_TimeBaseInitTypeDefTure;
        TIM_ICInitTypeDef TIM_ICInitTypeDefTure;
        GPIO_InitTypeDef GPIO_InitTypeDefTure;
        
        /* 使能GPIO 和 定时器的时钟 */
        ULTRASONIC_TIM_CLOCK_FUN(RCC_APB1Periph_TIM2,ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
       
        /* 使能输入引脚 */
        GPIO_InitTypeDefTure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_InitTypeDefTure.GPIO_Pin = TIM_CAPTURE_PIN;
        GPIO_Init(TIM_CAPTURE_PORT,&GPIO_InitTypeDefTure);
        /* 使能控制发送引脚 */
        GPIO_InitTypeDefTure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitTypeDefTure.GPIO_Pin = START_CONTROL_PIN;
        GPIO_InitTypeDefTure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(START_CONTROL_PORT,&GPIO_InitTypeDefTure);
        GPIO_ResetBits(START_CONTROL_PORT,START_CONTROL_PIN);
        GPIO_ResetBits(TIM_CAPTURE_PORT,TIM_CAPTURE_PIN);
        
        /* 配置基本结构体 */
        TIM_TimeBaseInitTypeDefTure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInitTypeDefTure.TIM_Period = ULTRASONIC_TIM_Period;
        TIM_TimeBaseInitTypeDefTure.TIM_Prescaler = ULTRASONIC_TIM_Prescaler;
        TIM_TimeBaseInitTypeDefTure.TIM_RepetitionCounter = 0;
        TIM_TimeBaseInitTypeDefTure.TIM_ClockDivision = 0;
        TIM_TimeBaseInit(ULTRASONIC_TIM,&TIM_TimeBaseInitTypeDefTure);
        
        /* 配置输入结构体 */
        TIM_ICInitTypeDefTure.TIM_Channel = TIM_Channel_3;
        TIM_ICInitTypeDefTure.TIM_ICFilter = 0;
        TIM_ICInitTypeDefTure.TIM_ICPolarity = TIM_ICPolarity_Rising;
        TIM_ICInitTypeDefTure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
        TIM_ICInitTypeDefTure.TIM_ICSelection = TIM_ICSelection_DirectTI;
        TIM_ICInit(ULTRASONIC_TIM,&TIM_ICInitTypeDefTure);
        
        TIM_PrescalerConfig(ULTRASONIC_TIM,ULTRASONIC_TIM_Prescaler,TIM_PSCReloadMode_Immediate);
        
        TIM_ITConfig(ULTRASONIC_TIM,TIM_IT_Update|TIM_IT_CC3,ENABLE);
        /* 使能定时器中断 */
        NVIC_Configuration();
        TIM_Cmd(ULTRASONIC_TIM,ENABLE);
}
capture state={0,0,0};


float Read_Distance(void)
{
        double distance;
        if(state.finish == 1)
        {
               distance = state.updata_num *  ULTRASONIC_TIM_Period + state.capture_val;
               distance = distance / 36.0;
               distance = distance * 0.034/2.0;//时间换算成距离 单位为cm
               state.finish = 0;
        }
        /* 使能模块发送超声波 */
        GPIO_SetBits(START_CONTROL_PORT,START_CONTROL_PIN);
        delay_us(15);
        GPIO_ResetBits(START_CONTROL_PORT,START_CONTROL_PIN);
        state.start = 1;
        return distance;
}
        
        


void TIM2_IRQHandler(void)
{

        if(TIM_GetITStatus(ULTRASONIC_TIM,TIM_IT_Update) == SET)
        {
                TIM_ClearFlag(ULTRASONIC_TIM,TIM_IT_Update);
//                printf("updata runing\n");
                if(state.measuring == 1)
                {
                        printf("updata runing\n");
                        if(state.updata_num >= 50)//没有障碍
                        {
                                state.updata_num = 50;
                                state.capture_val =0;
                                TIM_OC3PolarityConfig(ULTRASONIC_TIM,TIM_ICPolarity_Rising);
                                state.finish = 1;
                                state.measuring = 0;
                        }
                        state.updata_num++;
                }
                
        }
        if(TIM_GetITStatus(ULTRASONIC_TIM,TIM_IT_CC3) == SET)
        {
               TIM_ClearFlag(ULTRASONIC_TIM,TIM_IT_CC3);
                printf("上升沿触发\n");
                if(state.start == 1 && state.finish == 0 )
                {
                        ULTRASONIC_TIM->CNT = 0;
                        TIM_OC3PolarityConfig(ULTRASONIC_TIM,TIM_ICPolarity_Falling);
                        state.updata_num = 0;
                        state.measuring = 1;
                        state.start = 0;
                }
                else if(state.start == 0)
                {
                        printf("下降沿触发\n");
                        state.capture_val = TIM_GetCapture3(ULTRASONIC_TIM);
                        printf("%d\n",state.capture_val);
                        TIM_OC3PolarityConfig(ULTRASONIC_TIM,TIM_ICPolarity_Rising);
                        state.finish = 1;
                        state.measuring = 0;
                }             
        }
}
        
        
        
        
        
        
        
        