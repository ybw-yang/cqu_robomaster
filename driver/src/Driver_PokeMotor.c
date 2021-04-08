
#define __DRIVER_POKEMOTOR_GLOBALS

#include "OSinclude.h"
#include "Driver_Judge.h"
#include "Driver_Control.h"
#include "Driver_PokeMotor.h"

//�������������5050/Ȧ

/**
  * @brief  ���������ʼ��
  * @param  void
  * @retval void
  */
void Poke_InitConfig(void)
{
#if FRICTYPE == 1
    
    //����������ѹ����
    GPIO_ResetBits(GPIOC, GPIO_Pin_0); 
    
#else
    
    PokeMotorParam.RealLocation = 0;
    PokeMotorParam.RealSpeed = 0;
    PokeMotorParam.Status = PokeMotorParam_Working;
    PokeMotorParam.TargetLocation = 0;
    PokeMotorParam.LastShotTick = 0;
    
    //�������������λ�ó�ʼ��
    TIM3->CNT = POKEENCODERCenter;
    
    //������������ʼ��Ϊ˳ʱ��
    GPIO_SetBits(GPIOA, GPIO_Pin_5);
    
#endif
}


#if FRICTYPE == 1

/**
  * @brief  �����ƽ�
  * @param  1 �ƽ�  0 ����
  * @retval void
  * @note   �˺�����������ִ��
  */
void Poke_CylinderAdjust(uint8_t Target)
{
    static portTickType LastShotTick = 0;
    portTickType CurrentTick = xTaskGetTickCount();
    
    if(CurrentTick - LastShotTick < 100)
    {
        Poke_CylinderControl(1);
    }
    else if(CurrentTick - LastShotTick < ARTILLERYSHOTCRACK)
    {
        Poke_CylinderControl(0);
    }
    else if(Target)
    {
        Poke_CylinderControl(1);
        LastShotTick = CurrentTick;
    }
    else
    {
        Poke_CylinderControl(0);
    }
}


/**
  * @brief  ���׿���
  * @param  1 �Ƴ�        0 �ջ�
  * @retval void
  */
void Poke_CylinderControl(uint8_t Target)
{
    
    if(Target)
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_1); 
    }
    else
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_1); 
    }
}

#else

/**
  * @brief  ���������������
  * @param  ������С����399��
  * @retval void
  */
void Poke_MotorCurrent(int16_t Current)
{
    int16_t S;
    
    S = Current > 399 ? 399 : Current;
    S = Current < -399 ? -399 : Current;
    
    //˳ʱ��
    if(Current >= 0)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
        TIM5->CCR2 = S;
    }
    else
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        TIM5->CCR2 = -S;
    }
}


/**
  * @brief  �����������һ��
  * @param  void
  * @retval 1 �����ɹ�      0 ����ʧ��
  * @note   ����������ڷ�ֹͣ״̬��working,struck����Ч,������Ƶ
  */
uint8_t Poke_MotorStep(void)
{
    portTickType CurrentTick = xTaskGetTickCount();
    
    if((PokeMotorParam.Status == PokeMotorParam_Working) && 
        (CurrentTick - PokeMotorParam.LastShotTick >= POKESTEPMINTIMECRACK))
    {
        PokeMotorParam.TargetLocation = PokeMotorParam.RealLocation - POKELINESPERSTEP;
        PokeMotorParam.LastShotTick = CurrentTick;
        
        return 1;
    }
    return 0;
}


/**
  * @brief  ����������ƺ���
  * @param  1 PID����     0ʧ��
  * @retval void
  * @note   �˺���Ҫ����������
  */
void Poke_MotorAdjust(uint8_t mode)
{
    int16_t PokeCurrent;
    static portTickType LastStruckDealTick = 0;
    
    portTickType CurrentTick = xTaskGetTickCount();
    
    
    if(mode)
    {
        PokeMotorParam.RealSpeed = POKEENCODERCenter - TIM3->CNT;
        TIM3->CNT = POKEENCODERCenter;
        
        //���С��30��
        if(((PokeOPID.CurrentError < 30) && (PokeOPID.CurrentError > -30)))
        {
            PokeMotorParam.Status = PokeMotorParam_Working;
        }
            
        if(((PokeMotorParam.TargetLocation > PokeMotorParam.RealLocation) && (PokeIPID.Iout > PokeIPID.IMax * 0.85F)) || ((PokeMotorParam.TargetLocation < PokeMotorParam.RealLocation) && (PokeIPID.Iout < -PokeIPID.IMax * 0.85F)))
        {
            PokeMotorParam.Status = PokeMotorParam_Stuck;
            if(CurrentTick - LastStruckDealTick >= 400)
            {
                if((PokeMotorParam.TargetLocation != PokeMotorParam.RealLocation))
                {
                    PokeMotorParam.TargetLocation = PokeMotorParam.RealLocation + ((PokeMotorParam.TargetLocation > PokeMotorParam.RealLocation) ? (-POKESTRUCKDEALLINES) : POKESTRUCKDEALLINES);
                }
                else
                {
                    PokeMotorParam.TargetLocation = PokeMotorParam.RealLocation + ((PokeOPID.PIDout > 0) ? (-POKESTRUCKDEALLINES) : POKESTRUCKDEALLINES);
                }
            }
            LastStruckDealTick = CurrentTick;
        }
    
        //ʵ��λ�ü���
        PokeMotorParam.RealLocation += PokeMotorParam.RealSpeed;
        
        PokeCurrent = Control_PokeIPID();
        
        Poke_MotorCurrent(PokeCurrent);
    }
    else
    {
        Poke_MotorCurrent(0);
    }
}


/**
  * @brief  ��������ٶȿ��ƺ���������ר�ã�
  * @param  1 PID����     0ʧ��
  * @retval void
  * @note   �˺���Ҫ����������
  */
void Poke_MotorSpeedAdjust(uint8_t mode, uint8_t speed)
{
    int16_t PokeCurrent;
    static portTickType StuckDealTick = 0;
    static int8_t LastDir = 1;
    
    portTickType CurrentTick = xTaskGetTickCount();
    
    
    if(mode)
    {
        PokeMotorParam.RealSpeed = POKEENCODERCenter - TIM3->CNT;
        TIM3->CNT = POKEENCODERCenter;
        
        //���С��30��
        if(((PokeOPID.CurrentError < 30) && (PokeOPID.CurrentError > -30)))
        {
            PokeMotorParam.Status = PokeMotorParam_Working;
        }
            
        //����
        if(StuckDealTick < CurrentTick)
        {
            if((PokeIPID.Iout > PokeIPID.IMax * 0.85F) || (PokeIPID.Iout < -PokeIPID.IMax * 0.85F))
            {
                PokeMotorParam.Status = PokeMotorParam_Stuck;
                StuckDealTick = CurrentTick + 450;
                LastDir *= -1;
            }
        }
        
        PokeCurrent = Poke_MotorSpeedPID(LastDir * speed);
        
//        if(StuckDealTick > CurrentTick)
//        {
//            PokeCurrent = Poke_MotorSpeedPID(LastDir * 20);
//        }
//        else
//        {
//            PokeCurrent = Poke_MotorSpeedPID(LastDir * 20);
//        }
        
        Poke_MotorCurrent(PokeCurrent);
    }
    else
    {
        Poke_MotorCurrent(0);
    }
}

#endif








