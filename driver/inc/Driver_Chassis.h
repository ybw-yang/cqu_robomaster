#ifndef __DRIVER_CHASSIS_H
#define __DRIVER_CHASSIS_H

#include "main.h"


//�ٶ�ö��
typedef enum
{
    ChassisSpeedLevel_Hight,
    ChassisSpeedLevel_Low
}ChassisSpeedLevel_Enum;


//���������
typedef struct
{
    int16_t TargetSpeed;             //���Ŀ���ٶ�
    int16_t RealSpeed;               //���ʵ���ٶ�(RPM)
	  int16_t RealAngle;               //���ʵ�ʽǶ�(0-8191)
    int16_t LimitCurrent;            //���Ƶ���
    int16_t NeedCurrent;             //�������
}OneMotorParam_Struct;


//���̵�������ṹ��
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
