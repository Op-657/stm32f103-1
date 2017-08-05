#include "UltrasonicWave.h"


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
        ULTRASONIC_TIM_CLOCK_FUN(ULTRASONIC_TIM_CLOCK_Periph,ENABLE);
        RCC_APB2PeriphClockCmd(START_CONTROL_PORT_CLOCK_Periph|TIM_CAPTURE_PORT_CLOCK_Periph,ENABLE);
       
        /* 配置输入引脚，特别注意的是输入引脚应配置成下拉输入，确保稳定性 */
        GPIO_InitTypeDefTure.GPIO_Mode = GPIO_Mode_IPD;
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
        TIM_ICInitTypeDefTure.TIM_Channel = ULTRASONIC_TIM_TIM_Channel;
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

capture flag={0,0,0,0};
float distance = 0;
void Read_Distance(void)
{
        /* 只有在成功捕获的时候才更新距离值 */
        if(flag.finish == 1)
        {
                /* 定时器计数值 */
                distance = flag.updata_num *  ULTRASONIC_TIM_Period + flag.capture_val;
                /* 有计数值换算成时间，单位为ms ，公式由定时器分频因子决定 */
                distance = distance / 1000.0;
                /* 由速度距离公式，计算出距离，时间往返时间，要除以2 */
                distance = distance * 17;//时间换算成距离 单位为cm
                
                /* debug */
                printf("%0.2f\n",distance);
                 
                /* 开启下一次测距 */
                GPIO_SetBits(START_CONTROL_PORT,START_CONTROL_PIN);
                delay_us(15);
                GPIO_ResetBits(START_CONTROL_PORT,START_CONTROL_PIN);
                flag.finish = 0;
        }
}




void TIM2_IRQHandler(void)
{
        
        if(TIM_GetITStatus(ULTRASONIC_TIM,TIM_IT_Update) == SET)
        {
                TIM_ClearFlag(ULTRASONIC_TIM,TIM_IT_Update);
                if(flag.measuring == 1)         //标记到一次上升沿
                {
                        flag.updata_num++;
                        if(flag.updata_num >= 150)//时间太长了
                        {
                                flag.updata_num = 150;
                                flag.measuring = 0;
                                flag.capture_val = 0;
                                flag.finish = 1;//结束一次测距
                                
                        }                        
                }               
                
        }
        
        if(TIM_GetITStatus(ULTRASONIC_TIM,TIM_IT_CC3) == SET)
        {
                if(flag.finish == 0)
                {

                        if(flag.measuring == 1 )        //标记到一次上升沿
                        {
                               /* 标记一次成功捕获 */
                               flag.finish = 1;
                               /* 清标志位 */
                               flag.measuring = 0;
                               flag.capture_val = ULTRASONIC_TIM->CCR3;
                               /* 改变捕获方式为上升沿捕获 */
                               ULTRASONIC_TIM->CCER&=~(1<<9);
                        }
                        else
                        {
                                /* 计数器值清零 */
                                ULTRASONIC_TIM->CNT = 0;
                                /* 溢出计数值清零 */
                                flag.updata_num = 0;
                                /* 标记捕获到一次上升沿 */
                                flag.measuring = 1;
                                /* 改变捕获方式为下降沿触发 */
                                ULTRASONIC_TIM->CCER|=1<<9;
                        }
                                
                        

                }
                TIM_ClearFlag(ULTRASONIC_TIM,TIM_IT_CC3);                     
        }
}
        
        
        
        
        