#ifndef _IWDG_H
#define _IWDG_H

#include "main.h"

extern u8 sbus_warning;
extern u32 sbus_num0;

void IWDG_Init(u8 prer,u16 rlr);//IWDG��ʼ��
void IWDG_Feed(void);  //ι������
void IWDG_judge(void);
#endif
