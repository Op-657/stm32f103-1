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
        
        /* ʹ��GPIO �� ��ʱ����ʱ�� */
        ULTRASONIC_TIM_CLOCK_FUN(ULTRASONIC_TIM_CLOCK_Periph,ENABLE);
        RCC_APB2PeriphClockCmd(START_CONTROL_PORT_CLOCK_Periph|TIM_CAPTURE_PORT_CLOCK_Periph,ENABLE);
       
        /* �����������ţ��ر�ע�������������Ӧ���ó��������룬ȷ���ȶ��� */
        GPIO_InitTypeDefTure.GPIO_Mode = GPIO_Mode_IPD;
        GPIO_InitTypeDefTure.GPIO_Pin = TIM_CAPTURE_PIN;
        GPIO_Init(TIM_CAPTURE_PORT,&GPIO_InitTypeDefTure);
        /* ʹ�ܿ��Ʒ������� */
        GPIO_InitTypeDefTure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitTypeDefTure.GPIO_Pin = START_CONTROL_PIN;
        GPIO_InitTypeDefTure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(START_CONTROL_PORT,&GPIO_InitTypeDefTure);
        GPIO_ResetBits(START_CONTROL_PORT,START_CONTROL_PIN);
        GPIO_ResetBits(TIM_CAPTURE_PORT,TIM_CAPTURE_PIN);
        
        /* ���û����ṹ�� */
        TIM_TimeBaseInitTypeDefTure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInitTypeDefTure.TIM_Period = ULTRASONIC_TIM_Period;
        TIM_TimeBaseInitTypeDefTure.TIM_Prescaler = ULTRASONIC_TIM_Prescaler;
        TIM_TimeBaseInitTypeDefTure.TIM_RepetitionCounter = 0;
        TIM_TimeBaseInitTypeDefTure.TIM_ClockDivision = 0;
        TIM_TimeBaseInit(ULTRASONIC_TIM,&TIM_TimeBaseInitTypeDefTure);
        
        /* ��������ṹ�� */
        TIM_ICInitTypeDefTure.TIM_Channel = ULTRASONIC_TIM_TIM_Channel;
        TIM_ICInitTypeDefTure.TIM_ICFilter = 0;
        TIM_ICInitTypeDefTure.TIM_ICPolarity = TIM_ICPolarity_Rising;
        TIM_ICInitTypeDefTure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
        TIM_ICInitTypeDefTure.TIM_ICSelection = TIM_ICSelection_DirectTI;
        TIM_ICInit(ULTRASONIC_TIM,&TIM_ICInitTypeDefTure);
        
        TIM_PrescalerConfig(ULTRASONIC_TIM,ULTRASONIC_TIM_Prescaler,TIM_PSCReloadMode_Immediate);
        
        TIM_ITConfig(ULTRASONIC_TIM,TIM_IT_Update|TIM_IT_CC3,ENABLE);
        /* ʹ�ܶ�ʱ���ж� */
        NVIC_Configuration();
        TIM_Cmd(ULTRASONIC_TIM,ENABLE);
}

capture flag={0,0,0,0};
float distance = 0;
void Read_Distance(void)
{
        /* ֻ���ڳɹ������ʱ��Ÿ��¾���ֵ */
        if(flag.finish == 1)
        {
                /* ��ʱ������ֵ */
                distance = flag.updata_num *  ULTRASONIC_TIM_Period + flag.capture_val;
                /* �м���ֵ�����ʱ�䣬��λΪms ����ʽ�ɶ�ʱ����Ƶ���Ӿ��� */
                distance = distance / 1000.0;
                /* ���ٶȾ��빫ʽ����������룬ʱ������ʱ�䣬Ҫ����2 */
                distance = distance * 17;//ʱ�任��ɾ��� ��λΪcm
                
                /* debug */
                printf("%0.2f\n",distance);
                 
                /* ������һ�β�� */
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
                if(flag.measuring == 1)         //��ǵ�һ��������
                {
                        flag.updata_num++;
                        if(flag.updata_num >= 150)//ʱ��̫����
                        {
                                flag.updata_num = 150;
                                flag.measuring = 0;
                                flag.capture_val = 0;
                                flag.finish = 1;//����һ�β��
                                
                        }                        
                }               
                
        }
        
        if(TIM_GetITStatus(ULTRASONIC_TIM,TIM_IT_CC3) == SET)
        {
                if(flag.finish == 0)
                {

                        if(flag.measuring == 1 )        //��ǵ�һ��������
                        {
                               /* ���һ�γɹ����� */
                               flag.finish = 1;
                               /* ���־λ */
                               flag.measuring = 0;
                               flag.capture_val = ULTRASONIC_TIM->CCR3;
                               /* �ı䲶��ʽΪ�����ز��� */
                               ULTRASONIC_TIM->CCER&=~(1<<9);
                        }
                        else
                        {
                                /* ������ֵ���� */
                                ULTRASONIC_TIM->CNT = 0;
                                /* �������ֵ���� */
                                flag.updata_num = 0;
                                /* ��ǲ���һ�������� */
                                flag.measuring = 1;
                                /* �ı䲶��ʽΪ�½��ش��� */
                                ULTRASONIC_TIM->CCER|=1<<9;
                        }
                                
                        

                }
                TIM_ClearFlag(ULTRASONIC_TIM,TIM_IT_CC3);                     
        }
}
        
        
        
        
        