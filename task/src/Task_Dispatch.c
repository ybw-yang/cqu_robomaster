#include "main.h"
#include "OSinclude.h"

xQueueHandle    Queue_CANSend;   //����CAN���;��
 
//�������ȼ�
#define LED0_TASK_PRIO		2
//�����ջ��С	
#define LED0_STK_SIZE 		50  
//������
TaskHandle_t LED0Task_Handler;
//������
void led0_task(void *pvParameters);

//�������ȼ�
#define LED1_TASK_PRIO		3
//�����ջ��С	
#define LED1_STK_SIZE 		50  
//������
TaskHandle_t LED1Task_Handler;
//������
void led1_task(void *pvParameters);

//�������ȼ�
#define FLOAT_TASK_PRIO		4
//�����ջ��С	
#define FLOAT_STK_SIZE 		128
//������
TaskHandle_t FLOATTask_Handler;
//������
void float_task(void *pvParameters);

						/*���̿���2ms*/
//�������ȼ�
#define Task_Control_PRIO		5
//�����ջ��С	
#define Task_Control_STK_SIZE 		512
//������
TaskHandle_t Task_Control_Handler;
//������
void Task_Control(void *Parameters);

						/*����������7ms*/
//�������ȼ�
#define Task_Upper_part_PRIO	7
//�����ջ��С	
#define Task_Upper_part_STK_SIZE 		512
//������
TaskHandle_t Task_Upper_part_Handler;
//������
void Upper_part(void);

/*can���ͺ���*/
//�������ȼ�
#define TASK_CANSend_PRIO		5
//�����ջ��С	
#define Task_CANSend_STK_SIZE 		200
//������
TaskHandle_t Task_CANSend_Handler;
//������
void Task_CANSend(void *Parameters);

/*--PID����--*/
//�������ȼ�
#define Task_PID_Calculation_PRIO		6
//�����ջ��С	
#define Task_PID_Calculation_STK_SIZE 		100  
//������
TaskHandle_t Task_PID_Calculation_Handler;
//������
void Task_PID_Calculation(void);

/*--��MPU6500--*/
//�������ȼ�
#define Task_gpyo_read_PRIO		5
//�����ջ��С	
#define Task_gpyo_read_STK_SIZE 		512  
//������
TaskHandle_t Task_gpyo_read_Handler;
//������
void Task_gpyo_read(void);

/*--��̨����--*/
//�������ȼ�
#define Task_Yuntai_Ctrl_PRIO		5
//�����ջ��С	
#define Task_Yuntai_Ctrl_STK_SIZE 		100  
//������
TaskHandle_t Task_Yuntai_Ctrl_Handler;
//������
void Task_Yuntai_Ctrl(void);

/*--���������--*/
//�������ȼ�
#define Task_Ultrasonic_PRIO		7
//�����ջ��С	
#define Task_Ultrasonic_STK_SIZE 		50  
//������
TaskHandle_t Task_Ultrasonic_Handler;
//������
void Task_Ultrasonic(void);


//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
	
		Queue_CANSend = xQueueCreate(64, sizeof(CanSend_Type));	
	
    //����LED0����
    xTaskCreate((TaskFunction_t )led0_task,     	
                (const char*    )"led0_task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&LED0Task_Handler);   
    //����LED1����
    xTaskCreate((TaskFunction_t )led1_task,     
                (const char*    )"led1_task",   
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler);        
    //�����������
    xTaskCreate((TaskFunction_t )float_task,     
                (const char*    )"float_task",   
                (uint16_t       )FLOAT_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )FLOAT_TASK_PRIO,
                (TaskHandle_t*  )&FLOATTask_Handler);  
								
		//CAN��������
    xTaskCreate((TaskFunction_t )Task_CANSend,     
                (const char*    )"Task_CANSend",   
                (uint16_t       )Task_CANSend_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )TASK_CANSend_PRIO,
                (TaskHandle_t*  )&Task_CANSend_Handler);  
	
		//���̿�������
    xTaskCreate((TaskFunction_t )Task_Control,     
                (const char*    )"Task_Control",   
                (uint16_t       )Task_Control_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )Task_Control_PRIO,
                (TaskHandle_t*  )&Task_Control_Handler);  
		
		//�������ݼ�������
		xTaskCreate((TaskFunction_t )Upper_part,
                (const char*    )"Upper_part",
                (uint16_t       )Task_Upper_part_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Task_Upper_part_PRIO,
                (TaskHandle_t*  )&Task_Upper_part_Handler);
								
		//PID����
		xTaskCreate((TaskFunction_t )Task_PID_Calculation,
                (const char*    )"Task_PID_Calculation",
                (uint16_t       )Task_PID_Calculation_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Task_PID_Calculation_PRIO,
                (TaskHandle_t*  )&Task_PID_Calculation_Handler);			
								
		//��̨���ƺ���
		xTaskCreate((TaskFunction_t )Task_Yuntai_Ctrl,
                (const char*    )"Task_Yuntai_Ctrl",
                (uint16_t       )Task_Yuntai_Ctrl_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Task_Yuntai_Ctrl_PRIO,
                (TaskHandle_t*  )&Task_Yuntai_Ctrl_Handler);		
								
		//��6500����						
		xTaskCreate((TaskFunction_t )Task_gpyo_read,
                (const char*    )"Task_gpyo_read",
                (uint16_t       )Task_gpyo_read_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Task_gpyo_read_PRIO,
                (TaskHandle_t*  )&Task_gpyo_read_Handler);			

		//���������					
		xTaskCreate((TaskFunction_t )Task_Ultrasonic,
                (const char*    )"Task_Ultrasonic",
                (uint16_t       )Task_Ultrasonic_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Task_Ultrasonic_PRIO,
                (TaskHandle_t*  )&Task_Ultrasonic_Handler);
								
    vTaskDelete(NULL);              //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}
