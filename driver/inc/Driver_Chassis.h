#ifndef __DRIVER_CHASSIS_H
#define __DRIVER_CHASSIS_H

#include "main.h"


//速度枚举
typedef enum
{
    ChassisSpeedLevel_Hight,
    ChassisSpeedLevel_Low
}ChassisSpeedLevel_Enum;


//单电机参数
typedef struct
{
    int16_t TargetSpeed;             //电机目标速度
    int16_t RealSpeed;               //电机实际速度(RPM)
	  int16_t RealAngle;               //电机实际角度(0-8191)
    int16_t LimitCurrent;            //限制电流
    int16_t NeedCurrent;             //需求电流
}OneMotorParam_Struct;


//底盘电机参数结构体
typedef struct
{
    OneMotorParam_Struct LF;
    OneMotorParam_Struct RF;
    OneMotorParam_Struct LB;
    OneMotorParam_Struct RB;
    ChassisSpeedLevel_Enum SpeedLevel;
    float TargetVX;
    float TargetVY;
    float TargetOmega;
    float TargetABSAngle;
}ChassisParam_Struct;

extern ChassisParam_Struct ChassisParam;

void Chassis_Control(void);
void Chassis_SendMotorParam(void);
#endif
