#include "Task_Weapons.h"

int sendgunFlag = 0;
u32	position_gun=0;
int Dial_Wheel_target_speed_value = 0;
u8  mocaflag=0;
float dial_control=0;
int  prenum=0,currentnum=0,prenum_yaokong=3,currentnum_yaokong=3;
u8	RC_Ctl_s1_flt[2]={0};

/*-------------------------------------------------------------------------------------------------------------------
函数名  ：weapon_ctrl（）
函数变量 :
功能    ：武器控制函数
日期    ：2017.4.18
作者    ：杨博炜（改）
-----------------------------------------------------------------------------------------------------------------------*/
void weapon_ctrl(void)
{
	/*---------------------------------------- 武器-----------------------------------------------*/
		if(mocaflag==1)
		{
			prenum=currentnum;
			currentnum=DBUSBuffer[12];
		
			prenum_yaokong=currentnum_yaokong;
			currentnum_yaokong=DBUS_ReceiveData.switch_right;
		
			if(DBUSBuffer[12]==1||((prenum_yaokong-currentnum_yaokong)==2))
			{
				sendgunFlag = 1;
				Dial_Wheel_target_speed_value = dial_speed;
			}
			if(DBUSBuffer[13]==1)	
			{
				sendgunFlag = 2;//鼠标右键是否按下
				Dial_Wheel_target_speed_value = -dial_speed;
			}
		}
	/*---------------------------------------------------------------------------------------------*/
			
	/*---------------------------------------------------------------------------------------------*/	
//测试用
		if(DBUS_ReceiveData.switch_left==2&&LASTDBUS_ReceiveData.switch_left==3)//遥控左侧控制开关S1下播
		{
			mocaflag=1;	
			if(sendgunFlag==0)	sendgunFlag = 1;	//图像调试用
		}
//		if(mocaflag==1)//当摩擦轮打开时才去判断是否回中
//		{
//			if(DBUS_ReceiveData.switch_left==3)//遥控S2回中
//			{
//				mocaflag=2;//因为程序的原因，此处不能为0
//			}
//		}
}
/*------------------------------------------------------------------------
函数名  ：sendgun（）
函数变量 :
功能    ：拨轮控制函数
日期    ：2017.4.18
作者    ：杨博炜（改）
-----------------------------------------------------------------------------------------------------------------------*/
void sendgun(void)
{
	Dial_Wheel_position_set();
	dial_control = Driver_PID(&Dial_wheel_position_pid);
}
/*-------------------------------------------------------------------------------------------------------------------
函数名  ：Dial_wheel_on_off（）
函数变量 :
功能    ：摩擦轮开闭控制，拨轮转停控制
日期    ：2017.4.18
作者    ：杨博炜（改）
-----------------------------------------------------------------------------------------------------------------------*/
void Dial_wheel_on_off()
{
	static int dial_div=0;
	/*--------------------------拨轮的开闭---------------------------------*/
		if(mocaflag)
		{
			if(sendgunFlag==1)
			{
				Dial_Wheel_target_speed_value = dial_speed;
				if(Gun_Real_Speed_Value < 100&&Gun_Real_Speed_Value > 0)
				{
					dial_div++;
					if(dial_div >= 500)
					{
						dial_div = 0;
						sendgunFlag = 2;
					}
				}
			}
			else if(sendgunFlag==2)
			{
				Dial_Wheel_target_speed_value = -dial_speed;
				if(Gun_Real_Speed_Value > -100&&Gun_Real_Speed_Value<0)
				{
					dial_div++;
					if(dial_div >= 500)
					{
						dial_div = 0;
						sendgunFlag = 1;
					}
				}
			}
			else if(sendgunFlag == 0)
			{
				Dial_Wheel_target_speed_value = 0;
			}
			
//			if((sendgunForwardFlag)||(sendgunBackwardFlag))
//			{
//				dial_div+=1;
//				if(sendgunForwardFlag)
//				{
//					if(dial_div==250)
//					{
//						if(Gun_Real_Speed_Value<(Dial_Wheel_target_speed_value/10))
//						{
//							sendgunForwardFlag=0;
//							sendgunBackwardFlag=1;
//						}
//					}
//					if(dial_div==500)
//					{
//						Dial_Wheel_target_speed_value=0;
//						sendgunForwardFlag-=1;
//						dial_div=0;
//					}
//				}
//				else
//				{
//					if(dial_div==250)
//					{
//						if(Gun_Real_Speed_Value>(Dial_Wheel_target_speed_value/10))
//						{
//							sendgunForwardFlag=1;
//							sendgunBackwardFlag=0;
//						}
//					
//					}
//					if(dial_div==500)
//					{
//						Dial_Wheel_target_speed_value=0;
//						sendgunBackwardFlag-=1;
//						dial_div=0;
//					}
//				}
//			}
		}	
	
		/*------------------------摩擦轮的开闭--------------------------------*/
		if(mocaflag==1)//打开摩擦轮
		{
			TIM_SetCompare2(TIM4,1100);//单发1093
			TIM_SetCompare3(TIM4,1100);
		}
		if(mocaflag==2)//关闭摩擦轮
		{
			TIM_SetCompare2(TIM4,1000);
			TIM_SetCompare3(TIM4,1000);
		}
}
