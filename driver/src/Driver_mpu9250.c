
#define __MPU9250_GLOBALS

#include "Config.h"
#include "Driver_mpu9250.h"
#include "OSinclude.h"
#include "mpu9250dmp.h"


/**
  * @brief  mpu9250��ʼ��
  * @param  void
  * @retval void
  */
void MPU9250_InitConfig(void)
{
#if INFANTRY == 1       //��Ħ
    
    Omega_Offset.X = 0;
    Omega_Offset.Y = 0;
    Omega_Offset.Z = 0;
    
    Euler_Offset.Pitch = 0.7;
    Euler_Offset.Roll = 0;
    Euler_Offset.Yaw = 0;
    
#elif INFANTRY == 2     //����˹��
    
    Omega_Offset.X = -0.67;
    Omega_Offset.Y = -0.48;
    Omega_Offset.Z = 0.48;
    
    Euler_Offset.Pitch = 0;
    Euler_Offset.Roll = 0;
    Euler_Offset.Yaw = 0;
    
#elif INFANTRY == 3     //��ʿ��
    
    Omega_Offset.X = 0;
    Omega_Offset.Y = 0;
    Omega_Offset.Z = 0;
    
    Euler_Offset.Pitch = 0;
    Euler_Offset.Roll = 0;
    Euler_Offset.Yaw = 0;
    
#elif INFANTRY == 4     //������û���֣�����ô�аɣ�
    
    Omega_Offset.X = 0;
    Omega_Offset.Y = 0;
    Omega_Offset.Z = 0;
    
    Euler_Offset.Pitch = 0;
    Euler_Offset.Roll = 0;
    Euler_Offset.Yaw = 0;
    
#elif INFANTRY == 5     //����
    
    Omega_Offset.X = 0;
    Omega_Offset.Y = 0;
    Omega_Offset.Z = 0;
    
    Euler_Offset.Pitch = 0;
    Euler_Offset.Roll = 0;
    Euler_Offset.Yaw = 0;
    
#elif INFANTRY == 6     //Ӣ��
    
    Omega_Offset.X = 0;
    Omega_Offset.Y = 0;
    Omega_Offset.Z = 0;
    
    Euler_Offset.Pitch = 0;
    Euler_Offset.Roll = 0;
    Euler_Offset.Yaw = 0;
    
#elif INFANTRY == 7     //����
    
    Omega_Offset.X = 0;
    Omega_Offset.Y = 0;
    Omega_Offset.Z = 0;
    
    Euler_Offset.Pitch = -4;
    Euler_Offset.Roll = 0;
    Euler_Offset.Yaw = 0;
    
#endif
}


/**
  * @brief  mpu9250����ŷ���Ǽ����ٶ�
  * @param  void
  * @retval 0�ɹ�     1ʧ��
  */
uint8_t MPU9250_Update(void)
{
    uint8_t result;
    
    float Pitch, Roll, Yaw;
    float OX, OY, OZ;
    float GX, GY, GZ;
    
    result = MPU9250DMP_GetEuler(&Pitch, &Roll, &Yaw);
    if(result)return 1;
    result = MPU9250_GetGyro(&OX, &OY, &OZ);
    if(result)return 1;
    result = MPU9250_GetAccel(&GX, &GY, &GZ);
    if(result)return 1;
    
    Position.Euler.Pitch = Pitch - Euler_Offset.Pitch;
    Position.Euler.Roll = Roll - Euler_Offset.Roll;
    Position.Euler.Yaw = Yaw - Euler_Offset.Yaw;
    
    Position.Real.OX = -OX - Omega_Offset.X;
    Position.Real.OY = -OY - Omega_Offset.Y;
    Position.Real.OZ = -OZ - Omega_Offset.Z;
    
    Position.Real.GX = GX;
    Position.Real.GY = GY;
    Position.Real.GZ = GZ;
    
    return 0;
}

