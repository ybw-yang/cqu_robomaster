#include "Task_Ultrasonic.h"

Ultrasonic ultra[8] = {0};
float const AVOID_VALUE = 0.323;
float avoidance_value[8][2] = {
	//此列为前后			//此列为左右
	{0, 						AVOID_VALUE	},
	{-AVOID_VALUE	,	AVOID_VALUE	},
	{-AVOID_VALUE	,	0						},
	{-AVOID_VALUE	, -AVOID_VALUE},
	{0						, -AVOID_VALUE},
	{AVOID_VALUE	, -AVOID_VALUE},
	{AVOID_VALUE	, 0						},
	{AVOID_VALUE	, AVOID_VALUE	}
};

void Ultrasonic_Init(void)
{
	int i=0;
	for(;i<8;i++)
	{
		ultra[i].direction = i;
	}
}

/*-----------------------中断处理-------------------------*/
 
void TIM2_IRQHandler(void)
{
	//
	if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
		ultra[7].start_flag = 1;
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		if(ultra[7].time_flag==0)
		{
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==1)
			{
				ultra[7].time1 = TIM2->CCR1;
				ultra[7].time_flag = 1;
			}
		}
		else if(ultra[7].time_flag==1)
		{
			ultra[7].time2 = TIM2->CCR1;
			ultra[7].time_flag = 0;
			ultra[7].distance = 0.34*(ultra[7].time2 - ultra[7].time1)/2;
		}
	}
	//
	if(TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
	{
		ultra[6].start_flag = 1;
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
		if(ultra[6].time_flag==0)
		{
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==1)
			{
				ultra[6].time1 = TIM_GetCapture2(TIM2);
				ultra[6].time_flag = 1;
			}
		}
		else if(ultra[6].time_flag==1)
		{
			ultra[6].time2 = TIM_GetCapture2(TIM2);
			ultra[6].time_flag = 0;
			ultra[6].distance = 0.34*(ultra[6].time2 - ultra[6].time1)/2;
		}
	}
	//
	if(TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
	{
		ultra[5].start_flag = 1;
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
		if(ultra[5].time_flag==0)
		{
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)==1)
			{
				ultra[5].time1 = TIM_GetCapture3(TIM2);
				ultra[5].time_flag = 1;
			}
		}
		else if(ultra[5].time_flag==1)
		{
			ultra[5].time2 = TIM_GetCapture3(TIM2);
			ultra[5].time_flag = 0;
			ultra[5].distance = 0.34*(ultra[5].time2 - ultra[5].time1)/2;
		}
	}
	//
	if(TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET)
	{
		ultra[4].start_flag = 1;
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
		if(ultra[4].time_flag==0)
		{
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)==1)
			{
				ultra[4].time1 = TIM_GetCapture4(TIM2);
				ultra[4].time_flag = 1;
			}
		}
		else if(ultra[4].time_flag==1)
		{
			ultra[4].time2 = TIM_GetCapture4(TIM2);
			ultra[4].time_flag = 0;
			ultra[4].distance = 0.34*(ultra[4].time2 - ultra[4].time1)/2;
		}
	}
	
}

