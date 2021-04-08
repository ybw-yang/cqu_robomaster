#include "main.h"
#include "OSinclude.h"

const float MaxWheelSpeed = 6000;             //���̵��������ٶ�����
const float ChassisMaxSumCurrent = 10000.0F;  //���̵�������

ChassisParam_Struct ChassisParam;

/*-----------------------------------------------�����趨����----------------------------------------------------------*/

/**
  * @brief  ���̳�ʼ��
  * @param  void 
  * @retval void
  */
void Chassis_InitConfig(void)
{
    
    ChassisParam.TargetVX = 0;
    ChassisParam.TargetVY = 0;
    ChassisParam.TargetOmega = 0;
    ChassisParam.TargetABSAngle = 0;
    ChassisParam.SpeedLevel = ChassisSpeedLevel_Hight;
    
}

/**
  * @brief  XY�����ٶ�����
  * @param  X�ٶ�
  * @param  Y�ٶ�
  * @retval void
  */
void Chassis_SpeedSet(float XSpeed, float YSpeed)
{
    XSpeed = XSpeed > MaxWheelSpeed? MaxWheelSpeed : XSpeed;
    XSpeed = XSpeed < -MaxWheelSpeed ? -MaxWheelSpeed : XSpeed;
    
    YSpeed = YSpeed > MaxWheelSpeed ? MaxWheelSpeed : YSpeed;
    YSpeed = YSpeed < -MaxWheelSpeed ? -MaxWheelSpeed : YSpeed;
    
    ChassisParam.TargetVX = XSpeed;
    ChassisParam.TargetVY = YSpeed;
}

/**
  * @brief  ����Ŀ��Ƕ�����
  * @param  Ŀ��Ƕ�
  * @retval void
  */
void Chassis_TargetDirectionSet(float Target)
{
    ChassisParam.TargetABSAngle = Target;
}


/*--------------------------------------------------�������㺯��-------------------------------------------------------------*/

/**
  * @brief  ���ֽ���
  * @param  x�ٶȣ�ǰ��
  * @param  y�ٶȣ��ң�
  * @param  ���ٶȣ�˳ʱ��Ϊ����
  * @retval void
  */
static void MecanumCalculate(float Vx, float Vy, float Omega, int16_t *Speed)
{
    float Buffer[4], Param, MaxSpeed;
    uint8_t index;
    
    Buffer[0] = +Vx + Vy - Omega;
    Buffer[1] = -Vx + Vy - Omega;
    Buffer[2] = -Vx - Vy - Omega;
    Buffer[3] = +Vx - Vy - Omega;
    
    //�޷�
    for(index = 0, MaxSpeed = 0; index < 4; index++)
    {
        if((Buffer[index] > 0 ? Buffer[index] : -Buffer[index]) > MaxSpeed)
        {
            MaxSpeed = (Buffer[index] > 0 ? Buffer[index] : -Buffer[index]);
        }
    }
    if(MaxWheelSpeed < MaxSpeed)
    {
        Param = MaxWheelSpeed / MaxSpeed;
        Speed[0] = Buffer[0] * Param;
        Speed[1] = Buffer[1] * Param;
        Speed[2] = Buffer[2] * Param;
        Speed[3] = Buffer[3] * Param; 
    }
    else
    {
        Speed[0] = Buffer[0];
        Speed[1] = Buffer[1];
        Speed[2] = Buffer[2];
        Speed[3] = Buffer[3];
    }
}


/*---------------------------------------------------���ͺ���------------------------------------------------------------------*/

/**
  * @brief  ���͵��̿�������
  * @param  
  * @param  
  * @retval void
  */
