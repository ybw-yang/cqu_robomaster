#include "main.h"
#include "OSinclude.h"


//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

/*-----------------------------------------------------------------------*/

#include "main.h"

const float YAW_MAXLIMIT = 274+53;                //  左极限    安装好后需要重新修正PITCH_MAXLIMIT 和 PITCH_MINLIMIT	其余参数程序会自动计算并修正 104
const float YAW_MINLIMIT = 274-53;                //  右极限
const float YAW_CENTER   =   (YAW_MAXLIMIT+YAW_MINLIMIT) /2;                   //  中心点 

const int PITCH_MAXLIMIT  =	119+40;            //最高点			安装好后需要重新修正PITCH_MAXLIMIT 和 PITCH_MINLIMIT	其余参数程序会自动计算并修正 210
const int PITCH_MINLIMIT  = 119-40;   //最低点
const int PITCH_CENTER	= (PITCH_MAXLIMIT+PITCH_MINLIMIT)/2;

const int PITCH_ESC_MAX = 6000;		//原值为5000   20170117
const int YAW_ESC_MAX =	6000;
const int DAIL_ESC_MAX = 80000;
const int Track_Yaw_MAX = 6000;
const int Track_Pitch_MAX = 6000;

const int RADIO_DATA_MAX = 230;		//底盘速度最值
const int RADIO_DATA_MIN = 80;
/*------------------------------PID控制参数---------------------------------------*/  
/*地盘跟随pid*/
const float UNDERPAN_FOLLOW_P = 10;
const float UNDERPAN_FOLLOW_I	= 0.004;
const float UNDERPAN_FOLLOW_D	=100;
/*yaw轴的pid*/

const float YAW_P = 120;//150
const float YAW_I	= 0.5;//0.7
const float YAW_D	= 1100;//1100

const float YAW_SPEED_P = 2;
const float YAW_SPEED_I  = 0.002;
const float YAW_SPEED_D  = 3;
/*pitch的pid*/   
const float PITCH_P = 120;//150;
const float PITCH_I  = 0.7;	//0.7
const float PITCH_D  = 500;	//1200

const float PITCH_SPEED_P = 1.5;
const float PITCH_SPEED_I = 0.001;
const float PITCH_SPEED_D = 1;
/*底盘走直PID*/
const float GO_P = 13;
const float GO_I = 0.01;
const float GO_D = 100;

/*图像跟随PID*/
const float Track_Yaw_P = 0.4;//0.4;
const float Track_Yaw_I = 0.01;//0.3;
const float Track_Yaw_D = 3;//3;

const float Track_Pitch_P = 0.4;
const float Track_Pitch_I = 0.03;
const float Track_Pitch_D = 2;

//-----超声波检测距离--------//
int	const	DISTANCE = 500;

/*波轮位置PID*/
const float DAIL_P=2;
const float DAIL_I=0.1;
const float DAIL_D=1;
const int dial_speed = 6000;//波轮速度设定值
/*灵敏度*/
const float remote_sensitivity_x = 4;  //遥控器灵敏度，数值越低越灵敏
const float remote_sensitivity_y = 8;  //遥控器灵敏度，数值越低越灵敏

const float mouse_sensitivity_x = 4;	 //鼠标灵敏度，数值越高越灵敏
const float mouse_sensitivity_y = 16;	 //鼠标灵敏度，数值越高越灵敏
/*-----------------------------------*/
int Graph_flag = 0;
u8 Buff_flag = 0;
int Buff_End=0x41;

int main(void)

{
		ctrl_Init();
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
		while(1);
}
