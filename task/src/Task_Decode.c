#include "Task_Decode.h"

/*-------------------------------------------------------------------------------------------------------------------
������  ��data_decoder����
�������� :
����    ��ͨѶ���
����    ��2017.03.14
����    ����
-----------------------------------------------------------------------------------------------------------------------*/
int radio_ahead_back_data=0;//�ܿ���
int radio_left_right_data=0;
int radio_turn_data=0;
float radio_turn_data_follow = 0;
float radio_turn_data_baiwei = 0;

int radio_ahead_back_data_circle = 0;//��Բ
int radio_left_right_data_circle = 0;

int radio_positive_spin_data = 0;
int radio_negative_spin_data = 0; //��������

float radio_ahead_back_data_avoidance = 0;//����������
float radio_left_right_data_avoidance = 0;

int radio_ahead_back_data_keyboard=0;//���̿���
int radio_left_right_data_keyboard=0;
int radio_turn_data_keyboard=0;
int radio_turn_data_mouse=0;

int radio_ahead_back_data_remote=0;//ң�ؿ���
int radio_left_right_data_remote=0;
int radio_turn_data_remote=0;

float radio_turn_data_straight = 0;
//������
//yaw
float radio_3=0,radio_3_remote=0,radio_3_keyboard=0;
//pitch
float radio_2=0,radio_2_remote=0,radio_2_keyboard=0;
//

int s1_position=2;
int normal=1;
int state = 0;													//����״̬����־������Ϊ0����������̨Ϊ1
int st1=0,st2=0;												//״̬�л���Ҫ�õ���״̬��־λ

int ON_OFF=1,ON_OFF_CURRENT=3,ON_OFF_BEFORE=3;

u32 sbus_num0=0;
int  s1,s2=0;

uint8_t mouse_left;
uint8_t mouse_right;
uint16_t key;
uint16_t  sbus_channel_temp[6] = {0};  							//  temp sbus decode channel data

decode_keyboard decode_the_key;
u8 decode_buf[10]={0};
u8 buff_flag_reg[2]={0};
int decode_key=0;
u8 decode_flag=0;

u16	keyTimeOfUnderpan=0;			//���㰴����������ʱ��

u8 mouse_state_keyboard,mouse_state_remote=0,mouse_state=0;
float wheel[4]={0.0};
u8 wheel_serial=0;

u8 switch_right_reg[2] = {0};
	
