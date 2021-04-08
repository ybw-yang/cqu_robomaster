#include "main.h"
#include "OSinclude.h"


//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

/*-----------------------------------------------------------------------*/

#include "main.h"

const float YAW_MAXLIMIT = 274+53;                //  ����    ��װ�ú���Ҫ��������PITCH_MAXLIMIT �� PITCH_MINLIMIT	�������������Զ����㲢���� 104
const float YAW_MINLIMIT = 274-53;                //  �Ҽ���
const float YAW_CENTER   =   (YAW_MAXLIMIT+YAW_MINLIMIT) /2;                   //  ���ĵ� 

const int PITCH_MAXLIMIT  =	119+40;            //��ߵ�			��װ�ú���Ҫ��������PITCH_MAXLIMIT �� PITCH_MINLIMIT	�������������Զ����㲢���� 210
const int PITCH_MINLIMIT  = 119-40;   //��͵�
const int PITCH_CENTER	= (PITCH_MAXLIMIT+PITCH_MINLIMIT)/2;

const int PITCH_ESC_MAX = 6000;		//ԭֵΪ5000   20170117
const int YAW_ESC_MAX =	6000;
const int DAIL_ESC_MAX = 80000;
const int Track_Yaw_MAX = 6000;
const int Track_Pitch_MAX = 6000;

const int RADIO_DATA_MAX = 230;		//�����ٶ���ֵ
const int RADIO_DATA_MIN = 80;
/*------------------------------PID���Ʋ���---------------------------------------*/  
/*���̸���pid*/
const float UNDERPAN_FOLLOW_P = 10;
const float UNDERPAN_FOLLOW_I	= 0.004;
const float UNDERPAN_FOLLOW_D	=100;
/*yaw���pid*/

const float YAW_P = 120;//150
const float YAW_I	= 0.5;//0.7
const float YAW_D	= 1100;//1100

const float YAW_SPEED_P = 2;
const float YAW_SPEED_I  = 0.002;
const float YAW_SPEED_D  = 3;
/*pitch��pid*/   
const float PITCH_P = 120;//150;
const float PITCH_I  = 0.7;	//0.7
const float PITCH_D  = 500;	//1200

const float PITCH_SPEED_P = 1.5;
const float PITCH_SPEED_I = 0.001;
const float PITCH_SPEED_D = 1;
/*������ֱPID*/
const float GO_P = 13;
const float GO_I = 0.01;
const float GO_D = 100;

/*ͼ�����PID*/
const float Track_Yaw_P = 0.4;//0.4;
const float Track_Yaw_I = 0.01;//0.3;
const float Track_Yaw_D = 3;//3;

const float Track_Pitch_P = 0.4;
const float Track_Pitch_I = 0.03;
const float Track_Pitch_D = 2;

//-----������������--------//
int	const	DISTANCE = 500;

/*����λ��PID*/
const float DAIL_P=2;
const float DAIL_I=0.1;
const float DAIL_D=1;
const int dial_speed = 6000;//�����ٶ��趨ֵ
/*������*/
const float remote_sensitivity_x = 4;  //ң���������ȣ���ֵԽ��Խ����
const float remote_sensitivity_y = 8;  //ң���������ȣ���ֵԽ��Խ����

const float mouse_sensitivity_x = 4;	 //��������ȣ���ֵԽ��Խ����
const float mouse_sensitivity_y = 16;	 //��������ȣ���ֵԽ��Խ����
/*-----------------------------------*/
int Graph_flag = 0;
u8 Buff_flag = 0;
int Buff_End=0x41;

int main(void)

{
		ctrl_Init();
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
		while(1);
}
