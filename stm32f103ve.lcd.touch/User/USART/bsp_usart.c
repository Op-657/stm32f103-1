#include "bsp_usart.h"

//static void NVIC_Configuration(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure;
//  
//  /* Ƕ�������жϿ�������ѡ�� */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//  
//  /* ����USARTΪ�ж�Դ */
//  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
//  /* �������ȼ�*/
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  /* �����ȼ� */
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  /* ʹ���ж� */
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  /* ��ʼ������NVIC */
//  NVIC_Init(&NVIC_InitStructure);
//}

void USART_Config (void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;      //�������ӵ���GPIO
	USART_InitTypeDef  USART_InitStructure;		//���ô��ڳ�ʼ��
	
	// �򿪴���GPIO��ʱ��
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	// �򿪴��������ʱ��
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//�������
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;	//PA9   PA10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//����Ӳ��������
	USART_InitStructure.USART_Mode  =   USART_Mode_Rx | USART_Mode_Tx;			//���շ�������
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;				//������
	USART_InitStructure.USART_Parity = USART_Parity_No;									//У��
	USART_InitStructure.USART_StopBits = USART_StopBits_1;					//ֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;				//�ֳ�
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
//	// �����ж����ȼ�����
//	NVIC_Configuration();
//	
//	// ʹ�ܴ��ڽ����ж�
//	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	
//	
	// ʹ�ܴ���
	USART_Cmd(DEBUG_USARTx, ENABLE);
}
	

void USART_sent( USART_TypeDef*pUSARTx, uint8_t Data )			//�����ַ�����
{
	USART_SendData(pUSARTx,Data); 
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	//�ȴ��������
}


void USART_Sent(USART_TypeDef*pUSARTx,char *str)			//�����ַ�������
{
	unsigned int k = 0;
	do
	{
		USART_sent( pUSARTx, *(str+k) );
		k++;
	}while (*(str+k)!='\0');
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);	//�ȴ��������
}

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_sent(DEBUG_USARTx, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}


