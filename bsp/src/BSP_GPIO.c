#include "main.h"

/**
  * @brief  GPIO初始化
  * @param  void
  * @retval void 
  */
void BSP_GPIO_InitConfig(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	GPIO_InitTypeDef	LASER; 
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
	
	//超声波
	//GPIOI 0 配置触产生触发发超声波工作的信号
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOI,&GPIO_InitStructure); //初始化
	GPIO_ResetBits(GPIOI,GPIO_Pin_0);
	
	//红灯
	GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin    =   GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_7);
 
	//绿灯
	GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin    =   GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOF,GPIO_Pin_14);

	//USART3
	GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin    =   GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
	
	//CAN1
	GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin    =   GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);
	
	
	//CAN2
	GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin    =   GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);
	
	
	//USART1
	GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin    =   GPIO_Pin_7;
	GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
				
	LASER.GPIO_Pin = GPIO_Pin_13;	
	LASER.GPIO_Mode = GPIO_Mode_OUT;
	LASER.GPIO_OType = GPIO_OType_PP;
	LASER.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOG, &LASER);
	GPIO_SetBits(GPIOG,GPIO_Pin_13);//cyq
	
	LED_GREEN_ON;
}


