#ifndef _BSP_INFRARED_SEND_H_
#define _BSP_INFRARED_SEND_H_
#include <stm32f10x.h>
#include "bsp_systick.h"

#define TIM_x           TIM3
#define TIM_CCR_x       CCR1     
void Send_infrared_code(const u8 * code, u8 code_len);
#endif

