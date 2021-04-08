
#include "Driver_Steering.h"



/**
  * @brief  �����ʼ��
  * @param  void
  * @retval void
  */
void Steering_InitConfig(void)
{
    TIM8->CCR4 = 0;      //�رղ���
}


/**
  * @brief  ����Ƕȿ���
  * @param  0 �رղ���          1 �򿪲���      2ʧ��
  * @retval void 
  */
void Steering_Control(uint8_t status)
{
    if(status == 1)
    {
        TIM8->CCR4 = 800;
    }
    else if(status == 0)
    {
        TIM8->CCR4 = 2200;
    }
    else if(status == 2)
    {
        TIM8->CCR4 = 0;
    }
}








