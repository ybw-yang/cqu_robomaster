#ifndef __TASK_MPU9250_H
#define __TASK_MPU9250_H

void CopeSerial2Data(int ucData);
struct SAcc
{
	double a[3];
	double Temperature;
};
struct SGyro
{
	double w[3];
	double Temperature;
};
struct SAngle
{
	double Angle[3];
	double Temperature;
};
struct SMag
{
	double h[3];
	double Temperature;
};
extern float  mpu_9250_yaw_angle,mpu_9250_pitch_angle,mpu_9250_roll_angle;
extern float  mpu_9250_yaw_acc,mpu_9250_pitch_acc,mpu_9250_roll_acc;
extern float  mpu_9250_yaw_speed,mpu_9250_pitch_speed,mpu_9250_roll_speed;

#endif
