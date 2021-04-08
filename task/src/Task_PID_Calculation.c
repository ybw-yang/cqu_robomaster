#include "main.h"
#include "Task_PID_Calculation.h"

float mpu_9250_yaw_first=0;
float mpu_9250_pitch_first=0;
float mpu_9250_yaw_zero=0;// �������ϵ�Ƕ�
float mpu_9250_pitch_zero=0;
float yaw_angle_zero =0;
u8 Graph_change_flt[2] = {0};
u8 Graph_flag_flt[4]={0};
u16 track_count = 0;

void Task_PID_Calculation(void)
{
	u16 static div_0,div_1,div_2 = 0,div_3 = 10,track_div = 0;
	float static mouse_x_track,mouse_y_track=0;
	
	portTickType CurrentPIDCalculationTick = 0;                //��ǰϵͳʱ��

	for(;;)
	{
		CurrentPIDCalculationTick = xTaskGetTickCount();
		//����״̬ת��
	  Graph_change_flt[1] = Graph_change_flt[0];
		Graph_change_flt[0] = Graph_flag;
		
		if(Graph_change_flt[0]==0&&Graph_change_flt[1]==1)
		{
			mouse_x = (yaw_angle-YAW_CENTER)*100;
			mouse_y = (pitch_angle-PITCH_CENTER)*pitch_LSB_mouse;
			control_yaw_limited = mouse_x / 100;
			control_pitch_limited = mouse_y / pitch_LSB_mouse;			
		}
		else if(Graph_change_flt[0]==1&&Graph_change_flt[1]==0)
		{
			track_count = 0;
			mouse_x = MPU9250_Yaw_turn * 100;
			mouse_y = MPU9250_Pitch_turn * pitch_LSB_mouse;
			control_yaw_limited = mouse_x / 100;
			control_pitch_limited = mouse_y / pitch_LSB_mouse;			
		}
		if(whole_init==0)				//��̨λ�ó�ʼ����������
		{
			yaw_position_pid_set();
			current_output_yaw = Driver_PID(&yaw_position_pid);
			current_output_yaw = GetMedianNum(yaw_filter_positon_mean, 6,current_output_yaw);//��ֵ�˲�
			
			pitch_position_pid_set();
			current_output_pitch = Driver_PID(&pitch_position_pid);
			current_output_pitch = GetMedianNum(pitch_filter_positon_mean, 6,current_output_pitch);//��ֵ�˲�
			if(abs(pitch_angle-PITCH_CENTER)<1 && abs(yaw_angle-YAW_CENTER)<1)
			{
				//���������ǳ�ʼֵ
				if(mpu_9250_yaw_angle<50||mpu_9250_yaw_angle>-50)
				mpu_9250_yaw_zero= mpu_9250_yaw_angle;
				if(mpu_9250_pitch_angle<50||mpu_9250_pitch_angle>-50)
				mpu_9250_pitch_zero= mpu_9250_pitch_angle;
				yaw_angle_zero = yaw_angle;

				whole_init = 1;					//��ʼ�����
			}
		}
		else										//��̨λ�ó�ʼ�����
		{
			
			if(Graph_flag==0) 					//δ���ֵ���
			{
				yaw_position_pid_set();
				current_output_yaw = Driver_PID(&yaw_position_pid);
				current_output_yaw = GetMedianNum(yaw_filter_positon_mean, 6,current_output_yaw);//��ֵ�˲�

				glide_filter(track_mousex_mean, MPU9250_Yaw_turn/yaw_LSB_mouse);
				glide_filter(track_mousey_mean, MPU9250_Pitch_turn/pitch_LSB_mouse);

				clear_struct(&yaw_track_position_pid);
				clear_struct(&pitch_track_position_pid);
			}

			else if(Graph_flag==1)			//���ֵ���
			{
				track_div ++;
				
				if(track_div>=3)
				{
					track_div = 0;
					if(Re_flag)
					{
						Re_flag = 0;
						buff_yaw = Buff_yaw;
						if(Buff_yaw >=27 || Buff_yaw <= -27)	Buff_yaw = 0;					
						
						mouse_x_track = (MPU9250_Yaw_turn+Buff_yaw/*track_yaw_output*/)/yaw_LSB_mouse;
						mouse_y_track = (MPU9250_Pitch_turn-Buff_pitch/*track_pitch_output*/)/pitch_LSB_mouse;

						mouse_x = glide_filter(track_mousex_mean, mouse_x_track);
						mouse_y = glide_filter(track_mousey_mean, mouse_y_track);
						
					}
				}
				
				yuntai_position_pid_set();
				current_output_yaw = Driver_PID(&yuntai_position_pid);
				
			}
			//pitch
			yuntai_pitch_position_pid_set();
			current_output_pitch = Driver_PID(&yuntai_pitch_position_pid);

			/*----------------------------------------------------------------------------------------------------*/

		}
		
		/*------------------------------------���ٶȻ�����������------------------------------------------------*/
		//yaw
		yaw_speed_pid_set();
		current_output_yaw_speed = Driver_PID(&yaw_speed_pid);
		//pitch
		pitch_speed_pid_set();
		current_output_pitch_speed = Driver_PID(&pitch_speed_pid);
		/*---------------------------------------��ֱPID���Ƽ���-------------------------------------------------*/

		sendgun();//����PID���Ƽ���

		//ң����7ms����һ��
		mouse_x+=(int)(radio_3);
		mouse_y+=(int)(radio_2);
		
		/*--------------------------------�ж��Ƿ��⵽����--------------------------------*///
		div_0++;																																						 //
		div_1++;																																						 //
		div_2++;																																						 //
		if(div_0>=20)//ÿ��140ms��ֵһ��,���ͼ������֡�ʣ�Graph_flag_flt[0]����ֵ�ڴ����ж���1 //
		{																																										 //
			div_0 = 0;																																				 //
			Graph_flag_flt[2] = Graph_flag_flt[1];																						 //
			Graph_flag_flt[1] = Graph_flag_flt[0];																						 //
		}																																										 //
		if(div_1>=200)//ÿ��1.4s����һ��																											 //
		{																																										 //
			div_1 = 0;																																				 //
			Graph_flag_flt[0] = 0;																														 //
		}																																										 //
		if(div_2>=100)//ÿ��0.7s�ж�һ��																											 //
		{																																										 //
			div_2 = 0;																																				 //
			if(Graph_flag_flt[1]==0 && Graph_flag_flt[2]==0)																	 //
			{
				Graph_flag = 0;
				sendgunFlag = 0;
			}																																	 								 //
		}																																										 //
		/*---------------------------------------------------------------------------------*///
		
		//------------------------------------Ħ�����ӳٿ���---------------------------------//
		div_3++;                                                                             //
		if(div_3>=25713)	//34285ʱԼ����4min��8571Լ����1min,25713																							 //
		{																																										 //
			div_3 = 10;																																				 //
			mocaflag = 1;																																			 //
//			separate_flag = 1;
		}																																										 //
		if(mocaflag==1)
			div_3--;
		//-----------------------------------------------------------------------------------//
		
	  vTaskDelayUntil(&CurrentPIDCalculationTick, 7 / portTICK_RATE_MS);
	}
}
