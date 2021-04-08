#include "Task_Power_Limit.h"
//���ʻ���������	��ʼ
//�汾1.2version  
//���ߣ�warpgate
//���ܣ���̬�Ĺ��ʷ���
//���ʷ���ο�
//�޸�ʱ��20170417
//�汾�޸�˵����
//	������Ȳ��������Ȳ��ƶ��Ĵ���
//	��power_limit���������ֿ���д
//0418 ���̰�����Ϊ�����غ��½��ش��� ����������������� �½�������ƶ����� �������̿��Ի�������
//		���� ��keyTimeOfUnderpan֮��ĺ�����ϵ �Ķ�Ϊһ�κ��� ��������ֵ��ֱ�߱仯
//0429�Ľ����ʺ��� 
//0502�޸���pid��c�е�134�к�power_limit��c
//0510����Ϊ�����޸��� С���Ĺ������ƵĲ���
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
//����1�������ٶȴ�����������ת�ٴﵽTRIGGER_SPEED ���ƹ�������
//				�����ʩ��1����power_limit�в�����speed_value_is_zero 
//									2��TRIGGER_SPEED ��Ϊ0
//����2���������ٶȴ�����������ת��������ת������¶������ƹ�������
//				�����ʩ:	1��power_limit����speed_value_is_zero
//									2��TRIGGER_SPEED=0��
//-----------------------------------------------------------------

float POWER_UPLIMIT 					=	70.0;						//�������ƹ��ʣ�w�� �ȶ����й�������  ����78��0w��Ӧʵ�ʹ��ʸ������޻��ȶ���80w�������� ʵ�ʶ�Ӧ��ϵ���⣡
float POWER_T_I								= 0.0202;					//ת�س�����N*m/A��		ÿ�������0.0202ţ*�׵�Ť��
float DECAY 									= 1.0;								//˥������
short TRIGGER_SPEED						= 1;							//���������������Ƶ��ٶ�	2000
u16 PIDPWRMAX 								= 14000;							//pid.c �е�pidPanMax Ӧ�ñ���һ��		���޸�		//
u16 CURRENT_OUTPUTLM					=	10000;							//���������������һ�����ڵ������Ƶ�����		������4000


float pwrUpLimit							=	0;
u16 pidPwrMax									=	0;								//�����ٶ������𽥱仯��pid.current �������
extern u16 keyTimeOfUnderpan;

under_PID pid;
//#define POWER_COEFFICIENT (9550.0*POWER_UPLIMIT)/POWER_T_I			//��������һ��ϵ��  																															//���㣺������*�ٶȣ�<=��POWER_COEFFICIEN
//���ʻ���������	����

/*------------------------�������ƺ������� ��ʼ--------------------------
���ܣ������ܹ�������
������
���أ�
----------------------------------------------------------------------*/
//�����ֵ
float  myabs(float x)
{
	return x<0?	-x:x;
}

//�������ƺ���
//�˺�������������ļ����Ӻ������˺�����timer.c ��TIM4_IRQHandler()�����б����ã�218�У�
//��Ҫȥ�����ֹ��� ֻ���ڴ˺�����ע�͸ò��ּ��ɣ������ֱ���ҲӦ������Ӧ�ĸĶ�
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

	pwr_limit_proportion_compute();														//���㹦�ʷ����Ȩ��
	pwr_limit_proportion_distribution();											//���书��
	//speed_value_is_zero(); 																	��ѡ�ò���2 Ӧ�ü���ú���
	//��֤�����Ȳ�����
	underpan_stable_start();																	//�޶������������ƶ�ʱ��ļ��ٶ� ʵ�������ƶ�ƽ������ ���ǻ����ɲ������ �����������Ĳ���Ч��
	pid_current_output_limit();																//���յ�	�ܵ�����޷�
}

