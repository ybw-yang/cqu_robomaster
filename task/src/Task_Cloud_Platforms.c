
#include "Task_Cloud_Platforms.h"


/********************************************************************************
�������͵���壨24V�ģ�
��̨�Ľ��������ǣ� 0x1ff 
Data[0] Data[1] ��� 0x205 
Data[2] Data[3] ��� 0x206 
20160121
*********************************************************************************/

void New_Cmd_ESC(int16_t pitch_205,int16_t yaw_206)
{
	  static CanSend_Type SendData;
    
	 	SendData.CANx=2;
    SendData.SendCanTxMsg.StdId = 0x1FF;
    SendData.SendCanTxMsg.IDE = CAN_Id_Standard;
    SendData.SendCanTxMsg.RTR = CAN_RTR_Data;
    SendData.SendCanTxMsg.DLC = 0x08;
    
    SendData.SendCanTxMsg.Data[0] = (unsigned char)(pitch_205 >> 8);
    SendData.SendCanTxMsg.Data[1] = (unsigned char)pitch_205;
    SendData.SendCanTxMsg.Data[2] = (unsigned char)(yaw_206 >> 8);
    SendData.SendCanTxMsg.Data[3] = (unsigned char)yaw_206;
    SendData.SendCanTxMsg.Data[4] = 0x00;
    SendData.SendCanTxMsg.Data[5] = 0x00;
    SendData.SendCanTxMsg.Data[6] = 0x00;
    SendData.SendCanTxMsg.Data[7] = 0x00;
    

	xQueueSend(Queue_CANSend, &SendData, 10);
}

void Dial_wheel_ctrl(int16_t output5)
{
	  static CanSend_Type SendData;
   
		SendData.CANx=1;
    SendData.SendCanTxMsg.StdId = 0x1FF;
    SendData.SendCanTxMsg.IDE = CAN_Id_Standard;
    SendData.SendCanTxMsg.RTR = CAN_RTR_Data;
    SendData.SendCanTxMsg.DLC = 0x08;
    
    SendData.SendCanTxMsg.Data[0] = ((output5>>8)&0xff);
    SendData.SendCanTxMsg.Data[1] = output5&0xff;
    SendData.SendCanTxMsg.Data[2] =0;
    SendData.SendCanTxMsg.Data[3] =0;
    SendData.SendCanTxMsg.Data[4] =0;
    SendData.SendCanTxMsg.Data[5] =0;
    SendData.SendCanTxMsg.Data[6] =0;
    SendData.SendCanTxMsg.Data[7] =0;
    
		xQueueSend(Queue_CANSend, &SendData, 10);
}
/*-------------------------------------------------------------------------------------------------------------------
������  ��yuntai_Ctrl����
�������� :
����    ����̨���ƺ���
����    ��2016.12.30
����    ���쿣��ģ�
-----------------------------------------------------------------------------------------------------------------------*/
void Task_Yuntai_Ctrl(void)
{
	portTickType CurrentUpperPartTick = 0;                //��ǰϵͳʱ��

	for(;;)
	{
		CurrentUpperPartTick = xTaskGetTickCount();

		static float mpu_9250_ctrl_init = 0;
		static float MPU9250_Yaw_turn_keyboard = 0;
		static float MPU9250_Yaw_turn_qe = 0;
		static u8 turn_flag_keyboard_trg[2]={0};
		
		static u8 Yuntai_scan[2] = {0};
		Yuntai_scan[1] = Yuntai_scan[0];
		Yuntai_scan[0] = (DBUSBuffer[5] >> 4)& 0x0003;
		
		if(Yuntai_scan[0] ==3 && Yuntai_scan[1]==2)
		{
			separate_flag=!separate_flag;
		}
		
		turn_flag_keyboard_trg[0] = turn_flag_keyboard_trg[1];
		turn_flag_keyboard_trg[1] = turn_flag_keyboard;
		
		if(Graph_flag==1)  //���ֵ���
		{
			control_yaw_limited = mouse_x * yaw_LSB_mouse;//���ֵ�����̨�������

		}

		else if(Graph_flag==0)  //δ���ֵ���
		{
			if(turn_flag_keyboard==0)	//δ����qeת��
			{
				mpu_9250_yaw_first = MPU9250_Yaw_turn;
				if(turn_flag_keyboard_trg[1]==0 && turn_flag_keyboard_trg[0]==1)  					 //�½��ش���
				{
					MPU9250_Yaw_turn_keyboard = MPU9250_Yaw_turn_qe + MPU9250_Yaw_turn_keyboard ;
				}
//				if(separate_flag&&yuntai_back_to_init) //��̨��ʼɨ��
//				{
//					//��̨��ʼɨ��
//					if(mouse_x_add_direction == 0) mouse_x += 3;
//					if(mouse_x_add_direction == 1) mouse_x -= 3;
//					if(mouse_x >=6000)
//					{
//						mouse_x=6000;
//						mouse_x_add_direction = 1;
//					}
//					if(mouse_x <=-6000)	
//					{
//						mouse_x=-6000;
//						mouse_x_add_direction = 0;
//					}
//				}
				//��̨����
				control_yaw_limited = mouse_x * yaw_LSB_mouse;
			}
			else if(turn_flag_keyboard) //����qeת��
			{
				control_yaw_limited = 0;
				MPU9250_Yaw_turn_qe = MPU9250_Yaw_turn - mpu_9250_yaw_first;
			}
			if(mpu_9250_ctrl_init==0) mpu_9250_ctrl_init = 1;
			
			if(yuntai_back_to_init == 0)	//ʶ�����̨���̻ص���ʼλ��
				control_yaw_limited = 0;
		}
		
		if(mouse_y > PITCH_ANGLE_DIFFERENCE/4.0f/pitch_LSB_mouse)			mouse_y= PITCH_ANGLE_DIFFERENCE/4.0f/pitch_LSB_mouse;
		if(mouse_y < -PITCH_ANGLE_DIFFERENCE/4.0f/pitch_LSB_mouse)			mouse_y= -PITCH_ANGLE_DIFFERENCE/4.0f/pitch_LSB_mouse;
		
		control_pitch_limited = mouse_y * pitch_LSB_mouse;
		
		New_Cmd_ESC(current_output_pitch_speed,-current_output_yaw_speed); //��̨���Ƴ���
		
		Dial_wheel_ctrl(dial_control); //����can���ͺ���
		
		Dial_wheel_on_off();//Ħ���ֿ��տ��ƣ����ֵ�λ�ÿ���
		
		//IWDG_judge();//���Ź������ж�
		
	  vTaskDelayUntil(&CurrentUpperPartTick, 1 / portTICK_RATE_MS);
	}

}

