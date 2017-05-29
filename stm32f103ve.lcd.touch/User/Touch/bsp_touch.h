#ifndef __BSP_TOUCH_H
#define __BSP_TOUCH_H

#include "stm32f10x.h"


/******************************* XPT2046 �����������ź�ָʾ���Ŷ���(��ʹ���ж�) ***************************/
#define             Touch_PENIRQ_GPIO_CLK                        RCC_APB2Periph_GPIOE   
#define             Touch_PENIRQ_GPIO_PORT                       GPIOE
#define             Touch_PENIRQ_GPIO_PIN                        GPIO_Pin_4

//�����ź���Ч��ƽ
#define             Touch_PENIRQ_ActiveLevel                     0
#define             Touch_PENIRQ_Read()                          GPIO_ReadInputDataBit ( Touch_PENIRQ_GPIO_PORT, Touch_PENIRQ_GPIO_PIN )



/******************************* XPT2046 ������ģ��SPI���Ŷ��� ***************************/
#define             Touch_SPI_GPIO_CLK                         RCC_APB2Periph_GPIOE| RCC_APB2Periph_GPIOD

#define             Touch_SPI_CS_PIN		                        GPIO_Pin_13
#define             Touch_SPI_CS_PORT		                      GPIOD

#define	            Touch_SPI_CLK_PIN	                        GPIO_Pin_0
#define             Touch_SPI_CLK_PORT	                        GPIOE

#define	            Touch_SPI_MOSI_PIN	                        GPIO_Pin_2
#define	            Touch_SPI_MOSI_PORT	                      GPIOE

#define	            Touch_SPI_MISO_PIN	                        GPIO_Pin_3
#define	            Touch_SPI_MISO_PORT	                      GPIOE


#define             Touch_CS_DISABLE()                 	 GPIO_SetBits ( Touch_SPI_CS_PORT, Touch_SPI_CS_PIN )    
#define             Touch_CS_ENABLE()                          GPIO_ResetBits ( Touch_SPI_CS_PORT, Touch_SPI_CS_PIN )  

#define             Touch_CLK_HIGH()                           GPIO_SetBits ( Touch_SPI_CLK_PORT, Touch_SPI_CLK_PIN )    
#define             Touch_CLK_LOW()                            GPIO_ResetBits ( Touch_SPI_CLK_PORT, Touch_SPI_CLK_PIN ) 

#define             Touch_MOSI_1()                             GPIO_SetBits ( Touch_SPI_MOSI_PORT, Touch_SPI_MOSI_PIN ) 
#define             Touch_MOSI_0()                             GPIO_ResetBits ( Touch_SPI_MOSI_PORT, Touch_SPI_MOSI_PIN )

#define             Touch_MISO()                               GPIO_ReadInputDataBit ( Touch_SPI_MISO_PORT, Touch_SPI_MISO_PIN )

#define	            LCD_CHANNEL_X 	                          0x90 	          //ͨ��Y+��ѡ�������	
#define	            LCD_CHANNEL_Y 	                          0xd0	          //ͨ��X+��ѡ�������


//��� ���������İ��º��ͷ�

#define TOUCH_NOT_PRESSED  0
#define TOUCH_PRESSED      1

//��������д��FLASH��ı�־
#define		FLASH_TOUCH_PARA_FLAG_VALUE			 0xA5

//������־д��FLASH��ĵ�ַ
#define 	FLASH_TOUCH_PARA_FLAG_ADDR			(1*1024)

//��������д��FLASH��ĵ�ַ
#define 	FLASH_TOUCH_PARA_ADDR				(2*1024)

#define LCD_DEBUG_ON         0
#define LCD_INFO(fmt,arg...)           printf("<<-XPT2046-INFO->> "fmt"\n",##arg)
#define LCD_ERROR(fmt,arg...)          printf("<<-XPT2046-ERROR->> "fmt"\n",##arg)


//#define XPT2046_DEBUG(fmt,arg...)          do{\
//                                          if(XPT2046_DEBUG_ON)\
//                                          printf("<<-XPT2046-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
//                                          }while(0)

typedef enum
{
	TOUCH_STATE_RELEASE = 0,
	TOUCH_STATE_WAITING,
	TOUCH_STATE_PRESSED,
}Touch_State;


typedef struct         //У׼���ӽṹ�� 
{
	 float 		An,  		 //ע:sizeof(long double) = 8
			Bn,     
			Cn,   
			Dn,    
			En,    
			Fn,     
			Divider;
	
} strType_LCD_Calibration;



typedef struct         //У׼ϵ���ṹ�壨����ʹ�ã�
{
	float 	dX_X,  			 
		dX_Y,     
		dX,   
		dY_X,    
		dY_Y,    
		dY;

} strType_LCD_TouchPara;

typedef	struct          //Һ������ṹ�� 
{		
	/*����ֵ��ʾ��������*/
   int16_t x;			//��¼���µĴ�������ֵ
   int16_t y; 
	
	/*���ڼ�¼��������ʱ(����)����һ�δ���λ��*/
   int16_t pre_x;		
   int16_t pre_y;
	
} strType_LCD_Coordinate;  

/**************************��������********************/

uint16_t Touch_ReceiveData(void);
void Touch_SendCMD(uint8_t cmd)	;
void TOUCH_Init(void);
uint8_t touch_detect(void);
uint8_t LCD_Touch_Calibrate ( uint8_t LCD_Mode ) ;
void Calibrate_or_Get_TouchParaWithFlash(uint8_t LCD_Mode,uint8_t forceCal);
uint8_t LCD_Get_TouchedPoint ( strType_LCD_Coordinate * pDisplayCoordinate, strType_LCD_TouchPara * pTouchPara );
void LCD_TouchDown(strType_LCD_Coordinate * touch);
void LCD_TouchUp(strType_LCD_Coordinate * touch) ;
void LCD_TouchEvenHandler(void );
void Screen_Init(void);

#endif /* __BSP_TOUCH_H*/
