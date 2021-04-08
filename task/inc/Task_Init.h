#ifndef __TASK_INIT_H__
#define __TASK_INIT_H__

#include "main.h"

void ctrl_Init(void);
void param_Init(void);

extern float YAW_ANGLE_DIFFERENCE; 
extern float PITCH_ANGLE_DIFFERENCE;
extern float yaw_LSB;
extern float yaw_LSB_mouse;
extern float pitch_LSB;
extern float pitch_LSB_mouse;
extern float mpu_9250_yaw_first;
extern float mpu_9250_pitch_first;
extern float mpu_9250_yaw_zero;// мсбщргио╣Г╫г╤х
extern float mpu_9250_pitch_zero;
extern float yaw_angle_zero;
extern u8 	 whole_init;
#endif 

