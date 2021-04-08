
#ifndef __TASK_DECODE_H__
#define __TASK_DECODE_H__

#include "main.h"

typedef struct _decode_keyboard
	{
		u8 key_none;
		u8 key_w;
		u8 key_a;
		u8 key_s;
		u8 key_d;
		u8 key_q;
		u8 key_e;
		u8 key_ctrl;
		u8 key_shift;          
	} decode_keyboard;

extern decode_keyboard decode_the_key;	

extern int radio_ahead_back_data;//×Ü¿ØÖÆ
extern int radio_left_right_data;
extern int radio_turn_data;

extern int radio_ahead_back_data_circle;//»­Ô²
extern int radio_left_right_data_circle;
	
extern float radio_ahead_back_data_avoidance;//³¬Éù²¨±ÜÕÏ
extern float radio_left_right_data_avoidance;

extern int radio_ahead_back_data_keyboard;//¼üÅÌ¿ØÖÆ
extern int radio_left_right_data_keyboard;
extern int radio_turn_data_keyboard;
extern int radio_turn_data_mouse;

extern int radio_ahead_back_data_remote;//Ò£¿Ø¿ØÖÆ
extern int radio_left_right_data_remote;
extern int radio_turn_data_remote;

extern u8 mouse_state_keyboard,mouse_state_remote,mouse_state;
	
extern u8 decode_flag;
extern float radio_turn_data_follow;
extern float radio_turn_data_baiwei,radio_turn_data_straight;
	
extern float radio_2,radio_2_remote,radio_2_keyboard;
extern float radio_3,radio_3_remote,radio_3_keyboard;

extern int s1_position;	
extern int s1,s2;
extern float wheel[4];
extern u16	keyTimeOfUnderpan;

extern int radio_positive_spin_data;
extern int radio_negative_spin_data; //×ÔĞı±äÁ¿
	
void compute_key_time_of_underpan(void);
void data_decoder(void);
void Upper_part(void);
void Yaokong_Ctrl(void);
void Keyboard_Ctrl(void);
void dipan_Ctrl(void) ;


#endif 
