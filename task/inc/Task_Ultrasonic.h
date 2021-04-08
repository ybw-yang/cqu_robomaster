#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

#include "main.h"

typedef unsigned char uint8_t;

typedef struct
{
	int	direction;	//方向，0-7，顺时针，从左边开始
	int	time1;			//上升沿触发时计数器的值
	int	time2;			//下降沿触发时计数器的值
	u8	time_flag;	//区分上升沿和下降沿触发
	u8	time_update_flag;	//判断溢出中断
	u8	crash_flag;	//距离小于给定时的标志位
	u8	start_flag;	//超声波开始运行的标志位，第一次捕获时就置位
	float	distance;	//距离
} Ultrasonic;

extern Ultrasonic ultrasonic[8];
	
void Ultrasonic_Configuration(void);
void Ultrasonic_crash(void);
void Ultrasonic_Init(void);

#endif
