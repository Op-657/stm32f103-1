#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_GeneralTim.h" 
#include "bsp_infrared_send.h"
#include "bsp_key.h"


#define CODE_SIZE               6
static const u8 Code_Tem_26_off[CODE_SIZE] = {0xb2,0x4d,0x7b,0x84,0xe0,0x1f};
static const u8 Code_Tem_26_on[CODE_SIZE] = {0xb2,0x4d,0xbf,0x40,0xd0,0x2f};
static const u8 Code_Tem_26_up[CODE_SIZE] = {0xb2,0x4d,0xbf,0x40,0xa0,0x5f};
static const u8 Code_Tem_26_down[CODE_SIZE] = {0xb2,0x4d,0xbf,0x40,0x70,0x8f};

int main(void)
{
        u8 key_state = 0;
        u8 on_off_flag = 0xff;
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        systick_init();
        usart_init();
        GENERAL_TIM_Init();
        Key_GPIO_Init();
        while(1)
        {

                key_state = Key_Scan();
                if(key_state == 1)
                {
                        if(~on_off_flag)
                        {
                                Send_infrared_code(Code_Tem_26_off,CODE_SIZE);
                                printf("1");
                        }
                        else
                        {
                                Send_infrared_code(Code_Tem_26_on,CODE_SIZE);
                                printf("1");
                        }
                }
                if(key_state == 2)
                {
                        Send_infrared_code(Code_Tem_26_up,CODE_SIZE);
                        printf("2");
                }
                if(key_state == 3)
                {
                        Send_infrared_code(Code_Tem_26_down,CODE_SIZE);
                        printf("3");
                }
                                
        }
}