void data_decoder(void)
{
	LASTDBUS_ReceiveData = DBUS_ReceiveData;

	decode_flag=0;
	ON_OFF_CURRENT = ((DBUSBuffer[5] >> 4)& 0x0003);//ң���Ҳ࿪��S2
	if(ON_OFF_CURRENT==3 && ON_OFF_BEFORE==1)
	{
		ON_OFF=!ON_OFF;
	}
	ON_OFF_BEFORE=ON_OFF_CURRENT;
	if(ON_OFF==1)//����ң�ؽ���
	{
	sbus_num0=sbus_num0+1;//���ڿ��Ź��ж�
		
	DBUS_ReceiveData.ch1 = (DBUSBuffer[0] | DBUSBuffer[1]<<8) & 0x07FF;
	DBUS_ReceiveData.ch2 = (DBUSBuffer[1]>>3 | DBUSBuffer[2]<<5 ) & 0x07FF;
	DBUS_ReceiveData.ch3 = (DBUSBuffer[2]>>6 | DBUSBuffer[3]<<2 | DBUSBuffer[4]<<10) & 0x07FF;
	DBUS_ReceiveData.ch4 = (DBUSBuffer[4]>>1 | DBUSBuffer[5]<<7) & 0x07FF;
	
	DBUS_ReceiveData.switch_left = ( (DBUSBuffer[5] >> 4)& 0x000C ) >> 2;
	DBUS_ReceiveData.switch_right =  (DBUSBuffer[5] >> 4)& 0x0003 ;
	
	DBUS_ReceiveData.mouse.x = DBUSBuffer[6] | (DBUSBuffer[7] << 8);	//x axis
	DBUS_ReceiveData.mouse.y = DBUSBuffer[8] | (DBUSBuffer[9] << 8);
	DBUS_ReceiveData.mouse.z = DBUSBuffer[10]| (DBUSBuffer[11] << 8);
	
	DBUS_ReceiveData.mouse.press_left 	= DBUSBuffer[12];	// is pressed?
	DBUS_ReceiveData.mouse.press_right 	= DBUSBuffer[13];
	
	DBUS_ReceiveData.keyBoard.key_code 	= DBUSBuffer[14] | DBUSBuffer[15] << 8; //key borad code
	// w:1   a:4  s:2  d:8  q:40  e:80   shift:10   ctrl:20 
	if((DBUS_ReceiveData.switch_left != 1)&&(DBUS_ReceiveData.switch_left != 2)&&(DBUS_ReceiveData.switch_left != 3))
		{
        vTaskDelay(1);
				//reset CPU
				__set_FAULTMASK(1);
				NVIC_SystemReset();
		}
	if((DBUS_ReceiveData.switch_right!= 1)&&(DBUS_ReceiveData.switch_right!= 2)&&(DBUS_ReceiveData.switch_right!= 3))
		{
        vTaskDelay(1);
				//reset CPU
				__set_FAULTMASK(1);
				NVIC_SystemReset();
		}
	}
	
		compute_key_time_of_underpan();
	/*----------------------------������̰�-------------------------------*/
	decode_the_key.key_w = DBUS_ReceiveData.keyBoard.key_code & 0x01;
	decode_the_key.key_s = (DBUS_ReceiveData.keyBoard.key_code & 0x02)>>1;
	decode_the_key.key_a = (DBUS_ReceiveData.keyBoard.key_code & 0x04)>>2;
	decode_the_key.key_d = (DBUS_ReceiveData.keyBoard.key_code & 0x08)>>3;
	decode_the_key.key_shift = (DBUS_ReceiveData.keyBoard.key_code & 0x10)>>4;
	decode_the_key.key_ctrl = (DBUS_ReceiveData.keyBoard.key_code & 0x20)>>5;
	decode_the_key.key_q = (DBUS_ReceiveData.keyBoard.key_code & 0x40)>>6;
	decode_the_key.key_e = (DBUS_ReceiveData.keyBoard.key_code & 0x80)>>7;
	/*---------------------------�������---------------------------------*/	

	switch_right_reg[1] = switch_right_reg[0];
	switch_right_reg[0] = DBUS_ReceiveData.switch_right;
	if(switch_right_reg[1]==2&&switch_right_reg[0]==3)gpyo_register = MPU6500_Yaw_turn_baiwei;
}

void Upper_part(void)
{
	  portTickType CurrentUpperPartTick = 0;                //��ǰϵͳʱ��

	  	for(;;)
    {
        CurrentUpperPartTick = xTaskGetTickCount();
        
				weapon_ctrl();

				if(decode_flag)
				{
//					IWDG_Feed();
					
					data_decoder();
								
					Yaokong_Ctrl();

					Keyboard_Ctrl();
				}
				
				radio_3 = radio_3_remote + radio_3_keyboard;
				radio_2 = radio_2_remote + radio_2_keyboard;
				
				mouse_state = mouse_state_keyboard | mouse_state_remote;
				
				radio_ahead_back_data = radio_ahead_back_data_keyboard + 
																radio_ahead_back_data_remote + 
																radio_ahead_back_data_circle + 
																radio_ahead_back_data_avoidance;
				
				radio_left_right_data = radio_left_right_data_keyboard + 
																radio_left_right_data_remote + 
																radio_left_right_data_circle + 
																radio_left_right_data_avoidance;
				
				radio_turn_data = radio_turn_data_keyboard + 
													radio_turn_data_follow + 
													radio_turn_data_remote + 
													radio_turn_data_baiwei + 
													radio_positive_spin_data;
													
				if(separate_flag)
				{			
					radio_ahead_back_data = (radio_ahead_back_data	>=	RADIO_DATA_MAX?		RADIO_DATA_MAX	:	radio_ahead_back_data);
					radio_ahead_back_data = (radio_ahead_back_data	<=	-RADIO_DATA_MAX?	-RADIO_DATA_MAX	:	radio_ahead_back_data);
					radio_left_right_data = (radio_left_right_data	>=	RADIO_DATA_MAX?		RADIO_DATA_MAX	:	radio_left_right_data);		
					radio_left_right_data = (radio_left_right_data	<=	-RADIO_DATA_MAX?	-RADIO_DATA_MAX	:	radio_left_right_data);
					//
					radio_ahead_back_data = ((radio_ahead_back_data<=RADIO_DATA_MIN)&&(radio_ahead_back_data>0)?		RADIO_DATA_MIN	:	radio_ahead_back_data);
					radio_ahead_back_data = ((radio_ahead_back_data>=-RADIO_DATA_MIN)&&(radio_ahead_back_data<0)?		-RADIO_DATA_MIN	:	radio_ahead_back_data);
					radio_left_right_data = ((radio_left_right_data<=RADIO_DATA_MIN)&&(radio_left_right_data>0)?		RADIO_DATA_MIN	:	radio_left_right_data);
					radio_left_right_data = ((radio_left_right_data>=-RADIO_DATA_MIN)&&(radio_left_right_data<0)?		-RADIO_DATA_MIN	:	radio_left_right_data);
					
				}					

		    vTaskDelayUntil(&CurrentUpperPartTick, 7 / portTICK_RATE_MS);
    }

}

