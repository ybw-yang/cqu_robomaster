#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

#include "main.h"

typedef unsigned char uint8_t;

typedef struct
{
	int	direction;	//����0-7��˳ʱ�룬����߿�ʼ
	int	time1;			//�����ش���ʱ��������ֵ
	int	time2;			//�½��ش���ʱ��������ֵ
	u8	time_flag;	//���������غ��½��ش���
	u8	time_update_flag;	//�ж�����ж�
	u8	crash_flag;	//����С�ڸ���ʱ�ı�־λ
	u8	start_flag;	//��������ʼ���еı�־λ����һ�β���ʱ����λ
	float	distance;	//����
} Ultrasonic;

extern Ultrasonic ultrasonic[8];
	
void Ultrasonic_Configuration(void);
void Ultrasonic_crash(void);
void Ultrasonic_Init(void);

#endif
