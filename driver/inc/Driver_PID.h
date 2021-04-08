#ifndef __DRIVER_PID_H
#define __DRIVER_PID_H

typedef struct 
	{
		float Target;            
		float Current;         
		float Error[2];                               
		float Kp,Ki,Kd;
		float Pout,Iout,Dout,PIDout;
		float Imax,PIDmax;		             
	} PID;
	
typedef struct 
{
	float Target;            
	float Current;         
	float Error[3];                               
	float Kp,Ki,Kd;
	float ECur,ELast,EPre,PIDout;
	float PIDmax;		             
} Increment_PID;

extern PID ChaseeisPID;	
	
extern PID pitch_position_pid;
extern PID pitch_speed_pid;
extern PID yaw_position_pid;
extern PID yaw_speed_pid;	
extern PID go_straight_pid;
extern PID yuntai_position_pid;
extern PID yuntai_pitch_position_pid;
extern PID underpan_follow_pid;
extern PID Dial_wheel_position_pid;	
extern PID yaw_track_position_pid;
extern PID pitch_track_position_pid;

extern Increment_PID inc_yaw_track_position_pid;
extern int follow_angle;	

float Driver_Increment_PID(Increment_PID *ps);
float Driver_PID(PID *ps);
void clear_struct(PID *ps);

void Chassis_LF_PID_SET(void);
void Chassis_RF_PID_SET(void);
void Chassis_RB_PID_SET(void);
void Chassis_LB_PID_SET(void);
	
void Chassis_PID_Control(void);
//位置型
void pitch_position_pid_set(void);
void pitch_speed_pid_set(void);
void yaw_position_pid_set(void);
void yaw_speed_pid_set(void);
void go_straight_pid_set(void);
void yuntai_position_pid_set(void);
void yuntai_pitch_position_pid_set(void);
void underpan_follow_pid_set(void);
void Dial_Wheel_position_set(void);
void yaw_track_position_pid_set(void);
void pitch_track_position_pid_set(void);
//增量型
void inc_yaw_track_position_pid_set(void);
#endif