//���̰���ʱ����㺯��
//the last edit date 0502
//warpgate
u8 keyLoosenFlag=0;
u8 keyPushFlag=0;

void compute_key_time_of_underpan(void)
{
		if (DBUS_ReceiveData.keyBoard.key_code	>0)
	{
		keyTimeOfUnderpan++;
		keyPushFlag=1;
		if(keyTimeOfUnderpan>=T1)
		{
			keyTimeOfUnderpan=T1;
		}
	}
	else	if(keyPushFlag==1)
		{
			keyTimeOfUnderpan--;
			if(keyTimeOfUnderpan==1)
			{
				keyTimeOfUnderpan=1;
				keyPushFlag=0;
			}
		}
		if(DBUS_ReceiveData.ch1!=1024	||	DBUS_ReceiveData.ch2!=1024)		//�ж��Ƿ���ң��������
		{
			keyTimeOfUnderpan=T1;
		}
}

/**
  * @brief  ������������7ms��
  * @param  ubused
  * @retval void
  */
void Yaokong_Ctrl(void)
{
	if(DBUS_ReceiveData.switch_left== 1)
		{
		 s1_position=4;
		}
	if(DBUS_ReceiveData.switch_left== 3)
		{
		 s1_position=2;
		}
	if(DBUS_ReceiveData.switch_left== 2)
		{
		 s1_position=1;
		}
	s2=DBUS_ReceiveData.switch_right;    
	//ң��
	//if ok, then process the receive data.
	radio_ahead_back_data_remote = DBUS_ReceiveData.ch2 - 1024;//ǰ���ƶ�����Ӧ����ws
	if(radio_ahead_back_data_remote)
		go_straight_aheadback_flag_remote=1;
	else go_straight_aheadback_flag_remote=0;
	
	radio_left_right_data_remote = (DBUS_ReceiveData.ch1 - 1024);//���Һ��ƣ���Ӧ����ad
	if(radio_left_right_data_remote)
		go_straight_leftright_flag_remote=1;
	else go_straight_leftright_flag_remote=0;
	
	radio_turn_data_remote = -(DBUS_ReceiveData.ch3 - 1024);
	if(radio_turn_data_remote)
		turn_flag_remote=1;
	else turn_flag_remote=0;

	
//	radio_3_remote =-(DBUS_ReceiveData.ch3 - 1024)/remote_sensitivity_x;//��̨����ת����Ӧ��������ƶ�
	if(separate_flag==0)
	{
		if(radio_3<-7)
		{
			mouse_state_remote = 2;
		}
		else if(radio_3>7)
		{
			mouse_state_remote = 1;
		}
		else mouse_state_remote=0;
	}
	else
	{
		mouse_state_remote = 0;
	}
	
	radio_2_remote =(DBUS_ReceiveData.ch4 - 1024)/remote_sensitivity_y;//��̨����ת����Ӧ��������ƶ�
	
//	if(radio_turn_data_remote)
//		turn_flag_remote=1;
//	else turn_flag_remote=0;
}

