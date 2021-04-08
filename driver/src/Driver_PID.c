#include "main.h"
int follow_angle = 0;

PID pitch_position_pid = {0};
PID pitch_speed_pid = {0};
PID yaw_position_pid = {0};
PID yaw_speed_pid = {0};
PID go_straight_pid = {0};
PID yuntai_position_pid = {0};        //云台yaw 	 9250位置控制
PID yuntai_pitch_position_pid = {0};	 //云台pitch 9250位置控制
PID underpan_follow_pid = {0};				 //底盘跟随
PID Dial_wheel_position_pid = {0};		 //波轮电机PID
PID yaw_track_position_pid = {0};		 //图像yaw跟随PID
PID pitch_track_position_pid = {0};	 //图像pitch跟随PID

Increment_PID inc_yaw_track_position_pid = {0};
/*------------------------------------------------底盘PID参数------------------------------------------------------------*/

PID Chassis_LF_PID;
PID Chassis_RF_PID;
PID Chassis_RB_PID;
PID Chassis_LB_PID;

const float Chassis_LF_Kp     = 2;
const float Chassis_LF_Ki     = 0.2;
const float Chassis_LF_Kd     = 5;
const float Chassis_LF_Imax   = 1000;
const float Chassis_LF_PIDmax = 10000;

const float Chassis_RF_Kp     = 2;
const float Chassis_RF_Ki     = 0.2;
const float Chassis_RF_Kd     = 5;
const float Chassis_RF_Imax   = 1000;
const float Chassis_RF_PIDmax = 10000;

const float Chassis_RB_Kp     = 2;
const float Chassis_RB_Ki     = 0.2;
const float Chassis_RB_Kd     = 5;
const float Chassis_RB_Imax   = 1000;
const float Chassis_RB_PIDmax = 10000;

const float Chassis_LB_Kp     = 2;
const float Chassis_LB_Ki     = 0.2;
const float Chassis_LB_Kd     = 5;
const float Chassis_LB_Imax   = 1000;
const float Chassis_LB_PIDmax = 10000;

/*------------------------------------------------底盘PID设置(速度环控制)------------------------------------------------------------*/

void Chassis_LF_PID_SET(void)
{
	
	Chassis_LF_PID.Current = Real_Speed_Value[0];
	Chassis_LF_PID.Target  = ChassisParam.LF.TargetSpeed;
	Chassis_LF_PID.Kp      = Chassis_LF_Kp;
	Chassis_LF_PID.Ki      = Chassis_LF_Ki;
	Chassis_LF_PID.Kd      = Chassis_LF_Kd;
	Chassis_LF_PID.Imax    = Chassis_LF_Imax;
	Chassis_LF_PID.PIDmax  = Chassis_LF_PIDmax;

}

void Chassis_RF_PID_SET(void)
{
	
	Chassis_RF_PID.Current = Real_Speed_Value[1];
	Chassis_RF_PID.Target  = ChassisParam.RF.TargetSpeed;
	Chassis_RF_PID.Kp      = Chassis_RF_Kp;
	Chassis_RF_PID.Ki      = Chassis_RF_Ki;
	Chassis_RF_PID.Kd      = Chassis_RF_Kd;
	Chassis_RF_PID.Imax    = Chassis_RF_Imax;
	Chassis_RF_PID.PIDmax  = Chassis_RF_PIDmax;

}

void Chassis_RB_PID_SET(void)
{
	
	Chassis_RB_PID.Current = Real_Speed_Value[2];
	Chassis_RB_PID.Target  = ChassisParam.RB.TargetSpeed;
	Chassis_RB_PID.Kp      = Chassis_RB_Kp;
	Chassis_RB_PID.Ki      = Chassis_RB_Ki;
	Chassis_RB_PID.Kd      = Chassis_RB_Kd;
	Chassis_RB_PID.Imax    = Chassis_RB_Imax;
	Chassis_RB_PID.PIDmax  = Chassis_RB_PIDmax;

}

void Chassis_LB_PID_SET(void)
{
	
	Chassis_LB_PID.Current = Real_Speed_Value[3];
	Chassis_LB_PID.Target  = ChassisParam.LB.TargetSpeed;
	Chassis_LB_PID.Kp      = Chassis_LB_Kp;
	Chassis_LB_PID.Ki      = Chassis_LB_Ki;
	Chassis_LB_PID.Kd      = Chassis_LB_Kd;
	Chassis_LB_PID.Imax    = Chassis_LB_Imax;
	Chassis_LB_PID.PIDmax  = Chassis_LB_PIDmax;

}

