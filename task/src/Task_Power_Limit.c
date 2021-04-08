#include "Task_Power_Limit.h"
//功率环参数配置	开始
//版本1.2version  
//作者：warpgate
//功能：动态的功率分配
//功率分配参考
//修改时间20170417
//版本修改说明：
//	添加了稳步启动和稳步制动的代码
//	将power_limit（）函数分开书写
//0418 键盘按键改为上升沿和下降沿触发 上升沿完成启动过程 下降沿完成制动过程 两个过程可以互相抢断
//		电流 与keyTimeOfUnderpan之间的函数关系 改动为一次函数 电流上线值沿直线变化
//0429改进功率函数 
//0502修改了pid。c中的134行和power_limit。c
//0510以炖为蓝本修改了 小鸡的功率限制的参数
//----------------------------------------
//P(W)practical 		|			P(W)definition
//----------------------------------------
//75								|80
//----------------------------------------
//69								|75
//----------------------------------------
//63								|70
//----------------------------------------
//56								|65
//----------------------------------------

//-----------------------------------------------------------------
//策略1：采用速度触发，任意轮转速达到TRIGGER_SPEED 限制功率上限
//				具体措施：1、在power_limit中不调用speed_value_is_zero 
//									2、TRIGGER_SPEED 不为0
//策略2：不采用速度触发，任意轮转速在所有转速情况下都被限制功率上限
//				具体措施:	1、power_limit调用speed_value_is_zero
//									2、TRIGGER_SPEED=0；
//-----------------------------------------------------------------

float POWER_UPLIMIT 					=	70.0;						//底盘限制功率（w） 稳定运行功率上限  给定78。0w对应实际功率浮动上限会稳定在80w左右左右 实际对应关系待测！
float POWER_T_I								= 0.0202;					//转矩常数（N*m/A）		每安培产生0.0202牛*米的扭矩
float DECAY 									= 1.0;								//衰减因子
short TRIGGER_SPEED						= 1;							//触发启动功率限制的速度	2000
u16 PIDPWRMAX 								= 14000;							//pid.c 中的pidPanMax 应该保持一致		待修改		//
u16 CURRENT_OUTPUTLM					=	10000;							//整个功率限制最后一个环节电流限制的上限		热身赛4000


float pwrUpLimit							=	0;
u16 pidPwrMax									=	0;								//随着速度增大逐渐变化的pid.current 输出上限
extern u16 keyTimeOfUnderpan;

under_PID pid;
//#define POWER_COEFFICIENT (9550.0*POWER_UPLIMIT)/POWER_T_I			//方便计算的一个系数  																															//满足：（电流*速度）<=（POWER_COEFFICIEN
//功率环参数配置	结束

/*------------------------功率限制函数定义 开始--------------------------
功能：底盘总功率限制
参数：
返回：
----------------------------------------------------------------------*/
//求绝对值
float  myabs(float x)
{
	return x<0?	-x:x;
}

//功率限制函数
//此函数调用了其余的几个子函数，此函数在timer.c 的TIM4_IRQHandler()函数中被调用（218行）
//若要去除部分功能 只需在此函数中注释该部分即可，而部分变量也应该做相应的改动
	u8 wheel_number=0;
	float temp_p[4]={0};
	float temp_sum=0;
	u16 speedValueSum=0;
	float powerLimitSingleWheel[4]={0};	
void power_limit(void)
{	
	pid.current[0] = ChassisParam.LF.NeedCurrent;
	pid.current[1] = ChassisParam.RF.NeedCurrent;
	pid.current[2] = ChassisParam.RB.NeedCurrent;
	pid.current[3] = ChassisParam.LB.NeedCurrent;

	pwr_limit_proportion_compute();														//计算功率分配的权重
	pwr_limit_proportion_distribution();											//分配功率
	//speed_value_is_zero(); 																	当选用策略2 应该加入该函数
	//保证底盘稳步启动
	underpan_stable_start();																	//限定底盘启动和制动时候的加速度 实现启动制动平缓进行 但是会存在刹车滑行 和启动过慢的不利效果
	pid_current_output_limit();																//最终的	总的输出限幅
}

