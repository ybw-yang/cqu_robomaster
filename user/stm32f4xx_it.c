#include "main.h"
#include "stm32f4xx_it.h"


/*---------------------------------------------------CAN1(底盘+17mm武器)-----------------------------------------------------------*/


CanRxMsg CanRxData;     //CAN接收数据存储缓存

short Gun_Real_Angle_Value = 0;
short Gun_Real_Speed_Value = 0;

short Real_Angle_Value[4] = {0};
short Real_Speed_Value[4] = {0};
long Real_Position_Value[4] = {0};

short Real_Current_Value[4] = {0};
short Real_Velocity_Value[4] = {0};

char Real_Online[4] = {0};
char Real_Ctl1_Value[4] = {0};
char Real_Ctl2_Value[4] = {0};


void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg rx_message;
    
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	{
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
        
        
            if(rx_message.StdId == 0x201)
            {
                Real_Angle_Value[0] = (rx_message.Data[0]<<8)|(rx_message.Data[1]);
                Real_Speed_Value[0] = (rx_message.Data[2]<<8)|(rx_message.Data[3]);
                Real_Position_Value[0] = ((rx_message.Data[4]<<24)|(rx_message.Data[5]<<16)|(rx_message.Data[6]<<8)|(rx_message.Data[7]));
            }
            else if(rx_message.StdId == 0x202)
            {
                Real_Angle_Value[1] = (rx_message.Data[0]<<8)|(rx_message.Data[1]);
                Real_Speed_Value[1] = (rx_message.Data[2]<<8)|(rx_message.Data[3]);
                Real_Position_Value[1] = ((rx_message.Data[4]<<24)|(rx_message.Data[5]<<16)|(rx_message.Data[6]<<8)|(rx_message.Data[7]));
            }
            else if(rx_message.StdId == 0x203)
            {
                Real_Angle_Value[2] = (rx_message.Data[0]<<8)|(rx_message.Data[1]);
                Real_Speed_Value[2] = (rx_message.Data[2]<<8)|(rx_message.Data[3]);
                Real_Position_Value[2] = ((rx_message.Data[4]<<24)|(rx_message.Data[5]<<16)|(rx_message.Data[6]<<8)|(rx_message.Data[7]));
            }
            else if(rx_message.StdId == 0x204)
            {
                Real_Angle_Value[3] = (rx_message.Data[0]<<8)|(rx_message.Data[1]);
                Real_Speed_Value[3] = (rx_message.Data[2]<<8)|(rx_message.Data[3]);
                Real_Position_Value[3] = ((rx_message.Data[4]<<24)|(rx_message.Data[5]<<16)|(rx_message.Data[6]<<8)|(rx_message.Data[7]));
            }
						else if(rx_message.StdId == 0x205)
            {
                Gun_Real_Angle_Value = (rx_message.Data[0]<<8)|(rx_message.Data[1]);
                Gun_Real_Speed_Value = (rx_message.Data[2]<<8)|(rx_message.Data[3]);
            }
						//---------------以上是底盘电机的四个轮子的数据------------
						//edit
						 if(rx_message.StdId == 0x1B)
            {
                Real_Current_Value[0] = (rx_message.Data[0]<<8)|(rx_message.Data[1]);
                Real_Velocity_Value[0] = (rx_message.Data[2]<<8)|(rx_message.Data[3]);
                Real_Position_Value[0] = ((rx_message.Data[4]<<24)|(rx_message.Data[5]<<16)|(rx_message.Data[6]<<8)|(rx_message.Data[7]));
            }
            else if(rx_message.StdId == 0x2B)
            {
                Real_Current_Value[1] = (rx_message.Data[0]<<8)|(rx_message.Data[1]);
                Real_Velocity_Value[1] = (rx_message.Data[2]<<8)|(rx_message.Data[3]);
                Real_Position_Value[1] = ((rx_message.Data[4]<<24)|(rx_message.Data[5]<<16)|(rx_message.Data[6]<<8)|(rx_message.Data[7]));
            }
            else if(rx_message.StdId == 0x3B)
            {
                Real_Current_Value[2] = (rx_message.Data[0]<<8)|(rx_message.Data[1]);
                Real_Velocity_Value[2] = (rx_message.Data[2]<<8)|(rx_message.Data[3]);
                Real_Position_Value[2] = ((rx_message.Data[4]<<24)|(rx_message.Data[5]<<16)|(rx_message.Data[6]<<8)|(rx_message.Data[7]));
            }
            else if(rx_message.StdId == 0x4B)
            {
                Real_Current_Value[3] = (rx_message.Data[0]<<8)|(rx_message.Data[1]);
                Real_Velocity_Value[3] = (rx_message.Data[2]<<8)|(rx_message.Data[3]);
                Real_Position_Value[3] = ((rx_message.Data[4]<<24)|(rx_message.Data[5]<<16)|(rx_message.Data[6]<<8)|(rx_message.Data[7]));
            }
            else if(rx_message.StdId == 0x1F)
            {
                Real_Online[0] = 1;
            }
            else if(rx_message.StdId == 0x2F)
            {
                Real_Online[1] = 1;
            }
            else if(rx_message.StdId == 0x3F)
            {
                Real_Online[2] = 1;
            }
            else if(rx_message.StdId == 0x4F)
            {
                Real_Online[3] = 1;
            }
            else if(rx_message.StdId == 0x1C)
            {
                Real_Ctl1_Value[0] = rx_message.Data[0];
                Real_Ctl2_Value[0] = rx_message.Data[1];
            }
            else if(rx_message.StdId == 0x2C)
            {
                Real_Ctl1_Value[1] = rx_message.Data[0];
                Real_Ctl2_Value[1] = rx_message.Data[1];
            }
            else if(rx_message.StdId == 0x3C)
            {
                Real_Ctl1_Value[2] = rx_message.Data[0];
                Real_Ctl2_Value[2] = rx_message.Data[1];
            }
            else if(rx_message.StdId == 0x4C)
            {
                Real_Ctl1_Value[3] = rx_message.Data[0];
                Real_Ctl2_Value[3] = rx_message.Data[1];
							
            }
        }
}



