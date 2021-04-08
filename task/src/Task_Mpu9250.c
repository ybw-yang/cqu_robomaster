
#include "main.h"
#include <string.h>

u16 mpu9250_data[10]={0};
u16 ch;
int cn=0;
float  mpu_9250_yaw_angle,mpu_9250_pitch_angle,mpu_9250_roll_angle;
float  mpu_9250_yaw_acc,mpu_9250_pitch_acc,mpu_9250_roll_acc;
float  mpu_9250_yaw_speed,mpu_9250_pitch_speed,mpu_9250_roll_speed;
struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 		stcAngle;
struct SMag 		stcMag;

void CopeSerial2Data(int ucData)
{
	static int ucRxBuffer[250];
	static int ucRxCnt = 0;	
	ucRxBuffer[ucRxCnt]=ucData;
	ucRxCnt++;
	if (ucRxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//数据不满11个，则返回
	else
	{
		switch(ucRxBuffer[1])
		{
			case 0x51:
						stcAcc.a[0]=((ucRxBuffer[3]<<8) | ucRxBuffer[2]);//32768.0*16*9.8;
						stcAcc.a[1]=((ucRxBuffer[5]<<8) | ucRxBuffer[4]);//32768.0*16*9.8;
						stcAcc.a[2]=((ucRxBuffer[7]<<8) | ucRxBuffer[6]);//32768.0*16*9.8;
						stcAcc.Temperature=((ucRxBuffer[9]<<8) | ucRxBuffer[8])/100;
						if(stcAcc.a[0]>0x7fff) stcAcc.a[0]-=0xffff;
						if(stcAcc.a[1]>0x7fff) stcAcc.a[1]-=0xffff;
						if(stcAcc.a[2]>0x7fff) stcAcc.a[2]-=0xffff;
						mpu_9250_roll_acc = stcAcc.a[0]/32768*16*9.8;
						mpu_9250_pitch_acc = stcAcc.a[1]/32768*16*9.8;
						mpu_9250_yaw_acc = stcAcc.a[2]/32768*16*9.8;
						break;
			case 0x52:
						stcGyro.w[0]=((ucRxBuffer[3]<<8) | ucRxBuffer[2]);//32768.0*16*9.8;
						stcGyro.w[1]=((ucRxBuffer[5]<<8) | ucRxBuffer[4]);//32768.0*16*9.8;
						stcGyro.w[2]=((ucRxBuffer[7]<<8) | ucRxBuffer[6]);//32768.0*16*9.8;
						stcGyro.Temperature=((ucRxBuffer[9]<<8) | ucRxBuffer[8])/100;
						if(stcGyro.w[0]>0x7fff) stcGyro.w[0]-=0xffff;
						if(stcGyro.w[1]>0x7fff) stcGyro.w[1]-=0xffff;
						if(stcGyro.w[2]>0x7fff) stcGyro.w[2]-=0xffff;
						mpu_9250_roll_speed = stcGyro.w[0]/32768.0*2000;
						mpu_9250_pitch_speed = stcGyro.w[1]/32768.0*2000;
						mpu_9250_yaw_speed = stcGyro.w[2]/32768.0*2000;
						break;
			case 0x53:
						stcAngle.Angle[0]=((ucRxBuffer[3]<<8) | ucRxBuffer[2]);//32768.0*16*9.8;
						stcAngle.Angle[1]=((ucRxBuffer[5]<<8) | ucRxBuffer[4]);//32768.0*16*9.8;
						stcAngle.Angle[2]=((ucRxBuffer[7]<<8) | ucRxBuffer[6]);//32768.0*16*9.8;
						stcAngle.Temperature=((ucRxBuffer[9]<<8) | ucRxBuffer[8])/100;
						if(stcAngle.Angle[0]>0x7fff) stcAngle.Angle[0]-=0xffff;
						if(stcAngle.Angle[1]>0x7fff) stcAngle.Angle[1]-=0xffff;
						if(stcAngle.Angle[2]>0x7fff) stcAngle.Angle[2]-=0xffff;
						mpu_9250_roll_angle = stcAngle.Angle[0]/32768.0*180;
						mpu_9250_pitch_angle = stcAngle.Angle[1]/32768.0*180;
						mpu_9250_yaw_angle = stcAngle.Angle[2]/32768.0*180;
						break;
			case 0x54:
						stcMag.h[0]=((ucRxBuffer[3]<<8) | ucRxBuffer[2]);
						stcMag.h[1]=((ucRxBuffer[5]<<8) | ucRxBuffer[4]);
						stcMag.h[2]=((ucRxBuffer[7]<<8) | ucRxBuffer[6]);
						if(stcMag.h[0]>0x7fff) stcMag.h[0]-=0xffff;
						if(stcMag.h[1]>0x7fff) stcMag.h[1]-=0xffff;
						if(stcMag.h[2]>0x7fff) stcMag.h[2]-=0xffff;
						stcMag.Temperature=((ucRxBuffer[9]<<8) | ucRxBuffer[8])/100;
						break;
		}
		for(ucRxCnt=0;ucRxCnt<11;ucRxCnt++)
		{
			ucRxBuffer[ucRxCnt]=0;
		}
		ucRxCnt=0;
		if(whole_init==1)
		{
			MPU9250_Pitch_turn = first_order_filter(MPU9250_Pitch, mpu_9250_pitch_angle - mpu_9250_pitch_zero, MPU9250_Pitch_turn, 50);//pitch陀螺仪连续化
			MPU9250_Yaw_turn   = first_order_filter(MPU9250_Yaw,   mpu_9250_yaw_angle   - mpu_9250_yaw_zero,   MPU9250_Yaw_turn,   50);//yaw陀螺仪连续化
		}
	}
}


