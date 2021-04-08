#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "main.h"

#define EN_USART3_RX					1

extern u8 Re_flag;
extern u8 auxiliary_flag;
extern float Buff_yaw, Buff_pitch;

void BSP_UART3_InitConfig(u32 bound);
void BSP_UART6_InitConfig(u32 bound);


#endif
