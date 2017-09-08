#include "bsp_infrared_send.h"

/* ���ɺ���Э�����ʼ�ź� */
static void Start_signal(void)
{
        /* ʹ��38kHz���β���ʹ���ն˵�ƽ���� */
        TIM_x->TIM_CCR_x = 15;
        delay_ms(5);
        /* ʧ�ܾ��β���ʹ���ն˵�ƽ���� */
        TIM_x->TIM_CCR_x = 0;
        delay_ms(5);
}

/* ���ɺ���Э���ܶ���Ϊ 1 �Ĳ��� */
static void Creat_bit_1(void)
{
        TIM_x->TIM_CCR_x = 15;
        delay_us(500);
        TIM_x->TIM_CCR_x = 0;
        delay_us(1500);
}

/* ���ɺ���Э���ܶ���Ϊ 0 �Ĳ��� */
static void Creat_bit_0(void)
{
        TIM_x->TIM_CCR_x = 15;
        delay_us(500);
        TIM_x->TIM_CCR_x = 0;
        delay_us(500);
}

static void End_signal(void)
{
        TIM_x->TIM_CCR_x = 15;
        delay_us(500);       
        TIM_x->TIM_CCR_x = 0;
        delay_ms(5);
}


/* �������ɲ��κ��� */
void Send_infrared_code(const u8 * code, u8 code_len)
{
        char i = 0, j = 0, k = 0;
        const u8 * p = code;
        /* ִ��һ�Σ��������α��� */
        for(k = 0; k < 2; k++)
        {
                Start_signal();
                /* ѭ������������ֽ��� */
                for(j = 0; j < code_len; j++)
                {
                        /* ѭ������������ÿ���ֽڵ�ÿһ��bit */
                        for(i = 0; i < 8; i++)
                        {
                                if((*p)&(0x80>>i))
                                       Creat_bit_1(); 
                                else
                                       Creat_bit_0();
                        }
                        p++;
                }
                End_signal();
                p = code;
                
        }        
}