//���㹦�����Ƶ�Ȩ��
//����ÿ���ֵĹ���Ȩ��
void 	pwr_limit_proportion_compute(void)
{
	temp_sum=0;
	temp_sum = myabs(wheel[0]*ChassisParam.LF.NeedCurrent)+
						 myabs(wheel[1]*ChassisParam.RF.NeedCurrent)+
						 myabs(wheel[2]*ChassisParam.RB.NeedCurrent)+
						 myabs(wheel[3]*ChassisParam.LB.NeedCurrent);			
	if(temp_sum==0)	return;																											//�޳���ĸΪ0�����
	powerLimitSingleWheel[0] = myabs(wheel[0]*ChassisParam.LF.NeedCurrent)/temp_sum*POWER_UPLIMIT;
	powerLimitSingleWheel[1] = myabs(wheel[1]*ChassisParam.RF.NeedCurrent)/temp_sum*POWER_UPLIMIT;
	powerLimitSingleWheel[2] = myabs(wheel[2]*ChassisParam.RB.NeedCurrent)/temp_sum*POWER_UPLIMIT;
	powerLimitSingleWheel[3] = myabs(wheel[3]*ChassisParam.LB.NeedCurrent)/temp_sum*POWER_UPLIMIT;
}

	//power�������
	//������power����
	//��ÿһ�ֹ�������
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

//���һstep�������޷�����! 
	void pid_current_output_limit(void)
	{
		for(wheel_number=0;wheel_number<4;wheel_number++)	
		{
			if(pid.current[wheel_number]>CURRENT_OUTPUTLM)	pid.current[wheel_number]=CURRENT_OUTPUTLM;
			if(pid.current[wheel_number]<-CURRENT_OUTPUTLM)	pid.current[wheel_number]=-CURRENT_OUTPUTLM;		
		}	
	}

//���������ٶ�Ϊ0�����
	void speed_value_is_zero(void )
	{		
		for(wheel_number=0;wheel_number<4;wheel_number++)	
		{
			if(Real_Speed_Value[wheel_number]==0)
			{
			temp_p[wheel_number]=((9550.0f*powerLimitSingleWheel[wheel_number])/POWER_T_I)*100;			//100�����ֵ�����Լ���������
			pid.current[wheel_number]=DECAY*temp_p[wheel_number];		
			}
		}
	}
	//�����Ȳ��������� 
	//ԭ�����ų��ٵ����� �Թ��ʵ��������ͷ� ���յ������չ���
	//			���ʵ�����ֵ���ŵ����ٶ��������仯
	u16	T1	=	15;								//��decode��Ҳ���õ�			
	u16	A1	=	200;				//���޶���b1 t1 t2 �� ��(t1,2500) ��ֵ����Ψһȷ��
	#define	B1	CURRENT_OUTPUTLM-T1*A1;
	
//	static u8	t2	=	100;
	/*
	�����޷�����ֵ�����ٶ�+���ٶȳ��죩��ʱ���ϵ����
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
	|����������������������������>t
	��������ķ��̣�
	1:	pidPwrMax=a1*t+b1					(0,t1)
	2:	pidPwrMax=-a1*t+t2*a1+b1	(t1,t2)
	3:	pidPwrMax=4000						(t2,...)
	
	0418�Ķ�Ϊͼ����ʾ����
	*/
	void underpan_stable_start(void)
	{	

		if(keyTimeOfUnderpan	<=	T1)		//���ٶȵ���ʱ������
		{
			pidPwrMax=A1*keyTimeOfUnderpan+B1;			//keyTimeOfUnderpan  ��������ȫ��Ӧ��ʱ�� ���ƶ���ʱ�� ����������ڵݼ���
		}
		//���̰����Ӱ������ʱ����ǰʱ�����õ�ʱ������������������Ӧ�ĵ����޷���pidPwrMax
				for(wheel_number=0;wheel_number<4;wheel_number++)	
		{
			if(pid.current[wheel_number]>pidPwrMax)	pid.current[wheel_number]=pidPwrMax;
			if(pid.current[wheel_number]<-pidPwrMax)	pid.current[wheel_number]=-pidPwrMax;		
		}
	}
		
	
/*------------------------�������ƺ������� ����--------------------------*/
