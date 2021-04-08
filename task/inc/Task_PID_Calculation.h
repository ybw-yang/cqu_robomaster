#ifndef __TASK_PID_CALCULATION_H__
#define __TASK_PID_CALCULATION_H__
#include "main.h"

extern int speed;
extern u8 baiwei_flag;
extern u8 control_flag;
extern u8 sbus_warning;
extern u8 PID_Calculation_flag;
extern u8 Graph_flag_flt[4];

void sendgun(void);
void sendgunforward(void);
void sendgunbackward(void);
#endif
