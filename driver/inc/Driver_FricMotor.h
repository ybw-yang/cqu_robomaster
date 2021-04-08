#ifndef __DRIVER_FRICMOTOR_H
#define __DRIVER_FRICMOTOR_H


#include "Config.h"
#include "stm32f4xx.h"


#ifdef  __DRIVER_FRICMOTOR_GLOBALS
#define __DRIVER_FRICMOTOR_EXT
#else
#define __DRIVER_FRICMOTOR_EXT extern
#endif


#define ARTILLERYFRICCANIDLEFT                  0x201               //��Ħ����CAN ID
#define ARTILLERYFRICCANIDRIGHT                 0x202               //��Ħ����CAN ID
#define ARTILLERYFRICCONTROLCANID               0x200               //Ħ���ֿ���ID


#define ARTILLERYFRICMAXSPEED                  8600                //��Ħ��������ٶ�
#define ARTILLERYFRICMAXCURREN                 30000               //Ħ����������
#define ARTILLERYFRICMINCURRENOFTMAXSPEED      1200                //Ħ��������ٶ�ʱ����Ҫ����С����
#define ARTILLERYACCBELOWSPEED                 7000                //��Ħ�����ٶȵ��ڴ��ٶ�ʱ������������


//СӢ��ǹ��һ��������Ħ����
#if INFANTRY == 6
__DRIVER_FRICMOTOR_EXT int16_t ArtilleryFricRealSpeed[2];          //Ħ����ʵ���ٶ�  0��    1��
__DRIVER_FRICMOTOR_EXT int16_t ArtilleryFricTargetSpeed;           //Ħ����Ŀ���ٶ�  0ֹͣ  else ����ٶ�
#endif


void FricMotor_InitConfig(void);

#if FRICTYPE == 1
void FricArtillerySpeed_Adjust(int16_t TargetSpeed);
void FricArtilleryMotorCurrent(int16_t LeftArtilleryCurrent, int16_t RightArtilleryCurrent);
#else
void FricGunControl(uint8_t Control);
#endif



#endif
