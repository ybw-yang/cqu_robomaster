#include "Task_Fun.h"

const int GO_STRAIGHT_MAX = 3000;
int add_current,add_pre;
int go_straight_flag_trg[2]={0};
int radio_turn_data_trg[2]={0};

float pitch_position_difference[2]={0};//PID输入数组
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

double Circle_n=0;			//基地车旋转画圆所需计数变量
const float r = 0.8;//半径
const float T = 5.0;//周期
/*******限位用变量*****************/
float control_yaw_limited=0;
float control_pitch_limited=0;
/*******限位用变量*****************/
/*-------陀螺仪软寄存器数据初始化--------*/ 
float mpu_6500_pitch,mpu_6500_yaw,mpu_6500_roll;
float MPU6500_Yaw_turn=0;
float MPU6500_Pitch_turn=0;
float MPU9250_Yaw_turn=0;
float MPU9250_Pitch_turn=0;

float MPU6500_Pitch[2] ={0};   //一阶滤波数组
float MPU6500_Yaw[2] = {0};
float MPU9250_Pitch[2] ={0}; 
float MPU9250_Yaw[2] = {0};
float mouse_x =0;   									//YAW遥控速度积分值
float mouse_y =0;  									    //PITCH遥控速度积分值

float MPU6500_Yaw_turn_baiwei=0;
float flt_MPU6500_Yaw_turn_original[2]={0};

  //移位中值滤波数组
float yaw_filter_positon_mean[6]={0};
float pitch_filter_positon_mean[6]={0};
float go_straight_filter_positon_mean[6]={0};
float track_yaw_mean[6] = {0}; 
float track_pitch_mean[6] = {0}; 
float track_mousex_mean[6] = {0}; 
float track_mousey_mean[6] = {0}; 

float current_output_yaw;		//yaw电流输出 经过pid计算之后
float current_output_pitch;		//pitch 电流输出 经过pid计算之后
float track_yaw_output;
float track_pitch_output;

float contorl_pitch_speed;
float contorl_yaw_speed;
float current_output_yaw_speed;
float current_output_pitch_speed;

u8 go_straight_leftright_flag = 0;		//左右走直标志
u8 go_straight_aheadback_flag = 0;		//前后走直标志
u8 turn_flag = 0;

u8 go_straight_leftright_flag_keyboard = 0;//键盘左右走至
u8 go_straight_aheadback_flag_keyboard = 0;//键盘前后走至
u8 turn_flag_keyboard = 0;

u8 go_straight_aheadback_flag_remote = 0;//遥控前后走直
u8 go_straight_leftright_flag_remote = 0;//遥控左右走直
u8 turn_flag_remote = 0;

u8 pid_turn_flag = 0;
u8 under_follow_end_flag = 0;  //跟随完成
u8 separate_flag = 1;//云台底盘分离控制标志位
u8 attack_direction = 0;
u8 yuntai_back_to_init = 1; //云台回到初始角度
u8 baiwei_flag=0;

void turn_round(void) //底盘跟随与底盘画圆程序
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
		if(Graph_flag==1) //发现敌人
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
			
			if(yaw_angle -YAW_CENTER>=25)   //右摆45，转向
			{
				baiwei_flag=1;
			}
			else if(yaw_angle -YAW_CENTER<=-25)   //左摆45，转向
			{
				baiwei_flag=0;
			}
			
		}
		else if(Graph_flag==0&&yuntai_back_to_init==1) //未发现敌人
		{
			//底盘开始画圆
			radio_left_right_data_circle = -700*r/T*sin(2*3.14*Circle_n/T);
			radio_ahead_back_data_circle =  700*r/T*cos(2*3.14*Circle_n/T);
			go_straight_leftright_flag_remote=1;
			go_straight_aheadback_flag_remote=1;
			radio_turn_data_baiwei = 0;
			
			//底盘自旋程序 
			radio_positive_spin_data = 105;
			
		}
	}
	
}
/*-------------------------------------------------------------------------------------------------------------------
函数名  ：go_straight（）
函数变量 :
功能    ：走直函数
日期    ：2016.12.30
作者    ：杨博炜（改）
-----------------------------------------------------------------------------------------------------------------------*/
void go_straight(void)  //走直，作者：杨博炜,陈依然
{
	go_straight_leftright_flag = go_straight_leftright_flag_keyboard | go_straight_leftright_flag_remote;
	go_straight_aheadback_flag = go_straight_aheadback_flag_keyboard | go_straight_aheadback_flag_remote;
	turn_flag = turn_flag_keyboard | turn_flag_remote | mouse_state;
	
  go_straight_flag_trg[0] = go_straight_flag_trg[1];
	go_straight_flag_trg[1] = go_straight_aheadback_flag+go_straight_leftright_flag;
	radio_turn_data_trg[0] = radio_turn_data_trg[1];
	radio_turn_data_trg[1] = turn_flag;
	if( 0<(go_straight_aheadback_flag+go_straight_leftright_flag) 			//在前后或左右平动时
			&& (go_straight_aheadback_flag+go_straight_leftright_flag)<=2 
			&& turn_flag!=1																									//且未键盘转弯时
			&& mouse_state ==0																							//且未鼠标转弯时
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
函数名  ：gpyo_read（）
函数变量 :
功能    ：陀螺仪数据处理函数
日期    ：2017.1.3
作者    ：杨博炜（改）
-----------------------------------------------------------------------------------------------------------------------*/
void Task_gpyo_read(void)
{
	portTickType CurrentGPYOreadTick = 0;                //当前系统时间

	for(;;)
	{
		CurrentGPYOreadTick = xTaskGetTickCount();

		/*---------得到mpu6500陀螺仪三轴角度，程序改动较大时需重新标定-----*/	
			Prepare_Data();
			IMU_Update();
		/*-----------------陀螺仪数据运算（加入标志位判断可调节模式）-------------------*/
		
		if(isNaN(Angle.X)==0)
		{
			MPU6500_Pitch_turn = first_order_filter_gpyo(MPU6500_Pitch, Angle.X, MPU6500_Pitch_turn,50);		
			if(MPU6500_Pitch_turn< mouse_y * pitch_LSB_mouse/2 - 25) MPU6500_Pitch_turn = mouse_y * pitch_LSB_mouse/2 - 25;
			if(MPU6500_Pitch_turn> mouse_y * pitch_LSB_mouse/2 + 25) MPU6500_Pitch_turn = mouse_y * pitch_LSB_mouse/2 + 25;
		}
		
		if(isNaN(Angle.Z)==0)
		{
			MPU6500_Yaw_turn_baiwei = first_order_filter_gpyo(flt_MPU6500_Yaw_turn_original ,Angle.Z, MPU6500_Yaw_turn_baiwei,50) ;//用于摆尾
				
			MPU6500_Yaw_turn = first_order_filter_gpyo(MPU6500_Yaw, Angle.Z, MPU6500_Yaw_turn,50);//用于云台控制
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
