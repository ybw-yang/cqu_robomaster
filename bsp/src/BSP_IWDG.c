#include "main.h"

u8 sbus_warning;

//��ʼ���������Ź�
//prer:��Ƶ��:0~7(ֻ�е�3λ��Ч!)
//rlr:�Զ���װ��ֵ,0~0XFFF.
//��Ƶ����=4*2^prer.�����ֵֻ����256!
//rlr:��װ�ؼĴ���ֵ:��11λ��Ч.
//ʱ�����(���):Tout=((4*2^prer)*rlr)/32 (ms).
void IWDG_Init(u8 prer,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //ʹ�ܶ�IWDG->PR IWDG->RLR��д
	
	IWDG_SetPrescaler(prer); //����IWDG��Ƶϵ��

	IWDG_SetReload(rlr);   //����IWDGװ��ֵ

	IWDG_ReloadCounter(); //reload
	
	IWDG_Enable();       //ʹ�ܿ��Ź�
}

//ι�������Ź�
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();//reload
}

void IWDG_judge(void)
{
		static u8 t=0;
		static u32 sbus_num1=0;

/*---------------------�������Ź�-----------------------------*/
		t=t+1;
		if(t==200)
		{
			t=0;
			if(sbus_num1==sbus_num0)
			{
				sbus_warning=1;
				if(sbus_num1>=0xfffffff0) sbus_warning = 0;
			}
			else	
			{
				sbus_num1=sbus_num0;
				sbus_warning=0;
			}
		}
/*------------------------------------------------------------*/
}
