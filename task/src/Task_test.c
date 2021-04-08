#include "main.h"
#include "OSinclude.h"

//LED0������ 
void led0_task(void *pvParameters)
{
    while(1)
    {
        LED_GREEN_OFF;
        vTaskDelay(500);
			  LED_GREEN_ON;
			  vTaskDelay(500);
    }
}   

//LED1������
void led1_task(void *pvParameters)
{
    while(1)
    {
        LED_RED_OFF;
        vTaskDelay(500);
			  LED_RED_ON;
			  vTaskDelay(500);
    }
}

//�����������
void float_task(void *pvParameters)
{
	static float float_num=0.00;
	while(1)
	{
			float_num+=0.01f;
		   printf("float_num��ֵΪ: %.4f\r\n",float_num);
        vTaskDelay(1000);
	}
}

