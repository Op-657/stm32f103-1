#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f10x.h"
#include "bsp_led.h"
#include <stdio.h>

// ����1-USART1
#define  DEBUG_USARTx                   USART1				//���ô���
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1		//����ʱ��
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd		//APB����ʱ��	����ʱ��
#define  DEBUG_USART_BAUDRATE           115200				//������

// USART GPIO ���ź궨��
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)		//���ӵ�GPIOA	
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd		//APB����ʱ��	��Ӧ����ʱ��
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA 				//���͵�GPIO  
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9			//���͵�GPIO��pinֵ
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA				//���յ�GPIO 
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10			//���յ�GPIO��pinֵ

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler

void USART_Config (void);
void USART_sent( USART_TypeDef*pUSARTx, uint8_t Data );
void USART_Sent(USART_TypeDef*pUSARTx,char *str);
#endif /* __BSP_USART_H*/
