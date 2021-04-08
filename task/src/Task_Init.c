#include "main.h"
//#include "mpu6500.h"

const int Remote_Ctrl  = 1320;        //ң����ͨ����ֵ 1648-364 
const int Remote_Ctrl_mouse  = 12000;  //���ͨ����
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
������  ��ctrl_Init����
�������� :
����    ����̨���ƺ���
����    ��2016.12.30
����    ���쿣��ģ�
-----------------------------------------------------------------------------------------------------------------------*/
void ctrl_Init(void)
{	
	BSP_GPIO_InitConfig();

	BSP_TIM3_InitConfig();		//��������,�����ʼ����ʼ
	BSP_TIM12_InitConfig();		//����
	BSP_TIM4_InitConfig();		//Ħ����
	BSP_TIM8_InitConfig();		//������������벶��
	BSP_TIM2_InitConfig();		//������������벶��
	BSP_TIM7_InitConfig();		//�����������ź�

	BSP_CAN_InitConfig();
	BSP_UART6_InitConfig(115200);							//����mpu9250���ݴ���
	BSP_UART3_InitConfig(115200);							//����ͼ�����ݴ���

	BSP_DMA_InitConfig();
	BSP_NVIC_InitConfig();
	Ultrasonic_Init();					//�������ṹ���ʼ��
	/*----------------------��̨��ʼ��------------------*/ 
	//---------------------MPU6500��ʼ��-----------------
	while(!MPU6500_Init());							//�ȴ�MPU6500��ʼ���ɹ�
	IST8310_Init();         //�ش�оƬ����
	AHRS_Date_Init();
	MPU6500_Date_Offset(5000);
	param_Init();   										//������ʼ��
  //---------------------------------------------------									
	
	
}

/*-------------------------------------------------------------------------------------------------------------------
������  ��param_Init����
�������� :
����    ����̨�ز�����ʼ������
����    ��2016.12.30
����    ���쿣��ģ�
-----------------------------------------------------------------------------------------------------------------------*/
void param_Init(void )
{	
	/*  yaw �� pitch �����Ƕȷ�Χ  */
	YAW_ANGLE_DIFFERENCE 	= YAW_MAXLIMIT  	- YAW_MINLIMIT;
	PITCH_ANGLE_DIFFERENCE  = PITCH_MAXLIMIT   	- PITCH_MINLIMIT;
	//yaw����ң�����ı���ֵ   ������  �ֶ�ֵ �ֱ���
	yaw_LSB 				= YAW_ANGLE_DIFFERENCE / Remote_Ctrl;
	//yaw�������ı���ֵ		
	yaw_LSB_mouse=YAW_ANGLE_DIFFERENCE/Remote_Ctrl_mouse;
	//
	pitch_LSB   = PITCH_ANGLE_DIFFERENCE / Remote_Ctrl;                  //������С�ֶȵ�  
	//
	pitch_LSB_mouse=PITCH_ANGLE_DIFFERENCE/Remote_Ctrl_mouse;//	100/12000
	
	TIM_SetCompare1(TIM3,0);						//�����ʼ����ɣ��������ر�
}
