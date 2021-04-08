#include "Task_Fun.h"

const int GO_STRAIGHT_MAX = 3000;
int add_current,add_pre;
int go_straight_flag_trg[2]={0};
int radio_turn_data_trg[2]={0};

float pitch_position_difference[2]={0};//PID��������
float yaw_position_difference[2]={0};
float pitch_speed_difference[2]={0};
float yaw_speed_difference[2]={0};
float dial_wheel_difference[2]={0};
float go_straight_difference[2]={0};
float yuntai_position_difference[2]={0};
float yuntai_pitch_position_difference[2]={0};
float underpan_follow_difference[2]={0};

float gpyo_register=0;
float  ABS=0;

double Circle_n=0;			//���س���ת��Բ�����������
const float r = 0.8;//�뾶
const float T = 5.0;//����
/*******��λ�ñ���*****************/
float control_yaw_limited=0;
float control_pitch_limited=0;
/*******��λ�ñ���*****************/
/*-------��������Ĵ������ݳ�ʼ��--------*/ 
float mpu_6500_pitch,mpu_6500_yaw,mpu_6500_roll;
float MPU6500_Yaw_turn=0;
float MPU6500_Pitch_turn=0;
float MPU9250_Yaw_turn=0;
float MPU9250_Pitch_turn=0;

float MPU6500_Pitch[2] ={0};   //һ���˲�����
float MPU6500_Yaw[2] = {0};
float MPU9250_Pitch[2] ={0}; 
float MPU9250_Yaw[2] = {0};
float mouse_x =0;   									//YAWң���ٶȻ���ֵ
float mouse_y =0;  									    //PITCHң���ٶȻ���ֵ

float MPU6500_Yaw_turn_baiwei=0;
float flt_MPU6500_Yaw_turn_original[2]={0};

  //��λ��ֵ�˲�����
float yaw_filter_positon_mean[6]={0};
float pitch_filter_positon_mean[6]={0};
float go_straight_filter_positon_mean[6]={0};
float track_yaw_mean[6] = {0}; 
float track_pitch_mean[6] = {0}; 
float track_mousex_mean[6] = {0}; 
float track_mousey_mean[6] = {0}; 

float current_output_yaw;		//yaw������� ����pid����֮��
float current_output_pitch;		//pitch ������� ����pid����֮��
float track_yaw_output;
float track_pitch_output;

float contorl_pitch_speed;
float contorl_yaw_speed;
float current_output_yaw_speed;
float current_output_pitch_speed;

u8 go_straight_leftright_flag = 0;		//������ֱ��־
u8 go_straight_aheadback_flag = 0;		//ǰ����ֱ��־
u8 turn_flag = 0;

u8 go_straight_leftright_flag_keyboard = 0;//������������
u8 go_straight_aheadback_flag_keyboard = 0;//����ǰ������
u8 turn_flag_keyboard = 0;

u8 go_straight_aheadback_flag_remote = 0;//ң��ǰ����ֱ
u8 go_straight_leftright_flag_remote = 0;//ң��������ֱ
u8 turn_flag_remote = 0;

u8 pid_turn_flag = 0;
u8 under_follow_end_flag = 0;  //�������
u8 separate_flag = 1;//��̨���̷�����Ʊ�־λ
u8 attack_direction = 0;
u8 yuntai_back_to_init = 1; //��̨�ص���ʼ�Ƕ�
u8 baiwei_flag=0;

