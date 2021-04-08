#ifndef __DRIVER_JUDGE_H
#define __DRIVER_JUDGE_H


#include "Config.h"
#include "stm32f4xx.h"
#include "OSinclude.h"

#define JudgeBufferLength       150
#define JudgeFrameLength_1      46
#define JudgeFrameLength_2      11
#define JudgeFrameLength_3      24

#define JudgeFrameHeader        0xA5        //֡ͷ 


#ifdef  __DRIVER_GLOBALS
#define __DRIVER_EXT
#else
#define __DRIVER_EXT extern
#endif


//С��״̬ö��
typedef enum
{
    BUFF_TYPE_NONE, //��Ч
    BUFF_TYPE_ARMOR = 0x01, //������
    BUFF_TYPE_SUPPLY = 0x04, //��Ѫ��
    BUFF_TYPE_BULLFTS= 0x08, //�ӵ���
}LBuffType_Enum;


//λ��״̬�ṹ��
typedef __packed struct
{
    uint8_t flag; //0 ��Ч�� 1 ��Ч
    uint32_t x;
    uint32_t y;
    uint32_t z;
    uint32_t compass;
}GpsData_Struct;


//����������Ϣ�ṹ��
typedef __packed struct
{
    uint32_t remainTime;
    uint16_t remainLifeValue;
    float realChassisOutV;
    float realChassisOutA;
    uint8_t runeStatus[4];
    uint8_t bigRune0Status;
    uint8_t bigRune1status;
    uint8_t conveyorBelts0:2;
    uint8_t conveyorBelts1:2;
    uint8_t parkingApron0:1;
    uint8_t parkingApron1:1;
    uint8_t parkingApron2:1;
    uint8_t parkingApron3:1;
    GpsData_Struct gpsData;
}GameInfo_Struct;


//ʵʱѪ���仯��Ϣ�ṹ��
typedef __packed struct
{
    uint8_t weakId:4;
    uint8_t way:4;
    uint16_t value;
}RealBloodChangedData_Struct;


//ʵʱ�����Ϣ�ṹ��
typedef __packed struct
{
    float realBulletShootSpeed;
    float realBulletShootFreq;
    float realGolfShootSpeed;
    float realGolfShootFreq;
}RealShootData_Struct;


//����ϵͳ�ṹ��
typedef struct
{
    float RealVoltage;                  //ʵʱ��ѹ
    float RealCurrent;                  //ʵʱ����
    int16_t LastBlood;                  //ʣ��Ѫ��
    uint8_t LastHartID;                 //�ϴ��յ��˺���װ�װ�ID��
    portTickType LastHartTick;          //�ϴ����˺�ʱ�� 
    float LastShotSpeed;                //�ϴ�����ٶ�
    portTickType LastShotTick;          //�ϴ����ʱ��
#if INFANTRY == 7
    uint16_t ShootNum;                  //�ѷ����ӵ���
    uint8_t BulletUseUp;                //1 �����ӵ�����          0 �����ӵ�δ����
    uint16_t ShootFail;                 //����ʧ��ʱ�� 
#endif
}InfantryJudge_Struct;


//����ϵͳ���ݻ���
__DRIVER_EXT uint8_t JudgeDataBuffer[JudgeBufferLength];
//ʵʱ��ѹ
__DRIVER_EXT InfantryJudge_Struct InfantryJudge;
//֡�ʼ�����
__DRIVER_EXT float JudgeFrameCounter;
//֡��
__DRIVER_EXT float JudgeFrameRate;


unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8);
unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength);
    
void Judge_InitConfig(void);



#endif
