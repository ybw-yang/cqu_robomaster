
#include "Handler.h"

#include "Task_Monitor.h"

#include "bsp_oled.h"
#include "Driver_Bell.h"
#include "Driver_DBUS.h"
#include "Driver_Judge.h"
#include "Driver_vision.h"
#include "Driver_Chassis.h"
#include "Driver_mpu9250.h"
#include "Driver_CloudMotor.h"
#include "Driver_SuperGyroscope.h"

#include "Config.h"

int16_t speed = 0;

/**
  * @brief  ����������
  * @param  unused
  * @retval voif
  */
void Task_Monitor(void *Parameters)
{
    static uint8_t WarningCounter = 0;
    uint8_t WarningLocation = 0;
    //ϵͳ״̬��ÿһλ��Ӧһ������0������1����
/****************************************************************
    15  |   14  |   13  |   12  |   11  |   10  |   9   |   8   |
        |       |       |       |       |����ϵͳ|��ǰ����|��ǰ����|
-----------------------------------------------------------------
    7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
������|�Һ����|����Gyr|    -  | vision|  Yaw  | Pitch |  DBUS |
****************************************************************/
    uint16_t SysErrorStatus = 0;
    
    for(;;)
    {
/**************************  ��   ����֡��ͳ��   ��  **************************/
        //DBUS֡��ͳ��
        DBUSFrameRate = DBUSFrameCounter;	
        DBUSFrameCounter = 0;
        
        //��̨���֡��ͳ��
        CloudParam.Pitch.FrameRate = CloudParam.Pitch.FrameCounter;
        CloudParam.Pitch.FrameCounter = 0;
        CloudParam.Yaw.FrameRate = CloudParam.Yaw.FrameCounter;
        CloudParam.Yaw.FrameCounter = 0;
        
        //��������������֡ͳ��
        SuperGyoFrameRate = SuperGyoFrameCounter;
        SuperGyoFrameCounter = 0;
        
        //�Ӿ�֡��ͳ��
        PCFrameRate = PCFrameCounter;
        PCFrameCounter = 0;
        
        //���̵��֡��ͳ��
        ChassisFrameRate[0] = ChassisFrameCounter[0];
        ChassisFrameCounter[0] = 0;
        ChassisFrameRate[1] = ChassisFrameCounter[1];
        ChassisFrameCounter[1] = 0;
        ChassisFrameRate[2] = ChassisFrameCounter[2];
        ChassisFrameCounter[2] = 0;
        ChassisFrameRate[3] = ChassisFrameCounter[3];
        ChassisFrameCounter[3] = 0;
        
        //����ϵͳ֡��ͳ��
        JudgeFrameRate = JudgeFrameCounter;
        JudgeFrameCounter = 0;
        
        //DBUS֡�ʹ���
        if(DBUSFrameRate < 6)
        {
            SysErrorStatus |= 0x0001;
        }
        else
        {
            SysErrorStatus &= 0xFFFE;
        }
        //Pitch��̨֡�ʹ���
        if(CloudParam.Pitch.FrameRate < 30)
        {
            SysErrorStatus |= 0x0002;
        }
        else
        {
            SysErrorStatus &= 0xFFFD;
        }
        //Yaw��̨֡�ʹ���
        if(CloudParam.Yaw.FrameRate < 30)
        {
            SysErrorStatus |= 0x0004;
        }
        else
        {
            SysErrorStatus &= 0xFFFB;
        }
        //�Ӿ�֡�ʹ���
        if(PCFrameRate < 2)
        {
            SysErrorStatus |= 0x0008;
        }
        else
        {
            SysErrorStatus &= 0xFFF7;
        }
        //����������֡�ʹ���
        if(SuperGyoFrameRate < 30)
        {
            SysErrorStatus |= 0x0020;
        }
        else
        {
            SysErrorStatus &= 0xFFDF;
        }
        //��ǰ���̵��֡�ʹ���
        if(ChassisFrameRate[0] < 30)
        {
            SysErrorStatus |= 0x0040;
        }
        else
        {
            SysErrorStatus &= 0xFFBF;
        }
        //��ǰ���̵��֡�ʹ���
        if(ChassisFrameRate[1] < 30)
        {
            SysErrorStatus |= 0x0080;
        }
        else
        {
            SysErrorStatus &= 0xFF7F;
        }
        //�����̵��֡�ʹ���
        if(ChassisFrameRate[2] < 30)
        {
            SysErrorStatus |= 0x0100;
        }
        else
        {
            SysErrorStatus &= 0xFEFF;
        }
        //�Һ���̵��֡�ʹ���
        if(ChassisFrameRate[3] < 30)
        {
            SysErrorStatus |= 0x0200;
        }
        else
        {
            SysErrorStatus &= 0xFDFF;
        }
        //����ϵͳ֡�ʹ���
        if(JudgeFrameRate < 3)
        {
            SysErrorStatus |= 0x0400;
            ChassisMaxSumCurrent = 2380;
        }
        else
        {
            SysErrorStatus &= 0xFBFF;
        }
        
/**************************  ��   ����֡��ͳ��   ��  **************************/
/**************************************************************************************************/
/**************************  ��   ����   ��  **************************/
        if(WarningCounter == 0)
        {
            WarningLocation = 0;
            GUI_WidgetText_SetText(Oled_Handler, "");
            
#if INFANTRY == 7
            
            //yaw��̨���
            if(SysErrorStatus & 0x0004)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "Yaw   ");
#endif
                WarningLocation++;
            }
            //pitch��̨���
            if(SysErrorStatus & 0x0002)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "Pitch ");