void turn_round(void) //���̸�������̻�Բ����
{
	static u8 Graph_flag_rise_eage[2] = {0};
	Graph_flag_rise_eage[1] = Graph_flag_rise_eage[0];
	Graph_flag_rise_eage[0] = Graph_flag;
	if(!separate_flag)
  {
		radio_positive_spin_data = 0;
		radio_turn_data_baiwei = 0;
		radio_turn_data_straight = 0;
		
		radio_ahead_back_data_circle = 0;
		radio_left_right_data_circle = 0;
  }
	else
	{
		if(Graph_flag==1) //���ֵ���
		{
			radio_positive_spin_data = 0;
			
			if(baiwei_flag==0)
			{
				radio_turn_data_baiwei= -110;  
			}
			else if(baiwei_flag==1)
			{
				radio_turn_data_baiwei= 110;   
			}
			
			if(yaw_angle -YAW_CENTER>=25)   //�Ұ�45��ת��
			{
				baiwei_flag=1;
			}
			else if(yaw_angle -YAW_CENTER<=-25)   //���45��ת��
			{
				baiwei_flag=0;
			}
			
		}
		else if(Graph_flag==0&&yuntai_back_to_init==1) //δ���ֵ���
		{
			//���̿�ʼ��Բ
			radio_left_right_data_circle = -700*r/T*sin(2*3.14*Circle_n/T);
			radio_ahead_back_data_circle =  700*r/T*cos(2*3.14*Circle_n/T);
			go_straight_leftright_flag_remote=1;
			go_straight_aheadback_flag_remote=1;
			radio_turn_data_baiwei = 0;
			
			//������������ 
			radio_positive_spin_data = 105;
			
		}
	}
	
}
/*-------------------------------------------------------------------------------------------------------------------
������  ��go_straight����
�������� :
����    ����ֱ����
����    ��2016.12.30
����    ���쿣��ģ�
-----------------------------------------------------------------------------------------------------------------------*/
void go_straight(void)  //��ֱ�����ߣ���,����Ȼ
{
	go_straight_leftright_flag = go_straight_leftright_flag_keyboard | go_straight_leftright_flag_remote;
	go_straight_aheadback_flag = go_straight_aheadback_flag_keyboard | go_straight_aheadback_flag_remote;
	turn_flag = turn_flag_keyboard | turn_flag_remote | mouse_state;
	
  go_straight_flag_trg[0] = go_straight_flag_trg[1];
	go_straight_flag_trg[1] = go_straight_aheadback_flag+go_straight_leftright_flag;
	radio_turn_data_trg[0] = radio_turn_data_trg[1];
	radio_turn_data_trg[1] = turn_flag;
	if( 0<(go_straight_aheadback_flag+go_straight_leftright_flag) 			//��ǰ�������ƽ��ʱ
			&& (go_straight_aheadback_flag+go_straight_leftright_flag)<=2 
			&& turn_flag!=1																									//��δ����ת��ʱ
			&& mouse_state ==0																							//��δ���ת��ʱ
	)
	{
		if(Graph_flag == 0&&yuntai_back_to_init == 1&&separate_flag==1)
		{
			go_straight_pid_set();
			radio_turn_data_straight = -Driver_PID(&go_straight_pid);
		}
		else pid_turn_flag = 0;
	}
}


/*-------------------------------------------------------------------------------------------------------------------
������  ��gpyo_read����
�������� :
����    �����������ݴ�����
����    ��2017.1.3
����    ���쿣��ģ�
-----------------------------------------------------------------------------------------------------------------------*/
void Task_gpyo_read(void)
{
	portTickType CurrentGPYOreadTick = 0;                //��ǰϵͳʱ��

	for(;;)
	{
		CurrentGPYOreadTick = xTaskGetTickCount();

		/*---------�õ�mpu6500����������Ƕȣ�����Ķ��ϴ�ʱ�����±궨-----*/	
			Prepare_Data();
			IMU_Update();
		/*-----------------�������������㣨�����־λ�жϿɵ���ģʽ��-------------------*/
		
		if(isNaN(Angle.X)==0)
		{
			MPU6500_Pitch_turn = first_order_filter_gpyo(MPU6500_Pitch, Angle.X, MPU6500_Pitch_turn,50);		
			if(MPU6500_Pitch_turn< mouse_y * pitch_LSB_mouse/2 - 25) MPU6500_Pitch_turn = mouse_y * pitch_LSB_mouse/2 - 25;
			if(MPU6500_Pitch_turn> mouse_y * pitch_LSB_mouse/2 + 25) MPU6500_Pitch_turn = mouse_y * pitch_LSB_mouse/2 + 25;
		}
		
		if(isNaN(Angle.Z)==0)
		{
			MPU6500_Yaw_turn_baiwei = first_order_filter_gpyo(flt_MPU6500_Yaw_turn_original ,Angle.Z, MPU6500_Yaw_turn_baiwei,50) ;//���ڰ�β
				
			MPU6500_Yaw_turn = first_order_filter_gpyo(MPU6500_Yaw, Angle.Z, MPU6500_Yaw_turn,50);//������̨����
		}
		#if bluetooth
		push(0,MPU6500_Yaw_turn);
		push(1,MPU6500_Yaw_turn_baiwei);
		push(2,gpyo_register);
		push(3,current_output_yaw);
		push(4,yaw_speed);
		sendDataToScope();
		#endif
		
	  vTaskDelayUntil(&CurrentGPYOreadTick, 7 / portTICK_RATE_MS);	
	}
}
