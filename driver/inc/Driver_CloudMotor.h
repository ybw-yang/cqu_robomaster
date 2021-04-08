
#ifndef __DRIVER_CLOUDMOTOR_H
#define __DRIVER_CLOUDMOTOR_H

#include "Config.h"
#include "stm32f4xx.h"


//���CAN ID
#define YAWMOTORCANID		0x205
#define PITCHMOTORCANID		0x206

#define COULDCONTROLID	    0x1FF


//�Ƕ��޷�
#if INFANTRY == 1       //��Ħ

    #define PitchEncoderUPLimit             5500            //��̨��е���ޱ�����ֵ
    #define PitchEncoderDOWNLimit           4550            //��̨��е���ޱ�����ֵ
    #define PitchEncoderCenter              4820            //��̨Pitchˮƽ������ֵ
    #define YawEncoderLEFTLimit             5600            //��̨��е���ޱ�����ֵ
    #define YawEncoderRIGHTLimit            3800            //��̨��е�Ҽ��ޱ�����ֵ
    #define YawEncoderCenter                4650            //��̨Yaw�б�����ֵ

    #define PitchABSUPLimit                 60.0F           //�������ԽǶ��ϼ���
    #define PitchABSDOWNLimit               -40.0F          //�������ԽǶ��¼���
    
    #define PitchEncoderPassZeroBoundary    1               //Pitch����������ֽ�ֵ�����ֵ�жϹ���
    #define YawEncoderPassZeroBoundary      1               //Pitch����������ֽ�ֵ�����ֵ�жϹ���

#elif INFANTRY == 2     //����˹��

    #define PitchEncoderUPLimit             3500
    #define PitchEncoderDOWNLimit           2470
    #define PitchEncoderCenter              2808
    #define YawEncoderLEFTLimit             5150
    #define YawEncoderRIGHTLimit            3150
    #define YawEncoderCenter                4122
    
    #define PitchABSUPLimit                 60.0F 
    #define PitchABSDOWNLimit               -40.0F
    
    #define PitchEncoderPassZeroBoundary    1
    #define YawEncoderPassZeroBoundary      1

#elif INFANTRY == 3     //��ʿ��

    #define PitchEncoderUPLimit             5600
    #define PitchEncoderDOWNLimit           4740
    #define PitchEncoderCenter              5038
    #define YawEncoderLEFTLimit             4600
    #define YawEncoderRIGHTLimit            2220
    #define YawEncoderCenter                3380
    
    #define PitchABSUPLimit                 60.0F 
    #define PitchABSDOWNLimit               -40.0F
    
    #define PitchEncoderPassZeroBoundary    1
    #define YawEncoderPassZeroBoundary      1

#elif INFANTRY == 4     //����

    #define PitchEncoderUPLimit             5500
    #define PitchEncoderDOWNLimit           4480
    #define PitchEncoderCenter              4862
    #define YawEncoderLEFTLimit             4570
    #define YawEncoderRIGHTLimit            2450
    #define YawEncoderCenter                3380
                
    #define PitchABSUPLimit                 60.0F 
    #define PitchABSDOWNLimit               -40.0F
    
    #define PitchEncoderPassZeroBoundary    1
    #define YawEncoderPassZeroBoundary      1

#elif INFANTRY == 5     //����

    #define PitchEncoderUPLimit             5830
    #define PitchEncoderDOWNLimit           4910
    #define PitchEncoderCenter              5066
    #define YawEncoderLEFTLimit             10400
    #define YawEncoderRIGHTLimit            8200
    #define YawEncoderCenter                9265
                
    #define PitchABSUPLimit                 60.0F 
    #define PitchABSDOWNLimit               -40.0F
    
    #define PitchEncoderPassZeroBoundary    1
    #define YawEncoderPassZeroBoundary      5000

#elif INFANTRY == 6     //СӢ��

    #define PitchEncoderUPLimit             4460
    #define PitchEncoderDOWNLimit           3490
    #define PitchEncoderCenter              4012
    #define YawEncoderLEFTLimit             7100
    #define YawEncoderRIGHTLimit            5000
    #define YawEncoderCenter                6180
                
    #define PitchABSUPLimit                 60.0F 
    #define PitchABSDOWNLimit               -40.0F
    
    #define PitchEncoderPassZeroBoundary    1
    #define YawEncoderPassZeroBoundary      1000

#elif INFANTRY == 7     //����

    #define PitchEncoderUPLimit             7850
    #define PitchEncoderDOWNLimit           7030
    #define PitchEncoderCenter              7360
    #define YawEncoderLEFTLimit             4340
    #define YawEncoderRIGHTLimit            2370
    #define YawEncoderCenter                3335
                
    #define PitchABSUPLimit                 60.0F 
    #define PitchABSDOWNLimit               -40.0F
    
    #define PitchEncoderPassZeroBoundary    2000
    #define YawEncoderPassZeroBoundary      1000

#endif




#ifdef  __CLOUDMOTOR_GLOBALS
#define __CLOUDMOTOR_EXT
#else   
#define __CLOUDMOTOR_EXT extern
#endif


//�Ƕ�����ö��
typedef enum
{
    AngleMode_OPP,          //��ԽǶ�
    AngleMode_ABS           //���ԽǶ�
}AngleMode_Enum;


//��̨��������ṹ��
typedef struct
{
    uint16_t RealEncoderAngle;                  //����Ƕȱ�����ֵ  
    float RealABSAngle;                         //ʵ�ʽǶȣ�����ֵ��
    float TargetABSAngle;                       //Ŀ��Ƕȣ�����ֵ��
    uint16_t FrameCounter;                      //֡�ʼ�����
    uint16_t FrameRate;                         //֡��
}ClourMotorParam_Struct;

//��̨�����ṹ��
typedef struct
{
    ClourMotorParam_Struct Pitch;               //Pitch�������
    ClourMotorParam_Struct Yaw;                 //Yaw�������
    uint8_t Lock;                               //0 ��������̨���ֹͣ       1 ��������
}CloudParam_Struct;


//��̨����
__CLOUDMOTOR_EXT CloudParam_Struct CloudParam;


void CloudMotor_InitConfig(void);
void CloudMotorCurrent(int16_t Pitch, int16_t Yaw);
void Cloud_Adjust(uint8_t mode);
void Cloud_AutoAdjust(float FeedSpeed, uint8_t mode);
void Cloud_YawAngleSet(float Target, AngleMode_Enum mode);
void Cloud_PitchAngleSet(float Target);


#endif
