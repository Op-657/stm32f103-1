#ifndef __USART_HMI_H
#define	__USART_HMI_H


#include "stm32f10x.h"
#include <stdio.h>

#define  DEBUG_USARTx                   USART3
#define  DEBUG_USART_CLK                RCC_APB1Periph_USART3
#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           9600

// USART GPIO Òý½Åºê¶¨Òå
#define  DEBUG_USART_GPIO_CLK            (RCC_APB2Periph_GPIOB)
#define  DEBUG_USART_GPIO_APBxClkCmd     RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT         GPIOB   
#define  DEBUG_USART_TX_GPIO_PIN          GPIO_Pin_10
#define  DEBUG_USART_RX_GPIO_PORT         GPIOB
#define  DEBUG_USART_RX_GPIO_PIN          GPIO_Pin_11

#define  DEBUG_USART_IRQ                  USART3_IRQn
#define  DEBUG_USART_IRQHandler           USART3_IRQHandler




void USART_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

#endif /* __USART_H */
