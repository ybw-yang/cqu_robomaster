
#ifndef __DRIVER_SUPERGYROSCOPE_H
#define __DRIVER_SUPERGYROSCOPE_H


#include "stm32f4xx.h"
#include "OSinclude.h"


#define SUPERGYROSCOPECANIC     0x11


#ifdef  __DRIVER_SUPERGYROSCOPE_GLOBALS
#define __DRIVER_SUPERGYROSCOPE_EXT
#else
#define __DRIVER_SUPERGYROSCOPE_EXT extern
#endif


//�������������ݽṹ��
typedef struct
{
    float Angle;
    float Omega;
    float Offset;
}SuperGyoParam_Struct;


//��������������
__DRIVER_SUPERGYROSCOPE_EXT SuperGyoParam_Struct SuperGyoParam;
//��������������֡����
__DRIVER_SUPERGYROSCOPE_EXT float SuperGyoFrameCounter;
//��������������֡֡��
__DRIVER_SUPERGYROSCOPE_EXT float SuperGyoFrameRate;


void SuperGyo_InitConfig(void);


#endif
