
#ifndef __DRIVER_VISION_H
#define __DRIVER_VISION_H


#include "stm32f4xx.h"
#include "OSinclude.h"
#include "CommonDataStructure.h"


#define PCDATALENGTH            26  //֡��
#define ENEMYDATABUFFERLENGHT   60 //��������֡
#define FITDATANUMMIN           8   //��С�������������


#ifdef  __DRIVER_VISION_GLOBALS
#define __DRIVER_VISION_EXT 
#else
#define __DRIVER_VISION_EXT extern
#endif




//PCָ��ö��
typedef enum
{
    PCOrder_Attack,
    PCOrder_BigSample,
    PCOrder_Shutdown
}PCOrder_Enum;


//�Ӿ����ݽṹ
typedef struct
{
    float X;
    float Y;
    float Z;
    float TimeStamp;
    int Time;
    char ID;
    portTickType Tick;
}Enemy_Struct;

//�����ݽṹ
typedef struct
{
    float X;
    float Y;
    float Z;
}Point_Struct;

//�����ݽṹ(����������ʵ�ʽǶȣ�
typedef struct
{
    float H;
    float V;
}AngleF_Struct;

//�����ݽṹ(�������ڱ������Ƕȣ�
typedef struct
{
    int16_t H;
    int16_t V;
}AngleI_Struct;

//Ԥ�п������ݽṹ
typedef struct
{
    AngleI_Struct Target;       //Ŀ��Ƕȣ���������λ���м�Ϊ0�������Ҹ���
    portTickType TargetTick;         //�ڴ˾���ʱ�䵽��Ŀ��Ƕ�
}ForcastControl_Struct;


//����ԭʼ���ݻ���
__DRIVER_VISION_EXT uint8_t PCDataBuffer[PCDATALENGTH];
//����ԭʼ���ݻ���ָ��
__DRIVER_VISION_EXT uint8_t PCDataBufferPoint;
//�����������ݻ���
__DRIVER_VISION_EXT Enemy_Struct EnemyDataBuffer[ENEMYDATABUFFERLENGHT];
//�����������ݻ���ָ��
__DRIVER_VISION_EXT uint8_t EnemyDataBufferPoint;
//Ԥ��Ŀ������
__DRIVER_VISION_EXT ForcastControl_Struct ForcastTarget;
//���ݸ��±�־
__DRIVER_VISION_EXT uint8_t VisionUpdataFlag;

#if INFANTRY == 7
//��������Ŀ����ɫ
__DRIVER_VISION_EXT int8_t PCVisionTargetColor;         //-1 δȷ��        0 ��         1 ��
#endif

//֡��ͳ��
__DRIVER_VISION_EXT int16_t PCFrameCounter;
//֡��ͳ��
__DRIVER_VISION_EXT int16_t PCFrameRate;


extern const uint8_t AttackModeOrder[PCDATALENGTH];
extern const uint8_t BigSymbolModeOrder[PCDATALENGTH];
extern const uint8_t ShutdownOrder[PCDATALENGTH];
    

void Vision_InitConfig(void);
AngleF_Struct RecToPolar(float X, float Y, float Z, float RealPitch, uint16_t PitchEncoder, uint8_t mode);
uint8_t ForcastCore(uint16_t SampleTime, uint16_t ForcastTime, Point_Struct *ForcastPoint);
uint8_t ForcastOnce(uint16_t SampleTime, uint16_t ForcastTime, AngleF_Struct *ForcastAngle, uint8_t TimeMode);
void SendPCOrder(PCOrder_Enum order);
void SendEnemyColor(char Co);


#endif
