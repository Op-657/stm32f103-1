 
#include "bsp_pwm_output.h" 

 /**
  * @brief  ����TIM3�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
static void TIMx_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* ����TIM3CLK Ϊ 72MHZ */
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
  macTIM_APBxClock_FUN (macTIM_CLK, ENABLE);

  /* GPIOA and GPIOB clock enable */
  macTIM_GPIO_APBxClock_FUN (macTIM_GPIO_CLK, ENABLE); 

  /*GPIOA Configuration: TIM3 channel 1 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  macTIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(macTIM_CH1_PORT, &GPIO_InitStructure);

  /*GPIOB Configuration: TIM3 channel 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  macTIM_CH2_PIN;
  GPIO_Init(macTIM_CH2_PORT, &GPIO_InitStructure);

  /*GPIOB Configuration: TIM3 channel 3 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  macTIM_CH3_PIN;
  GPIO_Init(macTIM_CH3_PORT, &GPIO_InitStructure);
	
  /*GPIOB Configuration: TIM3 channel 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  macTIM_CH4_PIN;
  GPIO_Init(macTIM_CH4_PORT, &GPIO_InitStructure);
	
}

/**
  * @brief  ����TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
  * @param  ��
  * @retval ��
  */
/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT ���¼���
 *                    TIMx_CCR(��ƽ�����仯)
 * �ź�����=(TIMx_ARR +1 ) * ʱ������
 * ռ�ձ�=TIMx_CCR/(TIMx_ARR +1)
 */
static void TIMx_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
 	u16 CCR1_Val = 0;        
 	u16 CCR2_Val = 0;
 	u16 CCR3_Val = 0;
 	u16 CCR4_Val = 0;

/* ----------------------------------------------------------------------- 
    macTIMx Channel1 duty cycle = (macTIMx_CCR1/ macTIMx_ARR+1)* 100% = 50%
    macTIMx Channel2 duty cycle = (macTIMx_CCR2/ macTIMx_ARR+1)* 100% = 37.5%
    macTIMx Channel3 duty cycle = (macTIMx_CCR3/ macTIMx_ARR+1)* 100% = 25%
    macTIMx Channel4 duty cycle = (macTIMx_CCR4/ macTIMx_ARR+1)* 100% = 12.5%
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 3600-1;       //10k
  TIM_TimeBaseStructure.TIM_Prescaler = 1;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
  TIM_TimeBaseInit(macTIMx, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
  TIM_OC1Init(macTIMx, &TIM_OCInitStructure);	 //ʹ��ͨ��1
  TIM_OC1PreloadConfig(macTIMx, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
  TIM_OC2Init(macTIMx, &TIM_OCInitStructure);	  //ʹ��ͨ��2
  TIM_OC2PreloadConfig(macTIMx, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//����ͨ��3�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
  TIM_OC3Init(macTIMx, &TIM_OCInitStructure);	 //ʹ��ͨ��3
  TIM_OC3PreloadConfig(macTIMx, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
  TIM_OC4Init(macTIMx, &TIM_OCInitStructure);	//ʹ��ͨ��4
  TIM_OC4PreloadConfig(macTIMx, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(macTIMx, ENABLE);			 // ʹ��TIM3���ؼĴ���ARR

  /* TIM3 enable counter */
  TIM_Cmd(macTIMx, ENABLE);                   //ʹ�ܶ�ʱ��3	
}

/**
  * @brief  TIM3 ���PWM�źų�ʼ����ֻҪ�����������
  *         TIM3���ĸ�ͨ���ͻ���PWM�ź����
  * @param  ��
  * @retval ��
  */
void TIMx_PWM_Init(void)
{
	TIMx_GPIO_Config();
	TIMx_Mode_Config();	
}

/*********************************************END OF FILE**********************/
