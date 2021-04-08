#ifndef __DRIVER_BELL_H
#define __DRIVER_BELL_H


#include "stm32f4xx.h"


#define MAXAWarning     65535           //���������


//��������
typedef enum
{
    None,               //�޾���
    MPUOffset,          //mpu9250����
    ArtOffset,          //��Ħ��������
    ArtAlign,           //��Ħ������У׼
    CloudOffset,        //��̨�������
    PokeStuck,          //���������ס
}Warning_TypeDef;


void Bell_InitConfig(void);
void Bell_Bark(uint8_t bark);
void Bell_BarkWarning(uint8_t Serial, uint16_t Count);



#endif
