/***************************************************************************************************************
名称：main.h

实现功能： 集合工程所有头文件，方便调用
修改日期： 20170328
作者：     杨博炜
修改格式： 修改者+修改项目+修改日期
修改者： 

***************************************************************************************************************/


#ifndef __MAIN_H__
#define __MAIN_H__

/*-------------------------------------------------标准库------------------------------------------------------*/

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_it.h"
#include "stdio.h"	
#include <stdint.h>
#include <math.h>
#include <string.h>
/*-------------------------------------------------task------------------------------------------------------*/

#include "Task_test.h"
#include "Task_CANSend.h"
#include "Task_Control.h"
#include "Task_Weapons.h"
#include "Task_Fun.h"
#include "Tast_Filter.h"
#include "Task_Init.h"
#include "Task_Decode.h"
#include "Task_Mpu9250.h"
#include "Task_Power_Limit.h"
#include "Task_Cloud_Platforms.h"
#include "Task_PID_Calculation.h"
#include "Task_Delay.h"
#include "Task_ahrs.h"
#include "Task_Ultrasonic.h"

/*-------------------------------------------------BSP------------------------------------------------------*/

#include "BSP_GPIO.h"
#include "BSP_UART.h"
#include "BSP_NVIC.h"
#include "BSP_CAN.h"
#include "BSP_DMA.h"
#include "BSP_IWDG.h"
#include "BSP_TIM.h"

/*-------------------------------------------------driver------------------------------------------------------*/

#include "Driver_DBUS.h"
#include "Driver_Chassis.h"
#include "Driver_PID.h"
#include "Driver_mpu6500_spi.h"
#include "Driver_mpu6500.h"
#include "Driver_IST8310.h"

/*------------------------------------------------------------------------------------------------------------*/

#define abs(x) ((x)>0? (x):(-(x)))
#define TRUE	1
#define FLASE 0

#define SpeedUp         700
#define normal_Speed    700

extern const float YAW_MAXLIMIT;             
extern const float YAW_MINLIMIT;        
extern const float YAW_CENTER;      

extern const int PITCH_MAXLIMIT;          
extern const int PITCH_MINLIMIT;   
extern const int PITCH_CENTER;

extern const int Remote_Ctrl;    
extern const int Remote_Ctrl;  

extern const int PITCH_ESC_MAX;		
extern const int YAW_ESC_MAX;
extern const int GO_STRAIGHT_MAX;
extern const int DAIL_ESC_MAX;
extern const int Track_Yaw_MAX;
extern const int Track_Pitch_MAX;

extern const int RADIO_DATA_MAX;		//底盘速度最大值
extern const int RADIO_DATA_MIN;

extern const int MOUSE_OUTPUT_MAXLIMI;
extern const int YMOUSE_OUTPUT_MAXLIMIP;
extern const int YMOUSE_OUTPUT_MAXLIMIN;

//定义于main.c中
extern const float YAW_P;
extern const float YAW_I;	
extern const float YAW_D;
//定义于main.c中
extern const float PITCH_P;
extern const float PITCH_I;
extern const float PITCH_D;

extern const float PITCH_SPEED_P ;
extern const float PITCH_SPEED_I ;
extern const float PITCH_SPEED_D ;

extern const float YAW_SPEED_P ;
extern const float YAW_SPEED_I ;
extern const float YAW_SPEED_D ;

extern const float Track_Yaw_P;
extern const float Track_Yaw_I;
extern const float Track_Yaw_D;

extern const float Track_Pitch_P;
extern const float Track_Pitch_I;
extern const float Track_Pitch_D;

/*底盘跟随pid*/
extern const float UNDERPAN_FOLLOW_P;
extern const float UNDERPAN_FOLLOW_I;
extern const float UNDERPAN_FOLLOW_D;

extern const float DAIL_P;
extern const float DAIL_I;
extern const float DAIL_D;

extern const int dial_speed;
extern const float normal_Power;
extern const float power_Up;

//------超声波检测距离--------//
extern int	const	DISTANCE;

/*灵敏度*/
extern const float remote_sensitivity_x;  //遥控器灵敏度，数值越低越灵敏
extern const float remote_sensitivity_y;  //遥控器灵敏度，数值越低越灵敏

extern const float mouse_sensitivity_x;	 //鼠标灵敏度，数值越高越灵敏
extern const float mouse_sensitivity_y;	 //鼠标灵敏度，数值越高越灵敏

extern int Graph_flag;
extern float buff_yaw;

#endif 