void Chassis_PID_Control(void)
{
  Chassis_LF_PID_SET();
	Chassis_RF_PID_SET();
	Chassis_RB_PID_SET();
	Chassis_LB_PID_SET();

	ChassisParam.LF.NeedCurrent = Driver_PID(&Chassis_LF_PID);
	ChassisParam.RF.NeedCurrent = Driver_PID(&Chassis_RF_PID);
	ChassisParam.RB.NeedCurrent = Driver_PID(&Chassis_RB_PID);
	ChassisParam.LB.NeedCurrent = Driver_PID(&Chassis_LB_PID);

}

/*----------------------------------------------------PID计算函数-----------------------------------------------------------------*/

void Dial_Wheel_position_set(void)
{
  Dial_wheel_position_pid.Target = Dial_Wheel_target_speed_value;
	Dial_wheel_position_pid.Current= Gun_Real_Speed_Value;
	*Dial_wheel_position_pid.Error = *dial_wheel_difference;
	Dial_wheel_position_pid.Kp = DAIL_P;
	Dial_wheel_position_pid.Ki = DAIL_I;
	Dial_wheel_position_pid.Kd = DAIL_D;
	Dial_wheel_position_pid.PIDmax = DAIL_ESC_MAX;
}

void pitch_position_pid_set(void)
{		
	pitch_position_pid.Target = control_pitch_limited+PITCH_CENTER;          
	pitch_position_pid.Current = pitch_angle;
	*pitch_position_pid.Error = *pitch_position_difference;
	pitch_position_pid.Kp=PITCH_P;
	pitch_position_pid.Ki=PITCH_I;
	pitch_position_pid.Kd=PITCH_D;
	pitch_position_pid.PIDmax = PITCH_ESC_MAX;
};

void pitch_speed_pid_set(void)
{		
	pitch_speed_pid.Target = current_output_pitch;        
	pitch_speed_pid.Current = pitch_speed;
	*pitch_speed_pid.Error = *pitch_speed_difference;
	pitch_speed_pid.Kp=PITCH_SPEED_P;
	pitch_speed_pid.Ki=PITCH_SPEED_I;
	pitch_speed_pid.Kd=PITCH_SPEED_D;
	pitch_speed_pid.PIDmax = PITCH_ESC_MAX;
};

void yaw_position_pid_set(void)
{		
	yaw_position_pid.Target = control_yaw_limited+YAW_CENTER;
	yaw_position_pid.Current = yaw_angle;
	*yaw_position_pid.Error = *yaw_position_difference;
	yaw_position_pid.Kp=YAW_P;
	yaw_position_pid.Ki=YAW_I;
	yaw_position_pid.Kd=YAW_D;
	yaw_position_pid.PIDmax = YAW_ESC_MAX;
};

void yaw_speed_pid_set(void)
{		
	yaw_speed_pid.Target = current_output_yaw;          
	yaw_speed_pid.Current = yaw_speed;
	*yaw_speed_pid.Error = *yaw_speed_difference;
	yaw_speed_pid.Kp=YAW_SPEED_P;
	yaw_speed_pid.Ki=YAW_SPEED_I;
	yaw_speed_pid.Kd=YAW_SPEED_D;
	yaw_speed_pid.PIDmax = YAW_ESC_MAX;
};

void go_straight_pid_set(void)
{		
	go_straight_pid.Target = gpyo_register;
	go_straight_pid.Current = MPU6500_Yaw_turn_baiwei;
	*go_straight_pid.Error = *go_straight_difference;
	go_straight_pid.Kp=GO_P;
	go_straight_pid.Ki=GO_I;
	go_straight_pid.Kd=GO_D;
	go_straight_pid.PIDmax = GO_STRAIGHT_MAX;
}

void underpan_follow_pid_set(void)
{		
	underpan_follow_pid.Target = follow_angle;
	underpan_follow_pid.Current = -yaw_angle + yaw_angle_zero;
	*underpan_follow_pid.Error = *underpan_follow_difference;
	underpan_follow_pid.Kp=UNDERPAN_FOLLOW_P;
	underpan_follow_pid.Ki=UNDERPAN_FOLLOW_I;
	underpan_follow_pid.Kd=UNDERPAN_FOLLOW_D;
	underpan_follow_pid.PIDmax = GO_STRAIGHT_MAX;
}

