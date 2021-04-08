#include "main.h"
//#include "mpu6500.h"

const int Remote_Ctrl  = 1320;        //遥控器通道差值 1648-364 
const int Remote_Ctrl_mouse  = 12000;  //鼠标通道差
const int MOUSE_OUTPUT_MAXLIMI = (Remote_Ctrl_mouse/2);
const int YMOUSE_OUTPUT_MAXLIMIP = (Remote_Ctrl_mouse/2);
const int YMOUSE_OUTPUT_MAXLIMIN = (Remote_Ctrl_mouse/2);

float YAW_ANGLE_DIFFERENCE;     
float PITCH_ANGLE_DIFFERENCE;
float yaw_LSB;
float yaw_LSB_mouse;
float pitch_LSB;
float pitch_LSB_mouse;

u8 	  whole_init = 1;

/*-------------------------------------------------------------------------------------------------------------------
函数名  ：ctrl_Init（）
函数变量 :
功能    ：云台控制函数
日期    ：2016.12.30
作者    ：杨博炜（改）
-----------------------------------------------------------------------------------------------------------------------*/
void ctrl_Init(void)
{	
	BSP_GPIO_InitConfig();

	BSP_TIM3_InitConfig();		//蜂鸣器响,程序初始化开始
	BSP_TIM12_InitConfig();		//波轮
	BSP_TIM4_InitConfig();		//摩擦轮
	BSP_TIM8_InitConfig();		//超声波测距输入捕获
	BSP_TIM2_InitConfig();		//超声波测距输入捕获
	BSP_TIM7_InitConfig();		//超声波触发信号

	BSP_CAN_InitConfig();
	BSP_UART6_InitConfig(115200);							//用于mpu9250数据传输
	BSP_UART3_InitConfig(115200);							//用于图像数据传输

	BSP_DMA_InitConfig();
	BSP_NVIC_InitConfig();
	Ultrasonic_Init();					//超声波结构体初始化
	/*----------------------云台初始化------------------*/ 
	//---------------------MPU6500初始化-----------------
	while(!MPU6500_Init());							//等待MPU6500初始化成功
	IST8310_Init();         //地磁芯片配置
	AHRS_Date_Init();
	MPU6500_Date_Offset(5000);
	param_Init();   										//变量初始化
  //---------------------------------------------------									
	
	
}

/*-------------------------------------------------------------------------------------------------------------------
函数名  ：param_Init（）
函数变量 :
功能    ：云台控参数初始化函数
日期    ：2016.12.30
作者    ：杨博炜（改）
-----------------------------------------------------------------------------------------------------------------------*/
void param_Init(void )
{	
	/*  yaw 和 pitch 的最大角度范围  */
	YAW_ANGLE_DIFFERENCE 	= YAW_MAXLIMIT  	- YAW_MINLIMIT;
	PITCH_ANGLE_DIFFERENCE  = PITCH_MAXLIMIT   	- PITCH_MINLIMIT;
	//yaw方向遥控器的比例值   灵敏度  分度值 分辨率
	yaw_LSB 				= YAW_ANGLE_DIFFERENCE / Remote_Ctrl;
	//yaw方向鼠标的比例值		
	yaw_LSB_mouse=YAW_ANGLE_DIFFERENCE/Remote_Ctrl_mouse;
	//
	pitch_LSB   = PITCH_ANGLE_DIFFERENCE / Remote_Ctrl;                  //计算最小分度的  
	//
	pitch_LSB_mouse=PITCH_ANGLE_DIFFERENCE/Remote_Ctrl_mouse;//	100/12000
	
	TIM_SetCompare1(TIM3,0);						//程序初始化完成，蜂鸣器关闭
}
