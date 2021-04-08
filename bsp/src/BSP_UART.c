
#include "main.h"
u8 auxiliary_flag=0;
float Buff_yaw, Buff_pitch,buff_yaw;
int rev[6]={0};
u8 Re_flag = 0;
	
/*---------------------------------------重映射printf------------------------------------------------------*/  

#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
	USART3->DR = (u8) ch;      
	return ch;
}
#endif
	
/*---------------------------------------------uart初始化----------------------------------------------------*/	

/**
  * @brief  UART初始化
  * @param  void
  * @retval void
  */
void BSP_UART3_InitConfig(u32 bound)
{
    GPIO_InitTypeDef	GPIO_InitStrucutre;
	USART_InitTypeDef	USART_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	/*			enable Colck port				*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3);
	
	/*			config GPIO pin				*/
	GPIO_InitStrucutre.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;		
	GPIO_InitStrucutre.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStrucutre.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStrucutre.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStrucutre.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStrucutre);
	
	/*			config USART3				*/
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3,&USART_InitStructure);
	
	/*		enable USART3			*/
	USART_Cmd(USART3,ENABLE);
	/*			clear transimission complete flag		*/
	USART_ClearFlag(USART3,USART_FLAG_TC);
	
#if	EN_USART3_RX
		/*			open Rx interrupt			*/
		USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
		/*			config NVIC control				*/
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_Init(&NVIC_InitStructure);
#endif

}

/*-------------------------------用于MPU9250-----------------------------------*/
void BSP_UART6_InitConfig(u32 bound)
{
	GPIO_InitTypeDef	GPIO_InitStrucutre;
	USART_InitTypeDef	USART_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	/*			enable Colck port				*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);

	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6);
	
	/*			config GPIO pin				*/
	GPIO_InitStrucutre.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_9;		
	GPIO_InitStrucutre.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStrucutre.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStrucutre.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStrucutre.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStrucutre);
	
	/*			config USART6				*/
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART6,&USART_InitStructure);
	
	/*		enable USART6			*/
	USART_Cmd(USART6,ENABLE);
	/*			clear transimission complete flag		*/
	USART_ClearFlag(USART6,USART_FLAG_TC);
	
	/*			open Rx interrupt			*/
	USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);
	/*			config NVIC control				*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);

}

void USART6_IRQHandler(void)
{
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
  {
		CopeSerial2Data((int)USART6->DR);//处理数据
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);
  }
	 if (USART_GetFlagStatus(USART6, USART_FLAG_PE) != RESET)  
   {  
       USART_ReceiveData(USART6);  
     USART_ClearFlag(USART6, USART_FLAG_PE);  
   }  
      
   if (USART_GetFlagStatus(USART6, USART_FLAG_ORE) != RESET)  
   {  
       USART_ReceiveData(USART6);  
     USART_ClearFlag(USART6, USART_FLAG_ORE);  
   }  
      
    if (USART_GetFlagStatus(USART6, USART_FLAG_FE) != RESET)  
   {  
       USART_ReceiveData(USART6);  
      USART_ClearFlag(USART6, USART_FLAG_FE);  
   }   
}
/*----------------------------------------------------------------------------------*/

void USART3_IRQHandler(void) //步兵车辅助瞄准
{
	u16 sum=0, judge =0;
	u8 RX_flag = 0;
//	u16 sendtest = 0;
	u16 Test=0;
	u8 static mark=0;
	if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET )
	{
		if(separate_flag)
		{
			RX_flag = USART_ReceiveData(USART3);
			switch(mark)
				{
					case 0: if(RX_flag == 0xFF) mark++;break;
					case 1: if(RX_flag == 0x00) mark++;break;
					case 2: rev[0] = USART_ReceiveData(USART3);mark++;break;
					case 3: rev[1] = USART_ReceiveData(USART3);mark++;break;
					case 4: rev[2] = USART_ReceiveData(USART3);mark++;break;
					case 5: rev[3] = USART_ReceiveData(USART3);mark++;break;
					case 6: rev[4] = USART_ReceiveData(USART3);mark++;break;
					case 7: rev[5] = USART_ReceiveData(USART3);mark=0;
					sum = (rev[1]<<8|rev[0])+(rev[3]<<8|rev[2])+255;
					judge = rev[5]<<8|rev[4];
					if(sum == judge)
					{
						if(rev[1]&0x80)//如果是负数，求原码
						{
							Test = (rev[1]<<8)|rev[0];
							Test--;
							Test = ~Test;
							rev[0] = Test&0x00FF;
							rev[1] = (Test>>8)&0x00FF;
							Test = 0;
							Buff_yaw = -((rev[1]<<8)|rev[0])/100.0;
						}
						else Buff_yaw = ((rev[1]<<8)|rev[0])/100.0;
						
						if(rev[3]&0x80)
						{
							Test = (rev[3]<<8)|rev[2];
							Test--;
							Test = ~Test;
							rev[2] = Test&0x00FF;
							rev[3] = (Test>>8)&0x00FF;
							Test = 0;
							Buff_pitch = -((rev[3]<<8)|rev[2])/100.0;
						}
						else Buff_pitch = ((rev[3]<<8)|rev[2])/100.0;
						Re_flag = 1;
						Graph_flag=1;
						Graph_flag_flt[0] = Graph_flag;
						if(sendgunFlag==0) sendgunFlag = 1;
						gpyo_register = MPU6500_Yaw_turn_baiwei;
						radio_turn_data_straight = 0;
						break;
					}
					else break;
				}
			}
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);					//clear IT pending flag
		
	}

}
/*-------------------------------------------*/
