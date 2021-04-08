#include "Task_Weapons.h"

int sendgunFlag = 0;
u32	position_gun=0;
int Dial_Wheel_target_speed_value = 0;
u8  mocaflag=0;
float dial_control=0;
int  prenum=0,currentnum=0,prenum_yaokong=3,currentnum_yaokong=3;
u8	RC_Ctl_s1_flt[2]={0};

/*-------------------------------------------------------------------------------------------------------------------
������  ��weapon_ctrl����
�������� :
����    ���������ƺ���
����    ��2017.4.18
����    ���쿣��ģ�
-----------------------------------------------------------------------------------------------------------------------*/
void weapon_ctrl(void)
{
	/*---------------------------------------- ����-----------------------------------------------*/
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
				sendgunFlag = 2;//����Ҽ��Ƿ���
				Dial_Wheel_target_speed_value = -dial_speed;
			}
		}
	/*---------------------------------------------------------------------------------------------*/
			
	/*---------------------------------------------------------------------------------------------*/	
//������
		if(DBUS_ReceiveData.switch_left==2&&LASTDBUS_ReceiveData.switch_left==3)//ң�������ƿ���S1�²�
		{
			mocaflag=1;	
			if(sendgunFlag==0)	sendgunFlag = 1;	//ͼ�������
		}
//		if(mocaflag==1)//��Ħ���ִ�ʱ��ȥ�ж��Ƿ����
//		{
//			if(DBUS_ReceiveData.switch_left==3)//ң��S2����
//			{
//				mocaflag=2;//��Ϊ�����ԭ�򣬴˴�����Ϊ0
//			}
//		}
}
/*------------------------------------------------------------------------
������  ��sendgun����
�������� :
����    �����ֿ��ƺ���
����    ��2017.4.18
����    ���쿣��ģ�
-----------------------------------------------------------------------------------------------------------------------*/
void sendgun(void)
{
	Dial_Wheel_position_set();
	dial_control = Driver_PID(&Dial_wheel_position_pid);
}
/*-------------------------------------------------------------------------------------------------------------------
������  ��Dial_wheel_on_off����
�������� :
����    ��Ħ���ֿ��տ��ƣ�����תͣ����
����    ��2017.4.18
����    ���쿣��ģ�
-----------------------------------------------------------------------------------------------------------------------*/
void Dial_wheel_on_off()
{
	static int dial_div=0;
	/*--------------------------���ֵĿ���---------------------------------*/
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
	
		/*------------------------Ħ���ֵĿ���--------------------------------*/
		if(mocaflag==1)//��Ħ����
		{
			TIM_SetCompare2(TIM4,1100);//����1093
			TIM_SetCompare3(TIM4,1100);
		}
		if(mocaflag==2)//�ر�Ħ����
		{
			TIM_SetCompare2(TIM4,1000);
			TIM_SetCompare3(TIM4,1000);
		}
}
