
#define __DRIVER_STATUSMACHINE_GLOBALS

#include "Config.h"
#include "Driver_DBUS.h"
#include "Driver_Judge.h"
#include "Driver_vision.h"
#include "Driver_Control.h"
#include "Driver_mpu9250.h"
#include "Driver_FricMotor.h"
#include "Driver_CloudMotor.h"
#include "Driver_StatusMachine.h"
#include "Driver_SuperGyroscope.h"

#include "bsp_oled.h"


/*
wasdǰ�����ҿ���
qe��������
���������䣨δ����Ħ����������Ħ���֣����Ҽ��Զ����
ctrl + shift�ر�Ħ����
*/

/**
  * @brief  ״̬����ʼ��
  * @param  void
  * @retval void
  */
void StatusMachine_InitConfig(void)
{
    ControlMode = ControlMode_KM;
    FricStatus = FricStatus_Stop;
    KMSubschema = KMSubschema_Normal;
}


/**
  * @brief  ״̬��״̬����
  * @param  void
  * @retval void
  * @note   ң�������뿪��1ң�������ƣ���1������3��Ħ���֣�2����
            ���뿪��3������ģʽ�����ȫ���ر�
            ���뿪��2�����̿��ƣ��Ҳ��뿪��3��Ħ���֣�����Ҽ��Զ����
            QE����
  */
uint8_t FristToKM = 1;
void StatusMachine_Update(portTickType Tick)
{
    
//����ֱ�ӽ����Զ�ģʽ
#if INFANTRY == 7
    
    static char Color = 'R';
    static uint32_t PushCounter = 0;
    static uint8_t RedCounter = 0, BlueCounter = 0, OffCounter = 0;
    static uint16_t ColorSendCounter = 0;
    
#if INFANTRY == 7
    int8_t LastPCVisionTargetColor;
#endif
    
    if(Tick > 8000)
    {
        //�����ж�
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9))
        {
            PushCounter = 0;
        }
        else
        {
            PushCounter++;
        }
        
        //��ɫ��������λ
        if(PushCounter == 1)        //�л���ɫ
        {
            if(Color == 'R')
            {
                Color = 'B';
                BlueCounter = 10;
                RedCounter = 0;
                OffCounter = 0;
                OLED_Print6x8Str(100, 10, 30, 8, (uint8_t *)"BLUE", INV_OFF, IS);
            }
            else
            {
                Color = 'R';
                BlueCounter = 0;
                RedCounter = 10;
                OffCounter = 0;
                OLED_Print6x8Str(100, 10, 30, 8, (uint8_t *)"RED ", INV_OFF, IS);
            }
        }
        else if(PushCounter == 1000)
        {
            BlueCounter = 0;
            RedCounter = 0;
            OffCounter = 50;
            OLED_Print6x8Str(100, 10, 30, 8, (uint8_t *)"OFF", INV_OFF, IS);
        }
        else        //ÿ2s������ɫ�л�ָ���֤��ɫ��ȷ
        {
            if(ColorSendCounter >= 300)
            {
                if(Color == 'R')
                {
                    RedCounter += 2;
                }
                else
                {
                    BlueCounter += 2;
                }
                
                ColorSendCounter = 0;
            }
            else
            {
                ColorSendCounter++;
            }
        }
        
        
        //����ָ��
        if(BlueCounter)
        {
            SendEnemyColor('B');
            BlueCounter--;
        }
        else if(RedCounter)
        {
            SendEnemyColor('R');
            RedCounter--;
        }
        else if(OffCounter)
        {
            SendPCOrder(PCOrder_Shutdown);
            OffCounter--;
        }
        
        FricStatus = FricStatus_Working;
        ControlMode = ControlMode_KM;
        
#if INFANTRY == 7
        //����Ŀ����ɫ��ʾ
        if(LastPCVisionTargetColor != PCVisionTargetColor)
        {
            LastPCVisionTargetColor = PCVisionTargetColor;
            if(PCVisionTargetColor == -1)
            {
                OLED_Print6x8Str(100, 30, 30, 8, (uint8_t *)" -  ", INV_OFF, IS);
            }
            else if(PCVisionTargetColor == 0)
            {
                OLED_Print6x8Str(100, 30, 30, 8, (uint8_t *)"RED ", INV_OFF, IS);
            }
            else if(PCVisionTargetColor == 1)
            {
                OLED_Print6x8Str(100, 30, 30, 8, (uint8_t *)"BLUE", INV_OFF, IS);
            }
        }
#endif
    }
    else
    {
        ControlMode = ControlMode_Protect;
    }
    
    return;
    