void Chassis_SendMotorParam(void)
{
	  static CanSend_Type SendData;

 	  SendData.CANx=1;
    SendData.SendCanTxMsg.StdId = 0x200;
    SendData.SendCanTxMsg.IDE = CAN_Id_Standard;
    SendData.SendCanTxMsg.RTR = CAN_RTR_Data;
    SendData.SendCanTxMsg.DLC = 0x08;
	
	  SendData.SendCanTxMsg.Data[0] = (((int)(pid.current[0])>>8)&0xff);
		SendData.SendCanTxMsg.Data[1] = (((int)(pid.current[0]))&0xff);
		SendData.SendCanTxMsg.Data[2] = (((int)(pid.current[1])>>8)&0xff);
		SendData.SendCanTxMsg.Data[3] = (((int)(pid.current[1]))&0xff);
		SendData.SendCanTxMsg.Data[4] = (((int)(pid.current[2])>>8)&0xff);
		SendData.SendCanTxMsg.Data[5] = (((int)(pid.current[2]))&0xff);
		SendData.SendCanTxMsg.Data[6] = (((int)(pid.current[3])>>8)&0xff);
		SendData.SendCanTxMsg.Data[7] = (((int)(pid.current[3]))&0xff);
	
		xQueueSend(Queue_CANSend, &SendData, 10);
    	
}

/*---------------------------------------------------���̿��ƺ���------------------------------------------------------------------*/

/**
  * @brief  ���̵���
  * @param  1 ��������          0 ����ֹͣ 
  * @param  1 �����ͷ�          0 ��������  
  * @retval void
  */
void Chassis_Control(void)
{
    int16_t WheelSpeed[4];
    int16_t PowerSum;
    int16_t ABSSpeed[4];

	  //ң����Ϊ-660-660���ٶ�Ϊ0-8000����ң����*10��ʾĿ���ٶȡ�
	  Chassis_SpeedSet(radio_ahead_back_data*10,radio_left_right_data*10);
    Chassis_TargetDirectionSet(radio_turn_data*10);

    //���ֽ���
    MecanumCalculate(ChassisParam.TargetVX, ChassisParam.TargetVY, ChassisParam.TargetABSAngle, WheelSpeed);
    ChassisParam.LF.TargetSpeed = WheelSpeed[0];
    ChassisParam.RF.TargetSpeed = WheelSpeed[1];
    ChassisParam.RB.TargetSpeed = WheelSpeed[2];
    ChassisParam.LB.TargetSpeed = WheelSpeed[3];
	
	 //PID�������
		Chassis_PID_Control(); 
	  
    //3510�����Ҫ���ж�̬��������
    ABSSpeed[0] = (ChassisParam.LF.NeedCurrent > 0 ? ChassisParam.LF.NeedCurrent : -ChassisParam.LF.NeedCurrent);
    ABSSpeed[1] = (ChassisParam.RF.NeedCurrent > 0 ? ChassisParam.RF.NeedCurrent : -ChassisParam.RF.NeedCurrent);
    ABSSpeed[2] = (ChassisParam.RB.NeedCurrent > 0 ? ChassisParam.RB.NeedCurrent : -ChassisParam.RB.NeedCurrent);
    ABSSpeed[3] = (ChassisParam.LB.NeedCurrent > 0 ? ChassisParam.LB.NeedCurrent : -ChassisParam.LB.NeedCurrent);

    PowerSum = ABSSpeed[0] + ABSSpeed[1] + ABSSpeed[2] + ABSSpeed[3];

    if(PowerSum > 10000)
    {
        ChassisParam.LF.LimitCurrent = ChassisMaxSumCurrent * ChassisParam.LF.NeedCurrent / PowerSum;
        ChassisParam.RF.LimitCurrent = ChassisMaxSumCurrent * ChassisParam.RF.NeedCurrent / PowerSum;
        ChassisParam.RB.LimitCurrent = ChassisMaxSumCurrent * ChassisParam.RB.NeedCurrent / PowerSum;
        ChassisParam.LB.LimitCurrent = ChassisMaxSumCurrent * ChassisParam.LB.NeedCurrent / PowerSum;
    }
    else
    {
        ChassisParam.LF.LimitCurrent = ChassisParam.LF.NeedCurrent;
        ChassisParam.RF.LimitCurrent = ChassisParam.RF.NeedCurrent;
        ChassisParam.RB.LimitCurrent = ChassisParam.RB.NeedCurrent;
        ChassisParam.LB.LimitCurrent = ChassisParam.LB.NeedCurrent;
    }
			
    Chassis_SendMotorParam();

}