void Keyboard_Ctrl(void)
{
				/*---��̨���������������Ƿ��ģ�---*/
	radio_3_keyboard =-DBUS_ReceiveData.mouse.x * mouse_sensitivity_x;
	radio_2_keyboard =+DBUS_ReceiveData.mouse.y * mouse_sensitivity_y;
	/*-------------------------------*/
	/*--------------�����Ƶ��̳���δ����ctrl״̬��--------------*/
	if(separate_flag==0)
	{
		if(radio_3_keyboard<0)
		{
			mouse_state_keyboard = 2;
		}
		else if(radio_3_keyboard>0)
		{
			mouse_state_keyboard = 1;
		}
		else mouse_state_keyboard=0;
	}
	else
	{
		mouse_state_keyboard = 0;
	}
/*-------------------------���̿���-------------------------*/	
	// ǰ���˶���ͬʱ��ws������Ϊ����
	if((decode_the_key.key_w && decode_the_key.key_s)!=1)  //δͬʱ����ws
	{
		if(decode_the_key.key_w) 
		{
			if(!decode_the_key.key_shift)
			radio_ahead_back_data_keyboard = normal_Speed;
			else 
			radio_ahead_back_data_keyboard = SpeedUp;
			go_straight_aheadback_flag_keyboard=1;
		}
		else if(decode_the_key.key_s) 
		{
			if(!decode_the_key.key_shift)
			radio_ahead_back_data_keyboard = -normal_Speed;
			else 
			radio_ahead_back_data_keyboard = -SpeedUp;
			go_straight_aheadback_flag_keyboard=1;
		}
		else 
		{
			go_straight_aheadback_flag_keyboard=0;
			radio_ahead_back_data_keyboard = 0;
		}
	}
	else radio_ahead_back_data_keyboard = 0;  //ͬʱ����ws
	
	// �����˶�,ͬʱ��ad������Ϊ����
	if((decode_the_key.key_a && decode_the_key.key_d)!=1) //δͬʱ����ad
	{
		if(decode_the_key.key_a) 
		{
			if(!decode_the_key.key_shift)
			radio_left_right_data_keyboard = -normal_Speed;
			else 
			radio_left_right_data_keyboard = -SpeedUp;
			go_straight_leftright_flag_keyboard=1;
		}
		else if(decode_the_key.key_d) 
		{
			if(!decode_the_key.key_shift)
			radio_left_right_data_keyboard = normal_Speed;
			else 
			radio_left_right_data_keyboard = SpeedUp;
			go_straight_leftright_flag_keyboard=1;
		}
		else		
		{
			//if(pid_turn_flag == 0)		//����ֱ���������У���������
			go_straight_leftright_flag_keyboard=0;
				radio_left_right_data_keyboard = 0;
		}
	}
	else radio_left_right_data_keyboard = 0; //ͬʱ����ad
	
	//������ת��ͬʱ��qe������Ϊ����
	if((decode_the_key.key_q && decode_the_key.key_e)!=1)  //δͬʱ����qe
	{
		if(decode_the_key.key_q) 
		{
			if(!decode_the_key.key_shift)
			radio_turn_data_keyboard = 0.5*normal_Speed;
			else 
			radio_turn_data_keyboard = 0.5*SpeedUp;
			turn_flag_keyboard=1;
		}
		else if(decode_the_key.key_e) 
		{
			if(!decode_the_key.key_shift)
			radio_turn_data_keyboard = -0.5*normal_Speed;
			else 
			radio_turn_data_keyboard = -0.5*SpeedUp;
			turn_flag_keyboard=1;
		}
		else 
		{
			if(pid_turn_flag == 0 && mouse_state_keyboard ==0)
				radio_turn_data_keyboard = 0;
			turn_flag_keyboard=0;
		}
	}
	else radio_turn_data_keyboard = 0;  //ͬʱ����qe
/*----------------------------------------------------------------*/
}

void dipan_Ctrl(void) 
{
	go_straight();
	turn_round();
	wheel[0] = (+radio_ahead_back_data + radio_left_right_data - radio_turn_data)*10;
	wheel[1] = (-radio_ahead_back_data + radio_left_right_data - radio_turn_data)*10;
	wheel[2] = (-radio_ahead_back_data - radio_left_right_data - radio_turn_data)*10;
	wheel[3] = (+radio_ahead_back_data - radio_left_right_data - radio_turn_data)*10;
		/*------------------------------------����PID������----------------------------------------------------*/
  ChassisParam.LF.TargetSpeed = wheel[0];
  ChassisParam.RF.TargetSpeed = wheel[1];
  ChassisParam.RB.TargetSpeed = wheel[2];
  ChassisParam.LB.TargetSpeed = wheel[3];
	Chassis_PID_Control();
	
	power_limit();
	
	Chassis_SendMotorParam();
}
