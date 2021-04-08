

#ifndef __DRIVER_MPU9250_H
#define __DRIVER_MPU9250_H


#include "stm32f4xx.h"


#ifdef  __MPU9250_GLOBALS
#define __MPU9250_EXT
#else
#define __MPU9250_EXT   extern
#endif


//��������̬�������ݽṹ
typedef struct
{
    //ʵ������
    struct
    {
        //���ٶ�ʵ������
        float OX;
        float OY;
        float OZ;
        //���ٶ�ʵ������
        float GX;
        float GY;
        float GZ;
        //�¶�ʵ������
        float T;
    }Real;
    
    //����ʵ�ʶԵ�ŷ����
    struct
    {
        float Pitch;
        float Roll;
        float Yaw;
    }Euler;
}MPU9250_TypeDef;


typedef struct
{
    float X;
    float Y;
    float Z;
}XYZOffset_TypeDef;


typedef struct
{
    float Pitch;
    float Roll;
    float Yaw;
}EulerOffset_TypeDef;


//��̬����
__MPU9250_EXT MPU9250_TypeDef Position;
//ƫ����������
__MPU9250_EXT XYZOffset_TypeDef Omega_Offset;
__MPU9250_EXT EulerOffset_TypeDef Euler_Offset;


void MPU9250_InitConfig(void);
uint8_t MPU9250_Update(void);


#endif