void CAN1_TX_IRQHandler(void) //CAN TX
{
    if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
	{
	     CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
       
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------*/
/*************************************************************************
                          CAN2_RX0_IRQHandler
描述：用于云台测试的 can总线接受函数
*************************************************************************/

float  now_current_yaw;
float  yaw_angle_0_360;
float  yaw_angle;
float  now_current_pitch;
float  pitch_angle_0_360;
float  pitch_angle;

float  pitch_speed;
float  yaw_speed;
float  pitch_angle_difference[2]={0};
float  yaw_angle_difference[2]={0};

float  pitch_difference[2]={0};
float  yaw_difference[2]={0};
float  yaw_speed_100;
float  pitch_speed_100;
int static conter = 0;
float pitch_angle_0_360_flt[2] = {0};
float yaw_angle_0_360_flt[2] = {0};

void  CAN2_RX0_IRQHandler(void)
{
    CanRxMsg rx_message;
    if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET) 
    {
      CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
      CAN_Receive(CAN2, CAN_FIFO0, &rx_message); 
			
			conter++;
			
		//pitch数据	
		if(rx_message.StdId == 0x205) //pitch
        {    					
			now_current_pitch=((rx_message.Data[0] << 8) | (rx_message.Data[1])) & 0xffff;  //获得云台电机0x205的码盘值   
			pitch_angle_0_360=(now_current_pitch/0x1fff)*360; //获得云台电机0x205的角度值
			pitch_angle	= first_order_filter( pitch_angle_0_360_flt, pitch_angle_0_360 ,pitch_angle, 359);
			/*----------------------------------------------------------------------*/
			pitch_angle_difference[0] = pitch_angle_difference[1];
		  pitch_angle_difference[1] = pitch_angle ;
			pitch_speed = pitch_angle_difference[1] - pitch_angle_difference[0];
			/*----------------------------------------------------------------------*/
			
			if(conter==50)
				{
					pitch_difference[0] = pitch_angle_difference[0];
				}
				if(conter==100)
				{
					pitch_difference[1] = pitch_angle_difference[1];
					pitch_speed_100=pitch_difference[1]-pitch_difference[0];
				}
				
		}
		
		//yaw数据
       if(rx_message.StdId == 0x206) //yaw
        {    
			LED_GREEN_ON;	
			now_current_yaw = ((rx_message.Data[0] << 8) | (rx_message.Data[1])) & 0xffff;  //获得云台电机0x206的码盘值   
	
			yaw_angle_0_360 = (now_current_yaw/0x1fff)*360; //获得云台电机0x206的角度值  
			yaw_angle = first_order_filter( yaw_angle_0_360_flt, yaw_angle_0_360 ,yaw_angle, 359);
		/*----------------------------------------------------------------------*/
			yaw_angle_difference[0] = yaw_angle_difference[1];
		  yaw_angle_difference[1] = yaw_angle ;
			yaw_speed = yaw_angle_difference[1] - yaw_angle_difference[0];			
		/*----------------------------------------------------------------------*/			
        }	
				if(conter==50)
				{
					yaw_difference[0] = yaw_angle_difference[0];
				}
				if(conter==100)
				{
					yaw_difference[1] = yaw_angle_difference[1];
					yaw_speed_100=yaw_difference[1]-yaw_difference[0];
					conter=0;
				}

   }
}

/*---------------------------------------------------------DBUS------------------------------------------------------------------------*/

uint8_t USART1temp;     //用于清空串口标志位的临时变量(空闲中断使用赋值来清空)

/**
  * @brief  DBUS空闲中断(USART1)
  * @param  void
  * @retval void
  */
void USART1_IRQHandler(void)
{
    USART1temp = USART1->DR;
    USART1temp = USART1->SR;
    
    DMA_Cmd(DMA2_Stream2, DISABLE);
    
    //数据量正确
    if(DMA2_Stream2->NDTR == DBUSBackLength)
    {
        DBUSFrameCounter++;         //帧数增加
        DBUS_DataDecoding();        //解码
    }
    
    //重启DMA
    DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);
    while(DMA_GetCmdStatus(DMA2_Stream2) != DISABLE);
    DMA_SetCurrDataCounter(DMA2_Stream2, DBUSLength + DBUSBackLength);
    DMA_Cmd(DMA2_Stream2, ENABLE);
}

/*---------------------------------------------------------DBUS------------------------------------------------------------------------*/

void DMA2_Stream2_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA2_Stream2, DMA_IT_TCIF2))
    {
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
		DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);

  	decode_flag=1;
	}
}
































/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
// 
//}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
