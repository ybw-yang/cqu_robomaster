#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

void BSP_GPIO_InitConfig(void);

#define LED_GREEN_OFF   GPIO_SetBits(GPIOF,GPIO_Pin_14)
#define LED_GREEN_ON  GPIO_ResetBits(GPIOF,GPIO_Pin_14)

#define LED_RED_OFF   GPIO_SetBits(GPIOE,GPIO_Pin_7)
#define LED_RED_ON  GPIO_ResetBits(GPIOE,GPIO_Pin_7)


#endif
