#ifndef __TASK_WEAPONS_H__
#define __TASK_WEAPONS_H__

#include "main.h"

extern int sendgunFlag;
extern u8 mocaflag;
extern int Dial_Wheel_target_speed_value;
extern float dial_control;

void Dial_wheel_on_off(void);
void weapon_ctrl(void);
void sendgun(void);//µ¥·¢
#endif
