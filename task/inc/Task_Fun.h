#ifndef __TASK_FUN_H__
#define __TASK_FUN_H__

#include "main.h"

extern const float GO_P;
extern const float GO_I;
extern const float GO_D;
extern const int GO_STRAIGHT_MAX;
extern u8    go_straight_aheadback_flag;
extern u8    go_straight_leftright_flag;
extern u8 	 go_straight_leftright_flag_keyboard;//键盘左右走至
extern u8    go_straight_aheadback_flag_keyboard;//键盘前后走至
extern u8 	 go_straight_aheadback_flag_remote;//遥控前后走直
extern u8 	 go_straight_leftright_flag_remote;//遥控左右走直
extern u8 	 turn_flag_keyboard;
extern u8		 turn_flag_remote;
extern u8    turn_flag;
extern u8    pid_turn_flag;
extern u8 	 turn_round_flag ;		
extern u8 	 separate_flag;
extern u8  	 attack_direction;

extern float MPU6500_Yaw_turn_baiwei;
extern float gpyo_register;

extern float control_yaw_limited;
extern float control_pitch_limited;

extern float current_output_yaw;
extern float current_output_pitch;

extern float current_output_pitch_speed;
extern float current_output_yaw_speed;

extern float track_yaw_output;
extern float track_pitch_output;

extern double Circle_n;
extern float go_straight_difference[2];//PID输入数组
extern float underpan_follow_difference[2];
extern float pitch_position_difference[2];
extern float yaw_position_difference[2];
extern float pitch_speed_difference[2];
extern float yaw_speed_difference[2];
extern float yuntai_position_difference[2];
extern float yuntai_pitch_position_difference[2];
extern float dial_wheel_difference[2];

extern float MPU9250_Pitch[2]; 
extern float MPU9250_Yaw[2] ;

extern float mouse_y;  
extern float mouse_x;
extern float MPU6500_Yaw_turn;
extern float MPU6500_Pitch_turn;
extern float MPU9250_Yaw_turn;
extern float MPU9250_Pitch_turn;

//移位中值滤波数组
extern float track_mousex_mean[6];
extern float track_mousey_mean[6];
extern float track_yaw_mean[6];
extern float track_pitch_mean[6];
extern float yaw_filter_positon_mean[6];
extern float pitch_filter_positon_mean[6];
extern float go_straight_filter_positon_mean[6];
extern struct PID *gs;

extern u8 yuntai_back_to_init; //云台回到初始角度

void turn_round(void);
void go_straight(void);
void Task_gpyo_read(void);
#endif 