#endif
                WarningLocation++;
            }
            //��������
            if(SysErrorStatus & 0x0008)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "PC    ");
#endif
                WarningLocation++;
            }
            //����������
            if(SysErrorStatus & 0x0020)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "GROY  ");
#endif
                WarningLocation++;
            }
            //����ϵͳ
            if(SysErrorStatus & 0x0400)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "Judge ");
#endif
                WarningLocation++;
            }
            //�޾���
            if(WarningLocation == 0)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "OK    ");
#endif
                WarningLocation++;
            }
            
#else
            
            //DBUS
            if(SysErrorStatus & 0x0001)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "DBUS  ");
#endif
                WarningLocation++;
            }
            //pitch��̨���
            if(SysErrorStatus & 0x0002)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "Pitch ");
#endif
                WarningLocation++;
            }
            //yaw��̨���
            if(SysErrorStatus & 0x0004)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "Yaw   ");
#endif
                WarningLocation++;
            }
            //��������
            if(SysErrorStatus & 0x0008)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "PC    ");
#endif
                WarningLocation++;
            }
            //����������
            if(SysErrorStatus & 0x0020)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "Groy  ");
#endif
                WarningLocation++;
            }
            //��ǰ���̵��
            if(SysErrorStatus & 0x0040)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "LF    ");
#endif
                WarningLocation++;
            }
            //��ǰ���̵��
            if(SysErrorStatus & 0x0080)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "RF    ");
#endif
                WarningLocation++;
            }
            //�����̵��
            if(SysErrorStatus & 0x0100)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "LB    ");
#endif
                WarningLocation++;
            }
            //�Һ���̵��
            if(SysErrorStatus & 0x0200)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "RB    ");
#endif
                WarningLocation++;
            }
            //����ϵͳ
            if(SysErrorStatus & 0x0400)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "Judge ");
#endif
                WarningLocation++;
            }
            //�޾���
            if(WarningLocation == 0)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "OK    ");
#endif
                WarningLocation++;
            }
            
#endif
            
        GUI_WidgetText_Show(Oled_Handler, IS, IS);
            
        }
        else
        {
            if(WarningCounter >= 2)
            {
                WarningCounter = 0;
            }
            else
            {
                WarningCounter++;
            }
        }
/**************************  ��   �����־��λ   ��  **************************/

        
        vTaskDelay(200);
    }
}