//计算功率限制的权重
//计算每个轮的功率权重
void 	pwr_limit_proportion_compute(void)
{
	temp_sum=0;
	temp_sum = myabs(wheel[0]*ChassisParam.LF.NeedCurrent)+
						 myabs(wheel[1]*ChassisParam.RF.NeedCurrent)+
						 myabs(wheel[2]*ChassisParam.RB.NeedCurrent)+
						 myabs(wheel[3]*ChassisParam.LB.NeedCurrent);			
	if(temp_sum==0)	return;																											//剔除分母为0的情况
	powerLimitSingleWheel[0] = myabs(wheel[0]*ChassisParam.LF.NeedCurrent)/temp_sum*POWER_UPLIMIT;
	powerLimitSingleWheel[1] = myabs(wheel[1]*ChassisParam.RF.NeedCurrent)/temp_sum*POWER_UPLIMIT;
	powerLimitSingleWheel[2] = myabs(wheel[2]*ChassisParam.RB.NeedCurrent)/temp_sum*POWER_UPLIMIT;
	powerLimitSingleWheel[3] = myabs(wheel[3]*ChassisParam.LB.NeedCurrent)/temp_sum*POWER_UPLIMIT;
}

	//power分配完成
	//单个轮power限制
	//对每一轮功率限制
void pwr_limit_proportion_distribution(void)
{
 
	for(wheel_number=0;wheel_number<4;wheel_number++)	
	{
		if (Real_Speed_Value[wheel_number]==0)	return;
		temp_p[wheel_number]=((9550.0f*powerLimitSingleWheel[wheel_number])/POWER_T_I)/Real_Speed_Value[wheel_number];
		if(Real_Speed_Value[wheel_number]>=TRIGGER_SPEED)
		{
			if(pid.current[wheel_number]>temp_p[wheel_number])	pid.current[wheel_number]=DECAY*temp_p[wheel_number];
			if(pid.current[wheel_number]<-temp_p[wheel_number])	pid.current[wheel_number]=-DECAY*temp_p[wheel_number];
		}
		if(Real_Speed_Value[wheel_number]<=-TRIGGER_SPEED)
		{
			if(pid.current[wheel_number]>-temp_p[wheel_number])	pid.current[wheel_number]=-DECAY*temp_p[wheel_number];
			if(pid.current[wheel_number]<temp_p[wheel_number])	pid.current[wheel_number]=DECAY*temp_p[wheel_number];
		}
	}
}

//最后一step，电流限幅函数! 
	void pid_current_output_limit(void)
	{
		for(wheel_number=0;wheel_number<4;wheel_number++)	
		{
			if(pid.current[wheel_number]>CURRENT_OUTPUTLM)	pid.current[wheel_number]=CURRENT_OUTPUTLM;
			if(pid.current[wheel_number]<-CURRENT_OUTPUTLM)	pid.current[wheel_number]=-CURRENT_OUTPUTLM;		
		}	
	}

//考虑轮子速度为0的情况
	void speed_value_is_zero(void )
	{		
		for(wheel_number=0;wheel_number<4;wheel_number++)	
		{
			if(Real_Speed_Value[wheel_number]==0)
			{
			temp_p[wheel_number]=((9550.0f*powerLimitSingleWheel[wheel_number])/POWER_T_I)*100;			//100这个数值可以自己按需设置
			pid.current[wheel_number]=DECAY*temp_p[wheel_number];		
			}
		}
	}
	//底盘稳步启动函数 
	//原理：随着车速的提升 对功率的限制逐渐释放 最终到达最终功率
	//			功率的上限值随着底盘速度正比例变化
	u16	T1	=	15;								//在decode中也有用到			
	u16	A1	=	200;				//当限定了b1 t1 t2 和 点(t1,2500) 此值将被唯一确定
	#define	B1	CURRENT_OUTPUTLM-T1*A1;
	
//	static u8	t2	=	100;
	/*
	电流限幅上限值（加速度+加速度常熟）与时间关系曲线
	^pidPwrMax
	|
	|				(T1,CURRENT_OUTPUTLM)
	|			 /
	|			/	 
	|		 /		
	|		/			 
	|	 /				
	|	/					 
	|/(0,B1)
	|
	|——————————————>t
	三段区间的方程：
	1:	pidPwrMax=a1*t+b1					(0,t1)
	2:	pidPwrMax=-a1*t+t2*a1+b1	(t1,t2)
	3:	pidPwrMax=4000						(t2,...)
	
	0418改动为图像所示曲线
	*/
	void underpan_stable_start(void)
	{	

		if(keyTimeOfUnderpan	<=	T1)		//加速度电流时间曲线
		{
			pidPwrMax=A1*keyTimeOfUnderpan+B1;			//keyTimeOfUnderpan  并不是完全对应于时间 当制动的时候 这个变量是在递减的
		}
		//键盘按键从按下起计时到当前时刻所用的时间依照上述函数所对应的电流限幅量pidPwrMax
				for(wheel_number=0;wheel_number<4;wheel_number++)	
		{
			if(pid.current[wheel_number]>pidPwrMax)	pid.current[wheel_number]=pidPwrMax;
			if(pid.current[wheel_number]<-pidPwrMax)	pid.current[wheel_number]=-pidPwrMax;		
		}
	}
		
	
/*------------------------功率限制函数定义 结束--------------------------*/
