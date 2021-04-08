
#ifndef __DRIVER_POKEMOTOR_H
#define __DRIVER_POKEMOTOR_H

#include "Config.h"
#include "stm32f4xx.h"
#include "OSinclude.h"


#define POKEENCODERLINESPERCIRCLE       5050        //�������������ÿ������
#define POKELINESPERSTEP                820         //�������������������

//��������������е�λ��
#define POKEENCODERCenter               0x7FFF


#ifdef  __DRIVER_POKEMOTOR_GLOBALS
#define __DRIVER_POKEMOTOR_EXT
#else
#define __DRIVER_POKEMOTOR_EXT extern
#endif


//�������״̬ö��
typedef enum
{
    PokeMotorParam_Working,                         //����
    PokeMotorParam_StuckDealing,                    //���ڷ�ת������
    PokeMotorParam_Stuck                            //����
}PokeChassisParam_Enum;  
    
    
//�����������    
typedef struct  
{   
    int16_t RealSpeed;              //��������ٶ�
    long RealLocation;              //�������ʵ��λ��
    long TargetLocation;            //�������Ŀ��λ��
    PokeChassisParam_Enum Status;   //״̬
    portTickType LastShotTick;      //�ϴη���ʱ��
}PokeMotorParam_Struct;


#if FRICTYPE == 0
//�����������
__DRIVER_POKEMOTOR_EXT PokeMotorParam_Struct PokeMotorParam;
#endif


void Poke_InitConfig(void);

#if FRICTYPE == 1

void Poke_CylinderAdjust(uint8_t Target);
void Poke_CylinderControl(uint8_t Target);

#else

void Poke_MotorCurrent(int16_t Current);
uint8_t Poke_MotorStep(void);
void Poke_MotorAdjust(uint8_t mode);
void Poke_MotorSpeedAdjust(uint8_t mode, uint8_t speed);

#endif




#endif
