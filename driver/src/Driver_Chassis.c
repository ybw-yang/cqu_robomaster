#include "main.h"
#include "OSinclude.h"

const float MaxWheelSpeed = 6000;             //底盘单个轮子速度限制
const float ChassisMaxSumCurrent = 10000.0F;  //底盘电流控制

ChassisParam_Struct ChassisParam;

/*-----------------------------------------------参数设定函数----------------------------------------------------------*/

/**
  * @brief  底盘初始化
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
  * @brief  XY方向速度设置
  * @param  X速度
  * @param  Y速度
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
  * @brief  底盘目标角度设置
  * @param  目标角度
  * @retval void
  */
void Chassis_TargetDirectionSet(float Target)
{
    ChassisParam.TargetABSAngle = Target;
}


/*--------------------------------------------------参数运算函数-------------------------------------------------------------*/

/**
  * @brief  麦轮解算
  * @param  x速度（前）
  * @param  y速度（右）
  * @param  角速度（顺时针为正）
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
    
    //限幅
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


/*---------------------------------------------------发送函数------------------------------------------------------------------*/

/**
  * @brief  发送底盘控制数据
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

/*---------------------------------------------------底盘控制函数------------------------------------------------------------------*/

/**
  * @brief  底盘调节
  * @param  1 正常控制          0 底盘停止 
  * @param  1 后轮释放          0 正常控制  
  * @retval void
  */
void Chassis_Control(void)
{
    int16_t WheelSpeed[4];
    int16_t PowerSum;
    int16_t ABSSpeed[4];

	  //遥控量为-660-660，速度为0-8000，用遥控量*10表示目标速度。
	  Chassis_SpeedSet(radio_ahead_back_data*10,radio_left_right_data*10);
    Chassis_TargetDirectionSet(radio_turn_data*10);

    //麦轮解算
    MecanumCalculate(ChassisParam.TargetVX, ChassisParam.TargetVY, ChassisParam.TargetABSAngle, WheelSpeed);
    ChassisParam.LF.TargetSpeed = WheelSpeed[0];
    ChassisParam.RF.TargetSpeed = WheelSpeed[1];
    ChassisParam.RB.TargetSpeed = WheelSpeed[2];
    ChassisParam.LB.TargetSpeed = WheelSpeed[3];
	
	 //PID控制输出
		Chassis_PID_Control(); 
	  
    //3510电机需要进行动态电流分配
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