void yuntai_position_pid_set(void)
{		
	yuntai_position_pid.Target = control_yaw_limited;
	yuntai_position_pid.Current = MPU9250_Yaw_turn;
	*yuntai_position_pid.Error = *yuntai_position_difference;
	yuntai_position_pid.Kp = YAW_P;
	yuntai_position_pid.Ki = YAW_I;
	yuntai_position_pid.Kd = YAW_D;
	yuntai_position_pid.PIDmax = YAW_ESC_MAX;
}

void yuntai_pitch_position_pid_set(void)
{		
	yuntai_pitch_position_pid.Target = control_pitch_limited;
	yuntai_pitch_position_pid.Current = MPU9250_Pitch_turn;
	*yuntai_pitch_position_pid.Error = *yuntai_pitch_position_difference;
	yuntai_pitch_position_pid.Kp = PITCH_P;
	yuntai_pitch_position_pid.Ki = PITCH_I;
	yuntai_pitch_position_pid.Kd = PITCH_D;
	yuntai_pitch_position_pid.PIDmax = PITCH_ESC_MAX;
}

void yaw_track_position_pid_set(void)
{		
	yaw_track_position_pid.Target = 0;
	yaw_track_position_pid.Current = -Buff_yaw;
	yaw_track_position_pid.Kp = Track_Yaw_P;
	yaw_track_position_pid.Ki = Track_Yaw_I;
	yaw_track_position_pid.Kd = Track_Yaw_D;
	yaw_track_position_pid.PIDmax = Track_Yaw_MAX;
}

void pitch_track_position_pid_set(void)
{		
	pitch_track_position_pid.Target = 0;
	pitch_track_position_pid.Current = -Buff_pitch;
	pitch_track_position_pid.Kp = Track_Pitch_P;
	pitch_track_position_pid.Ki = Track_Pitch_I;
	pitch_track_position_pid.Kd = Track_Pitch_D;
	pitch_track_position_pid.PIDmax = Track_Pitch_MAX;
}
/*-----------------------------------------------------------------------------------------*/


void inc_yaw_track_position_pid_set(void)
{		
	inc_yaw_track_position_pid.Target = MPU9250_Yaw_turn;
	inc_yaw_track_position_pid.Current = MPU9250_Yaw_turn-Buff_yaw;
	inc_yaw_track_position_pid.Kp = Track_Yaw_P;
	inc_yaw_track_position_pid.Ki = Track_Yaw_I;
	inc_yaw_track_position_pid.Kd = Track_Yaw_D;
	inc_yaw_track_position_pid.PIDmax = Track_Yaw_MAX;
}
float Driver_PID(PID *ps)			//位置式PID
{
	ps->Error[0] =  ps->Error[1];
	ps->Error[1] =  ps->Target - ps->Current;
	
	ps->Pout     =  ps->Kp * ps->Error[1];
	
	if(abs(ps->Error[1])>0.5f)
	ps->Iout     =  ps->Error[1] + ps->Iout;
	
	ps->Iout     =  ps->Iout > ps->Imax ? ps->Imax : ps->Iout;
	ps->Iout     =  ps->Iout < -ps->Imax ? -ps->Imax : ps->Iout;
	
	ps->Dout     =  ps->Kd * ( ps->Error[1] - ps->Error[0] );
	
  ps->PIDout   =  ps->Pout + ps->Ki * ps->Iout + ps->Dout;
	ps->PIDout   =  ps->PIDout > ps->PIDmax ? ps->PIDmax : ps->PIDout;
	ps->PIDout   =  ps->PIDout < -ps->PIDmax ? -ps->PIDmax : ps->PIDout;

	return ps->PIDout;
}



float Driver_Increment_PID(Increment_PID *ps)	//增量式PID
{
	ps->Error[0] =  ps->Error[1];
	ps->Error[1] =  ps->Error[2];
	ps->Error[2] =  ps->Target - ps->Current;
	
	ps->ECur     =  ps->Kp * (ps->Error[2] - ps->Error[1]);
	
	ps->ELast    =  ps->Ki * ps->Error[2];
	
	ps->EPre     =  ps->Kd * (ps->Error[2] - 2*ps->Error[1] + ps->Error[0]);
	
  ps->PIDout   =  ps->ECur + ps->ELast + ps->EPre;
	ps->PIDout   =  ps->PIDout > ps->PIDmax ? ps->PIDmax : ps->PIDout;
	ps->PIDout   =  ps->PIDout < -ps->PIDmax ? -ps->PIDmax : ps->PIDout;

	return ps->PIDout;
}

void clear_struct(PID *ps)
{
	ps->Error[0] = 0;
	ps->Error[1] = 0;

	ps->Iout = 0; 

	ps->PIDout = 0;

}
