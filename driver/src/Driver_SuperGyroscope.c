
#define __DRIVER_SUPERGYROSCOPE_GLOBALS

#include "Driver_SuperGyroscope.h"


/**
  * @brief  ���������ǳ�ʼ��
  * @param  void
  * @retval void
  */
void SuperGyo_InitConfig(void)
{
    SuperGyoFrameCounter = 0;
    SuperGyoParam.Angle = 0;
    SuperGyoParam.Omega = 0;
}



