#ifndef _IWDG_H
#define _IWDG_H

#include "main.h"

extern u8 sbus_warning;
extern u32 sbus_num0;

void IWDG_Init(u8 prer,u16 rlr);//IWDG³õÊ¼»¯
void IWDG_Feed(void);  //Î¹¹·º¯Êý
void IWDG_judge(void);
#endif