void TIM8_CC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)//溢出
	{
		TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
		for(int i=0;i<4;i++)
		{
			if(ultra[i].time_flag==1)                  //当计数溢出时发生在高电平持续期间，需要考虑溢出对捕获的影响，来保证计算的时间正确
			{
				ultra[i].time_update_flag=1;
			}
		}
	}
	
	if(TIM_GetFlagStatus(TIM8, TIM_FLAG_CC1)==SET)//前方超声波测距置位		
	{
		ultra[3].start_flag = 1;
		if(ultra[3].time_flag==0)
		{
			if( GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_5)==1)
			{  
				ultra[3].time1 = TIM8->CCR1;		
        TIM_ClearITPendingBit(TIM8, TIM_IT_CC1);
			}	
      else
			{
        TIM_ClearITPendingBit(TIM8, TIM_IT_CC1);
				ultra[3].time_flag--;
			}				
		} 
		else if(ultra[3].time_flag==1)
		{
			if(ultra[3].time_update_flag==0)               //捕获期间没有发生溢出中断
		    {
			   ultra[3].time2 = TIM_GetCapture1(TIM8);
					 ultra[3].distance=0.34*(ultra[3].time2-ultra[3].time1)/2;			
		    }	
		  else if(ultra[3].time_update_flag==1)                  //捕获期间发生溢出中断
		   {
			   ultra[3].time2 = TIM8->CCR1;
			   ultra[3].distance=0.34*(65535+ultra[3].time2-ultra[3].time1)/2;
				 ultra[3].time_update_flag=0;
		   }
			 TIM_ClearITPendingBit(TIM8, TIM_IT_CC1);
		}
		ultra[3].time_flag++;
		if(ultra[3].time_flag==2)
			ultra[3].time_flag = 0;
	}	
	
	if(TIM_GetFlagStatus(TIM8, TIM_FLAG_CC2)==SET)//
	{
		ultra[2].start_flag = 1;
		if(ultra[2].time_flag==0)
		{
			if( GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_6)==1)
			{  
				ultra[2].time1 = TIM8->CCR2;		
        TIM_ClearITPendingBit(TIM8, TIM_IT_CC2);
			}	
      else
			{
        TIM_ClearITPendingBit(TIM8, TIM_IT_CC2);
				ultra[2].time_flag--;
			}				
		} 
		else if(ultra[2].time_flag==1)
		{
			if(ultra[2].time_update_flag==0)               
		    {
			     ultra[2].time2 = TIM8->CCR2;
					 ultra[2].distance=0.34*(ultra[2].time2-ultra[2].time1)/2;			
		    }	
		  else if(ultra[2].time_update_flag==1)                 
		   {
			     ultra[2].time2 = TIM8->CCR2;
			     ultra[2].distance=0.34*(65535+ultra[2].time2-ultra[2].time1)/2;
				   ultra[2].time_update_flag=0;
		   }
			 TIM_ClearITPendingBit(TIM8, TIM_IT_CC2);
		}
		 ultra[2].time_flag++;
		 if(ultra[2].time_flag==2)
			  ultra[2].time_flag = 0;
	}	
	
	if(TIM_GetFlagStatus(TIM8, TIM_FLAG_CC3)==SET)
	{
		ultra[1].start_flag = 1;
		if(ultra[1].time_flag==0)
		{
			if( GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_7)==1)
			{  
				ultra[1].time1 = TIM8->CCR3;		
        TIM_ClearITPendingBit(TIM8, TIM_IT_CC3);
			}	
      else
			{
        TIM_ClearITPendingBit(TIM8, TIM_IT_CC3);
				ultra[1].time_flag--;
			}				
		} 
		else if(ultra[1].time_flag==1)
		{
			if(ultra[1].time_update_flag==0)               
		    {
			     ultra[1].time2 = TIM8->CCR3;;
					 ultra[1].distance=0.34*(ultra[1].time2-ultra[1].time1)/2;			
		    }	
		  else if(ultra[1].time_update_flag==1)                  
		   {
			   ultra[1].time2 = TIM8->CCR3;
			   ultra[1].distance=0.34*(65535+ultra[1].time2-ultra[1].time1)/2;
				 ultra[1].time_update_flag=0;
		   }
			 TIM_ClearITPendingBit(TIM8, TIM_IT_CC3);
		}
		ultra[1].time_flag++;
		if(ultra[1].time_flag==2)
			ultra[1].time_flag = 0;
	}	
	
	if(TIM_GetFlagStatus(TIM8, TIM_FLAG_CC4)==SET)
	{
		ultra[0].start_flag = 1;
		if(ultra[0].time_flag==0)
		{
			if( GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_2)==1)
			{  
				ultra[0].time1 = TIM8->CCR4;		
        TIM_ClearITPendingBit(TIM8, TIM_IT_CC4);
			}	
      else
			{
        TIM_ClearITPendingBit(TIM8, TIM_IT_CC4);
				ultra[0].time_flag--;
			}				
		} 
		else if(ultra[0].time_flag==1)
		{
			if(ultra[0].time_update_flag==0)              
		    {
			     ultra[0].time2 = TIM8->CCR4;
					 ultra[0].distance=0.34*(ultra[0].time2-ultra[0].time1)/2;			
		    }	
		  else if(ultra[0].time_update_flag==1)                  
		   {
			   ultra[0].time2 = TIM8->CCR4;
			   ultra[0].distance=0.34*(65535+ultra[0].time2-ultra[0].time1)/2;
				 ultra[0].time_update_flag=0;
		   }
			 TIM_ClearITPendingBit(TIM8, TIM_IT_CC4);
		}
		ultra[0].time_flag++;
		if(ultra[0].time_flag==2)
			ultra[0].time_flag = 0;
	}	

}

//1ms检测1次
void Task_Ultrasonic(void)
{ 
	portTickType	CurrentControlTick = 0;
	int i = 0;
	for(;;)
	{
		CurrentControlTick = xTaskGetTickCount();

		for(i=0;i<8;i++)
		{
			if(ultra[i].distance <= DISTANCE && ultra[i].start_flag)// && ultra[(i+4)%8].distifance > DISTANCE) //某一边小于给定值且其对边大于给定值
				ultra[i].crash_flag = 1;
			else if(ultra[i].distance > DISTANCE)
				ultra[i].crash_flag = 0;
		}		
		
		if(separate_flag)
		{
			for(int i=0;i<8;i++)
			{
				if(ultra[i].crash_flag)
				{
					radio_ahead_back_data_avoidance += avoidance_value[i][0];
					radio_left_right_data_avoidance += avoidance_value[i][1];
					radio_positive_spin_data = 90;
				}
			}
			if(radio_ahead_back_data_avoidance >= RADIO_DATA_MAX)	radio_ahead_back_data_avoidance=RADIO_DATA_MAX;
			if(radio_ahead_back_data_avoidance <= -RADIO_DATA_MAX)	radio_ahead_back_data_avoidance=-RADIO_DATA_MAX;	
			if(radio_left_right_data_avoidance >= RADIO_DATA_MAX)	radio_left_right_data_avoidance=RADIO_DATA_MAX;
			if(radio_left_right_data_avoidance <= -RADIO_DATA_MAX) radio_left_right_data_avoidance=-RADIO_DATA_MAX;
		}
		else
		{
			radio_ahead_back_data_avoidance = 0;
			radio_left_right_data_avoidance = 0;
			radio_positive_spin_data = 105;

		}
		
		vTaskDelayUntil(&CurrentControlTick, 1 / portTICK_RATE_MS);
	}
}

//定时0.1s触发超声波测距
int cntms = 0;
void TIM7_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM7,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
		GPIO_ResetBits(GPIOI,GPIO_Pin_0);
		cntms++;
		if(cntms>=10000)//0.1s测距一次
		{
			cntms=0;
			TIM_SetCounter(TIM8, 0);
			GPIO_SetBits(GPIOI,GPIO_Pin_0);
				
		}
	}
}
//


