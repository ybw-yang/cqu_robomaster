#include "main.h"
#include "OSinclude.h"

u8 PID_Calculation_flag;
/**
  * @brief  控制任务（周期5ms）
  * @param  ubused
  * @retval void
  */
void Task_Control(void *Parameters)  //底盘控制函数
{
    portTickType CurrentControlTick = 0;                //当前系统时间
	
	  //IWDG_Init(4,500); 
    
  	for(;;)
    {
        CurrentControlTick = xTaskGetTickCount();
        
				if(whole_init==1)
        dipan_Ctrl();
				if(separate_flag)
				{
					Circle_n+=0.001;	
				}
		    vTaskDelayUntil(&CurrentControlTick, 1 / portTICK_RATE_MS);
    }
}