#else
    
    static uint8_t RateCounter = 0;
    static uint8_t BigSampleCounter = 0;
    static uint8_t AttackCounter = 0;
    static portTickType LastPCShutdownSignalTick = 0;
    portTickType CurrentTick = xTaskGetTickCount();
    
    //֡�ʹ��ͽ�����
    if(DBUSFrameRate < 3)
    {
        ControlMode = ControlMode_Protect;
        return;
    }
    
    //����ģʽ
    if(DBUS_ReceiveData.switch_left == 1)
    {
        ControlMode = ControlMode_RC;
    }
    else if(DBUS_ReceiveData.switch_left == 2)
    {
        ControlMode = ControlMode_KM;
    }
    else
    {
        ControlMode = ControlMode_Protect;
    } 
    
    //������ģʽ
    if(ControlMode == ControlMode_RC)
    {
/*******************************************  ��  Ħ����  ��  *******************************************/
        if((DBUS_ReceiveData.switch_right == 3) || (DBUS_ReceiveData.switch_right == 2))
        {
            FricStatus = FricStatus_Working;
        }
        else
        {
            FricStatus = FricStatus_Stop;
        }
/*******************************************  ��  Ħ����  ��  *******************************************/
        
        //�Զ��޸ļ�����ģʽ
        KMSubschema = KMSubschema_Normal;
    }
    //����ģʽ
    else if(ControlMode == ControlMode_KM)
    {
/*******************************************  ��  Ħ����  ��  *******************************************/
        if((DBUS_ReceiveData.switch_right == 3) || (DBUS_ReceiveData.switch_right == 2))
        {
            //��������
            FricStatus = FricStatus_Working;
        }
        else
        {
            FricStatus = FricStatus_Stop;
        }
/*******************************************  ��  Ħ����  ��  *******************************************/
/*******************************************  ��   ģʽ   ��  *******************************************/
        //����ʲôʱ����CTRL���ع��ֶ�Normalģʽ
        if(DBUS_CheckPush(KEY_SHIFT))
        {
            KMSubschema = KMSubschema_Normal;
        }
        
        if(KMSubschema == KMSubschema_Halfauto)
        {
            KMSubschema = KMSubschema_Normal;
        }
        
        //ֻ������Normalģʽ�½���ģʽ�л�
        if(KMSubschema == KMSubschema_Normal)
        {
            //����վģʽ
            if(DBUS_CheckPush(KEY_CTRL))
            {
                KMSubschema = KMSubschema_Supply;
            }
#if AUTOSHOOTSUPPORT == 1
//            //���Զ�ģʽ
//            else if(DBUS_ReceiveData.mouse.press_right)
//            {
//                KMSubschema = KMSubschema_Halfauto;
//            }
#endif
#if AUTOBIGSAMPLESUPPORT == 1
            //�Զ����ģʽ
            else if(DBUS_CheckPush(KEY_X))
            {
                KMSubschema = KMSubschema_Bigsample;
            }
#endif
            //�ֶ����ģʽ
            else if(DBUS_CheckPush(KEY_Z))
            {
                KMSubschema = KMSubschema_MannualBigsample;
            }
            //ҡ��ģʽ
            else if(DBUS_CheckPush(KEY_F))
            {
                KMSubschema = KMSubschema_Swing;
            }
            //ȫ�Զ�ģʽ
            else if(DBUS_CheckPush(KEY_C))
            {
                KMSubschema = KMSubschema_Fullauto;
            }
            //����ȦȦģʽ
            else if(DBUS_CheckPush(KEY_G))
            {
                KMSubschema = KMSubschema_AutoCircle;
            }
            //ȫ��ȦȦģʽ
            else if(DBUS_CheckPush(KEY_R))
            {
                KMSubschema = KMSubschema_FullCircle;
            }
            else
            {
                BigSampleCounter = 0;
                AttackCounter = 0;
            }
        }
        
        
        //�Ӿ�ָ��
        //���ͷ���Ƶ�ʼ�С���ڸ���
        if(RateCounter == 4)
        {
            //�л���ɫĿ��
            if((DBUS_ReceiveData.ch1 > 600) && 
                (DBUS_ReceiveData.ch2 > 600) &&
                (DBUS_ReceiveData.ch3 < -600) &&
                (DBUS_ReceiveData.ch4 > 600))
            {
                SendEnemyColor('R');
            }
            //�л���ɫĿ��
            else if((DBUS_ReceiveData.ch1 < -600) && 
                (DBUS_ReceiveData.ch2 > 600) &&
                (DBUS_ReceiveData.ch3 > 600) &&
                (DBUS_ReceiveData.ch4 > 600))
            {
                SendEnemyColor('B');
            }
            //���ģʽ
            else if(KMSubschema == KMSubschema_Bigsample)
            {
                if(BigSampleCounter < VisiolModeChangeDataSendNum)
                {
                    SendPCOrder(PCOrder_BigSample);
                    BigSampleCounter++;
                }
//                else
//                {
//                    SendPCOrder(PCOrder_BigSample);
//                }
            }
            //�Զ����ģʽ
            else if(KMSubschema == KMSubschema_Halfauto)
            {
                if(AttackCounter < VisiolModeChangeDataSendNum)
                {
                    SendPCOrder(PCOrder_Attack);
                    AttackCounter++;
                }
            }
            RateCounter = 0;
        }
        else
        {
            RateCounter++;
        }
    
    
        
        //���ڰ��Զ��ع�
        if((KMSubschema == KMSubschema_Halfauto) && (!DBUS_ReceiveData.mouse.press_right))
        {
            KMSubschema = KMSubschema_Normal;
        }
/*******************************************  ��   ģʽ   ��  *******************************************/
    }
    else
    {
        FricStatus = FricStatus_Stop;
        KMSubschema = KMSubschema_Normal;
        
        //�ػ�
        if((DBUS_ReceiveData.ch1 > 600) && 
            (DBUS_ReceiveData.ch2 < -600) &&
            (DBUS_ReceiveData.ch3 < -600) &&
            (DBUS_ReceiveData.ch4 > 600))
        {
            if(LastPCShutdownSignalTick + 5000 < CurrentTick)
            {
                SendPCOrder(PCOrder_Shutdown);
            }
        }
        else
        {
            LastPCShutdownSignalTick = CurrentTick;
        }
    }
    
#endif
    
}















