#include "main.h"
#include "OSinclude.h"

xQueueHandle    Queue_CANSend;   //定义CAN发送句柄
 
//任务优先级
#define LED0_TASK_PRIO		2
//任务堆栈大小	
#define LED0_STK_SIZE 		50  
//任务句柄
TaskHandle_t LED0Task_Handler;
//任务函数
void led0_task(void *pvParameters);

//任务优先级
#define LED1_TASK_PRIO		3
//任务堆栈大小	
#define LED1_STK_SIZE 		50  
//任务句柄
TaskHandle_t LED1Task_Handler;
//任务函数
void led1_task(void *pvParameters);

//任务优先级
#define FLOAT_TASK_PRIO		4
//任务堆栈大小	
#define FLOAT_STK_SIZE 		128
//任务句柄
TaskHandle_t FLOATTask_Handler;
//任务函数
void float_task(void *pvParameters);

						/*底盘控制2ms*/
//任务优先级
#define Task_Control_PRIO		5
//任务堆栈大小	
#define Task_Control_STK_SIZE 		512
//任务句柄
TaskHandle_t Task_Control_Handler;
//任务函数
void Task_Control(void *Parameters);

						/*控制量运算7ms*/
//任务优先级
#define Task_Upper_part_PRIO	7
//任务堆栈大小	
#define Task_Upper_part_STK_SIZE 		512
//任务句柄
TaskHandle_t Task_Upper_part_Handler;
//任务函数
void Upper_part(void);

/*can发送函数*/
//任务优先级
#define TASK_CANSend_PRIO		5
//任务堆栈大小	
#define Task_CANSend_STK_SIZE 		200
//任务句柄
TaskHandle_t Task_CANSend_Handler;
//任务函数
void Task_CANSend(void *Parameters);

/*--PID运算--*/
//任务优先级
#define Task_PID_Calculation_PRIO		6
//任务堆栈大小	
#define Task_PID_Calculation_STK_SIZE 		100  
//任务句柄
TaskHandle_t Task_PID_Calculation_Handler;
//任务函数
void Task_PID_Calculation(void);

/*--读MPU6500--*/
//任务优先级
#define Task_gpyo_read_PRIO		5
//任务堆栈大小	
#define Task_gpyo_read_STK_SIZE 		512  
//任务句柄
TaskHandle_t Task_gpyo_read_Handler;
//任务函数
void Task_gpyo_read(void);

/*--云台控制--*/
//任务优先级
#define Task_Yuntai_Ctrl_PRIO		5
//任务堆栈大小	
#define Task_Yuntai_Ctrl_STK_SIZE 		100  
//任务句柄
TaskHandle_t Task_Yuntai_Ctrl_Handler;
//任务函数
void Task_Yuntai_Ctrl(void);

/*--超声波测距--*/
//任务优先级
#define Task_Ultrasonic_PRIO		7
//任务堆栈大小	
#define Task_Ultrasonic_STK_SIZE 		50  
//任务句柄
TaskHandle_t Task_Ultrasonic_Handler;
//任务函数
void Task_Ultrasonic(void);


//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
	
		Queue_CANSend = xQueueCreate(64, sizeof(CanSend_Type));	
	
    //创建LED0任务
    xTaskCreate((TaskFunction_t )led0_task,     	
                (const char*    )"led0_task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&LED0Task_Handler);   
    //创建LED1任务
    xTaskCreate((TaskFunction_t )led1_task,     
                (const char*    )"led1_task",   
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler);        
    //浮点测试任务
    xTaskCreate((TaskFunction_t )float_task,     
                (const char*    )"float_task",   
                (uint16_t       )FLOAT_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )FLOAT_TASK_PRIO,
                (TaskHandle_t*  )&FLOATTask_Handler);  
								
		//CAN发送任务
    xTaskCreate((TaskFunction_t )Task_CANSend,     
                (const char*    )"Task_CANSend",   
                (uint16_t       )Task_CANSend_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )TASK_CANSend_PRIO,
                (TaskHandle_t*  )&Task_CANSend_Handler);  
	
		//底盘控制任务
    xTaskCreate((TaskFunction_t )Task_Control,     
                (const char*    )"Task_Control",   
                (uint16_t       )Task_Control_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )Task_Control_PRIO,
                (TaskHandle_t*  )&Task_Control_Handler);  
		
		//控制数据计算任务
		xTaskCreate((TaskFunction_t )Upper_part,
                (const char*    )"Upper_part",
                (uint16_t       )Task_Upper_part_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Task_Upper_part_PRIO,
                (TaskHandle_t*  )&Task_Upper_part_Handler);
								
		//PID运算
		xTaskCreate((TaskFunction_t )Task_PID_Calculation,
                (const char*    )"Task_PID_Calculation",
                (uint16_t       )Task_PID_Calculation_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Task_PID_Calculation_PRIO,
                (TaskHandle_t*  )&Task_PID_Calculation_Handler);			
								
		//云台控制函数
		xTaskCreate((TaskFunction_t )Task_Yuntai_Ctrl,
                (const char*    )"Task_Yuntai_Ctrl",
                (uint16_t       )Task_Yuntai_Ctrl_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Task_Yuntai_Ctrl_PRIO,
                (TaskHandle_t*  )&Task_Yuntai_Ctrl_Handler);		
								
		//读6500数据						
		xTaskCreate((TaskFunction_t )Task_gpyo_read,
                (const char*    )"Task_gpyo_read",
                (uint16_t       )Task_gpyo_read_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Task_gpyo_read_PRIO,
                (TaskHandle_t*  )&Task_gpyo_read_Handler);			

		//超声波测距					
		xTaskCreate((TaskFunction_t )Task_Ultrasonic,
                (const char*    )"Task_Ultrasonic",
                (uint16_t       )Task_Ultrasonic_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Task_Ultrasonic_PRIO,
                (TaskHandle_t*  )&Task_Ultrasonic_Handler);
								
    vTaskDelete(NULL);              //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}
