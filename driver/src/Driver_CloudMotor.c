
#define __CLOUDMOTOR_GLOBALS

#include "Config.h"
#include "Task_CANSend.h"
#include "Driver_mpu9250.h"
#include "Driver_Control.h"
#include "Driver_CloudMotor.h"
#include "Driver_SuperGyroscope.h"

#include "Handler.h"


/**
  * @note   modified
  * @brief  �����ʼ��
  * @param  void
  * @retval void
  */
void CloudMotor_InitConfig(void)
{
    CloudParam.Pitch.FrameCounter = 0;
    CloudParam.Pitch.FrameRate = 0;
    CloudParam.Pitch.RealABSAngle = 0;
    CloudParam.Pitch.RealEncoderAngle = PitchEncoderCenter;
    CloudParam.Pitch.TargetABSAngle = 0;
    
    CloudParam.Yaw.FrameCounter = 0;
    CloudParam.Yaw.FrameRate = 0;
    CloudParam.Yaw.RealABSAngle = 0;
    CloudParam.Yaw.RealEncoderAngle = YawEncoderCenter;
    CloudParam.Yaw.TargetABSAngle = 0;
    
    CloudParam.Lock = 1;
}
    
    
    
/**
  * @brief  ��̨�����������
  * @param  pitch�������yaw�����
  * @retval void
  */
void CloudMotorCurrent(int16_t Pitch, int16_t Yaw)
{
    static CanSend_Type   SendData;
    
#if CANPORT == 1
    SendData.CANx = 1;
#else
    SendData.CANx = 2;
#endif
    
    SendData.SendCanTxMsg.DLC   =   8;
    SendData.SendCanTxMsg.IDE   =   CAN_ID_STD;
    SendData.SendCanTxMsg.RTR   =   CAN_RTR_Data;
    SendData.SendCanTxMsg.StdId =   COULDCONTROLID;
    
#if YAWMOTORCURRENTPLUSDIR == 1
    SendData.SendCanTxMsg.Data[0] = Yaw >> 8;
    SendData.SendCanTxMsg.Data[1] = Yaw;
#else
    SendData.SendCanTxMsg.Data[0] = (-Yaw) >> 8;
    SendData.SendCanTxMsg.Data[1] = (-Yaw);
#endif
    
#if PITCHMOTORCURRENTPLUSDIR == 1
    SendData.SendCanTxMsg.Data[2] = Pitch >> 8;
    SendData.SendCanTxMsg.Data[3] = Pitch;
#else
    SendData.SendCanTxMsg.Data[2] = (-Pitch) >> 8;
    SendData.SendCanTxMsg.Data[3] = (-Pitch);
#endif

    xQueueSend(Queue_CANSend, &SendData, 10);
}


/**
  * @brief  ������ں���
  * @Parma  0 ���ͣ��      1 PID����
  * @retval void
  * @note   �˺���Ӧ��������ִ��
  */
void Cloud_Adjust(uint8_t mode)
{
    int16_t PitchMotorCurrent, YawMotorCurrent;
    
    //δ����
    if(mode)
    {
        PitchMotorCurrent = Control_PitchPID();
        YawMotorCurrent = Control_YawPID();
    }
    //����
    else
    {
        PitchMotorCurrent = 0;
        YawMotorCurrent = 0;
    }
    
    //��̨�����������
    CloudMotorCurrent(PitchMotorCurrent, YawMotorCurrent);
}


/**
  * @brief  �Զ����������ں���
  * @Parma  0 ���ͣ��      1 PID����
  * @retval void
  * @note   �˺���Ӧ��������ִ��
  */
void Cloud_AutoAdjust(float FeedSpeed, uint8_t mode)
{
    int16_t PitchMotorCurrent, YawMotorCurrent;
    
    //δ����
    if(mode)
    {
        PitchMotorCurrent = Control_PitchPID();
        YawMotorCurrent = Control_FeedForwardYawPID(FeedSpeed);
    }
    //����
    else
    {
        PitchMotorCurrent = 0;
        YawMotorCurrent = 0;
    }
    
    //��̨�����������
    CloudMotorCurrent(PitchMotorCurrent, YawMotorCurrent);
}


/**
  * @note   modified
  * @brief  ��̨Yaw�Ƕ�����
  * @param  Ŀ��Ƕ�
  * @param  ģʽ
  * @retval void

  */
void Cloud_YawAngleSet(float Target, AngleMode_Enum mode)
{
    float LimitBuffer;
    
    if(mode == AngleMode_OPP)
    {
        Target += SuperGyoParam.Angle;
    }
    
    LimitBuffer = SuperGyoParam.Angle + (YawEncoderLEFTLimit - YawEncoderCenter) * 0.043945F;
    Target = Target > LimitBuffer ? LimitBuffer : Target;
    LimitBuffer = SuperGyoParam.Angle - (YawEncoderCenter - YawEncoderRIGHTLimit) * 0.043945F;
    Target = Target < LimitBuffer ? LimitBuffer : Target;
    
    CloudParam.Yaw.TargetABSAngle = Target;
}


/**
  * @note   modified
  * @brief  ��̨Pitch�Ƕȼ�ģʽ����
  * @param  Ŀ��Ƕ�(���ԽǶȣ�
  * @retval void
  * @note   Pitchһֱʹ�þ��ԽǶȿ���
  */
void Cloud_PitchAngleSet(float Target)
{
    float MachineABSLimit, MixedLimit;
    
    MachineABSLimit = Position.Euler.Pitch + ((int16_t)PitchEncoderUPLimit - CloudParam.Pitch.RealEncoderAngle) * 0.043945F;
    MixedLimit = MachineABSLimit > PitchABSUPLimit ? PitchABSUPLimit : MachineABSLimit;
    Target = Target > MixedLimit ? MixedLimit : Target;
    
    MachineABSLimit = Position.Euler.Pitch + (PitchEncoderDOWNLimit - CloudParam.Pitch.RealEncoderAngle) * 0.043945F;
    MixedLimit = MachineABSLimit > PitchABSDOWNLimit ? MachineABSLimit : PitchABSDOWNLimit;
    Target = Target < MixedLimit ? MixedLimit : Target;
    
    CloudParam.Pitch.TargetABSAngle = Target;
}






