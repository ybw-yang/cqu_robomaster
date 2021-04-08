
#define __DRIVER_VISION_GLOBALS

#include "Config.h"
#include "Driver_vision.h"
#include "Driver_mpu9250.h"
#include "Driver_CloudMotor.h"
#include <math.h>


//����ģʽָ��
const uint8_t AttackModeOrder[PCDATALENGTH] = {0x00, 0x00, 0x09, 0x00, 0x00,
                                               0x00, 0x00, 0x00, 0x00, 0x00,
                                               0x00, 0x00, 0x00, 0x00, 0x00,
                                               0x00, 0x00, 0x00, 0x00, 0x00,
                                               0x00, 0x00, 0x00, 0x00, 0x00,
                                               0x00};

//���ģʽָ��
const uint8_t BigSymbolModeOrder[PCDATALENGTH] = {0x00, 0x00, 0x28, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00};

//��Ŀ��ָ��
const uint8_t RedEnemyOrder[PCDATALENGTH] = {0x00, 0x00, 0xFA, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00};

//��Ŀ��ָ��
const uint8_t BlueEnemyOrder[PCDATALENGTH] = {0x00, 0x00, 0xFB, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00};

//�ػ�ģʽָ��
const uint8_t ShutdownOrder[PCDATALENGTH] = {0x00, 0x00, 0xFF, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00};



/*
X�� ��
Y�� ��
Z�� ǰ
*/


/**
  * @brief  �Ӿ���ʼ��
  * @param  void
  * @retval void
  */
void Vision_InitConfig(void)
{
    PCDataBufferPoint = 0;
    EnemyDataBufferPoint = 0;
    
#if INFANTRY == 7
    PCVisionTargetColor = -1;
#endif
    
    EnemyDataBuffer[EnemyDataBufferPoint].ID = 1;
    EnemyDataBuffer[EnemyDataBufferPoint].Time = 0;
    EnemyDataBuffer[EnemyDataBufferPoint].X = 0;
    EnemyDataBuffer[EnemyDataBufferPoint].Y = 0;
    EnemyDataBuffer[EnemyDataBufferPoint].Z = 1;
}


/**
  * @brief  ��ֱ������ϵת��Ϊ����������Ƕ�
  * @param  X
  * @param  Y
  * @param  Z
  * @param  0 �������������ٶ�      1 �����������ٶ�
  * @retval �Ƕ�(���������м�Ϊ0��
  */
//    AngleF_Struct ReturnData;
//    float distance;
//    float X0, Y0, Z0;
//    float L;
//    float AngleA;
AngleF_Struct RecToPolar(float X, float Y, float Z, float RealPitch, uint16_t PitchEncoder, uint8_t mode)
{
    
//    L = sqrt(X * X + Z * Z);
//    AngleA = (CloudParam.Pitch.RealABSAngle - (CloudParam.Pitch.RealEncoderAngle - PitchEncoderCenter) * 0.04395F) * 0.0087266F;
//    X0 = X;
//    Y0 = L * sin(AngleA);
//    Z0 = Z;
//    distance = L * cos(AngleA);
//    
//    if((X0 == 0) && (Y0 == 0) && (Z0 == 0))
//    {
//        ReturnData.H = 0;
//        ReturnData.V = 0;
//        return ReturnData;
//    }
//    
//    ReturnData.H =  - atan(X0 / Z0) * 57.2958F;
//    
//    if(mode == 0)
//    {
//    //�������������ٶ�
//        ReturnData.V = -atan(Y0 / distance) * 57.2958F;
//    }
//    else
//    {
//    //�����������ٶ�
//        ReturnData.V = (atan(((ACCofGravity * distance * distance) / (GUNSpeed * GUNSpeed) - Y0) / sqrt(Y0 * Y0 + distance * distance)) - atan(Y0 / distance)) / 2 * 57.2958F;
//    }
//    
//    return ReturnData;









    AngleF_Struct ReturnData;
    float Distance = sqrt(X * X + Z * Z);
    float distance;;
    float X0, Y0, Z0;
    
    X0 = X;
    Y0 = Y;
    Z0 = Z;
    
    if((X0 == 0) && (Y0 == 0) && (Z0 == 0))
    {
        ReturnData.H = 0;
        ReturnData.V = 0;
        return ReturnData;
    }
    
    ReturnData.H =  - atan(X / Z0) * 57.2958F;
    
    distance = sqrt(X0 * X0 + Z0 * Z0);
    if(mode == 0)
    {
    //�������������ٶ�
        ReturnData.V = -atan(Y0 / distance) * 57.2958F;
    }
    else
    {
    //�����������ٶ�
        ReturnData.V = (atan(((ACCofGravity * distance * distance) / (GUNSpeed * GUNSpeed) - Y0) / sqrt(Y0 * Y0 + distance * distance)) - atan(Y0 / distance)) / 2 * 57.2958F;
    }
    
    return ReturnData;
}


/**
  * @brief  ·����Ϻ��ĺ����������������һ�����
  * @param  Ԥ������ʱ�䳤��
  * @param  Ԥ��ʱ��
  * @param  Ԥ��λ��
  * @retval 0 �ɹ�        1 ʧ��
  */
uint8_t ForcastCore(uint16_t SampleTime, uint16_t ForcastTime, Point_Struct *ForcastPoint)
{
    int RelativeTime;                       //���ʱ�䣬��ֹ����ʱ�䳬��Χ
    uint16_t index = 0, Currentindex;       
    uint16_t SampleNum = 0;
    

    float A = 0;
    float B = 0;
    float C = 0;
    float D = 0;
    float E = 0;
    
    float Fx = 0;
    float Gx = 0;
    float Hx = 0;
    float Ix = 0;
    float Jx = 0;
    float Kx = 0;
    float Lx = 0;
    float Mx = 0;
    float Nx = 0;
    
    float Fz = 0;
    float Gz = 0;
    float Hz = 0;
    float Iz = 0;
    float Jz = 0;
    float Kz = 0;
    float Lz = 0;
    float Mz = 0;
    float Nz = 0;
    
    float Pax, Pbx, Pcx;
    float Paz, Pbz, Pcz;
    
    //Ѱ�����
    for(SampleNum = 0; SampleNum < ENEMYDATABUFFERLENGHT; SampleNum++)
    {
        if(EnemyDataBuffer[(EnemyDataBufferPoint + ENEMYDATABUFFERLENGHT - SampleNum - 1) % ENEMYDATABUFFERLENGHT].Time + SampleTime < EnemyDataBuffer[EnemyDataBufferPoint].Time)
        {
            break;
        }
    }
    
    //�������������
    if(SampleNum < 5)
    {
        return 1;
    }
    
    E =  -(1 + SampleNum);
    
    //�������
    for(index = 0; index <= SampleNum; index++)
    {
        Currentindex = (EnemyDataBufferPoint + ENEMYDATABUFFERLENGHT - index) % ENEMYDATABUFFERLENGHT;
        
        RelativeTime = EnemyDataBuffer[Currentindex].Time - EnemyDataBuffer[(EnemyDataBufferPoint + ENEMYDATABUFFERLENGHT - SampleNum) % ENEMYDATABUFFERLENGHT].Time;        
        
        A = A - RelativeTime * RelativeTime * RelativeTime * RelativeTime;
        B = B - RelativeTime * RelativeTime * RelativeTime;
        C = C - RelativeTime * RelativeTime;
        D = D - RelativeTime;
        
        Fx = Fx + RelativeTime * RelativeTime * EnemyDataBuffer[Currentindex].X;
        Gx = Gx + RelativeTime * EnemyDataBuffer[Currentindex].X;
        Hx = Hx + EnemyDataBuffer[Currentindex].X;
        
        Fz = Fz + RelativeTime * RelativeTime * EnemyDataBuffer[Currentindex].Z;
        Gz = Gz + RelativeTime * EnemyDataBuffer[Currentindex].Z;
        Hz = Hz + EnemyDataBuffer[Currentindex].Z;
    }
    

    Ix = D * Fx - C * Gx;
    Jx = A * D - B * C;
    Kx = B * D - C * C;
    Lx = E * Fx - Hx * C;
    Mx = A * E - C * C;
    Nx = B * E - C * D;
    
    Iz = D * Fz - C * Gz;
    Jz = A * D - B * C;
    Kz = B * D - C * C;
    Lz = E * Fz - Hz * C;
    Mz = A * E - C * C;
    Nz = B * E - C * D;
    
    //���ݷǷ�
    if((!(Mx * Kx - Jx * Nx)) ||
        (!Kx) ||
        (!C) ||
        (!(Mz * Kz - Jz * Nz)) ||
        (!Kz))
    {
        return 1;
    }

    Pax = (Ix * Nx - Lx * Kx) / (Mx * Kx - Jx * Nx);
    Pbx = -(Ix + Pax * Jx)  / Kx;
    Pcx = - (Fx + Pax * A + Pbx * B) / C;
    
    Paz = (Iz * Nz - Lz * Kz) / (Mz * Kz - Jz * Nz);
    Pbz = -(Iz + Paz * Jz) / Kz;
    Pcz = - (Fz + Paz * A + Pbz * B) / C;
    
    ForcastTime += EnemyDataBuffer[EnemyDataBufferPoint].Time - EnemyDataBuffer[(EnemyDataBufferPoint + ENEMYDATABUFFERLENGHT - SampleNum) % ENEMYDATABUFFERLENGHT].Time;
    
    ForcastPoint->X = (ForcastTime * ForcastTime * Pax + Pbx * ForcastTime + Pcx);
    ForcastPoint->Y = EnemyDataBuffer[Currentindex].Y;
    ForcastPoint->Z = (Paz * ForcastTime * ForcastTime + Pbz * ForcastTime + Pcz);
    
    return 0;
}


/**
  * @brief  ����һ����ϣ����Ŀ��Ƕȣ������̨�����
  * @param  �������ʱ��(ms)
  * @param  ���ʱ�䳤��(ms)
  * @param  ʱ��ģʽ    0 �Զ���ʱ�䣨��ForcastTime���� 1 �Զ��趨Ԥ��ʱ�䣨���ݾ��뼰�ӵ��ٶ�ȷ����
  * @retval 0 ������ϳɹ�        1 ���������ݲ��������ʧ��ֱ��ʹ�õ�ǰλ����ΪĿ��λ��
  */
uint8_t ForcastOnce(uint16_t SampleTime, uint16_t ForcastTime, AngleF_Struct *ForcastAngle, uint8_t TimeMode)
{
    Point_Struct ForcastPoint;
    float distance;
    
    //�Զ��������ʱ�䳤��
    if(TimeMode)
    {
        distance = sqrt(EnemyDataBuffer[EnemyDataBufferPoint].X * EnemyDataBuffer[EnemyDataBufferPoint].X + EnemyDataBuffer[EnemyDataBufferPoint].Y * EnemyDataBuffer[EnemyDataBufferPoint].Y);
        ForcastTime = distance * 1000 / GUNSpeed;
    }
    
    if(ForcastCore(SampleTime, ForcastTime, &ForcastPoint) == 0)        //������ϳɹ�
    {
        //Ŀ���ת��ΪĿ��Ƕ�
        *ForcastAngle = RecToPolar(ForcastPoint.X, 
                                    ForcastPoint.Y, 
                                    ForcastPoint.Z,
                                    Position.Euler.Pitch,
                                    CloudParam.Pitch.RealEncoderAngle,
                                    1);
        
        return 0;
    }
    //�������ʧ��
    else
    {
        //���ʧ��ֱ��ʹ�õ�ǰλ����ΪĿ��λ��
        *ForcastAngle = RecToPolar(EnemyDataBuffer[EnemyDataBufferPoint].X, 
                                    EnemyDataBuffer[EnemyDataBufferPoint].Y, 
                                    EnemyDataBuffer[EnemyDataBufferPoint].Z,
                                    Position.Euler.Pitch,
                                    CloudParam.Pitch.RealEncoderAngle,
                                    1);
        
        return 1;
    }
}
    
    
/**
  * @brief  ��PC��ָ��
  * @param  ָ��
  * @retval void
  */
void SendPCOrder(PCOrder_Enum order)
{
//    if(DMA2_Stream7->NDTR == 0)
//    {
        if(PCOrder_Attack == order)
        {
            DMA_Cmd(DMA2_Stream7, DISABLE);                     //�ر� DMA ����
            if (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE)      //ȷ�� DMA ���Ա�����
            {
                return;
            }                
            DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7);       //��ձ�־λ
            DMA2_Stream7->M0AR = (uint32_t)AttackModeOrder;     //��������
            DMA_SetCurrDataCounter(DMA2_Stream7, PCDATALENGTH); //���ݴ�����
            DMA_Cmd(DMA2_Stream7, ENABLE);                      //���� DMA ����
        }
        else if(PCOrder_BigSample == order)
        {
            DMA_Cmd(DMA2_Stream7, DISABLE);                     //�ر� DMA ����
            if (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE)      //ȷ�� DMA ���Ա�����
            {
                return;
            }            
            DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7);       //��ձ�־λ
            DMA2_Stream7->M0AR = (uint32_t)BigSymbolModeOrder;  //��������
            DMA_SetCurrDataCounter(DMA2_Stream7, PCDATALENGTH); //���ݴ�����
            DMA_Cmd(DMA2_Stream7, ENABLE);                      //���� DMA ����
        }
        else if(PCOrder_Shutdown == order)
        {
            DMA_Cmd(DMA2_Stream7, DISABLE);                     //�ر� DMA ����
            if (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE)      //ȷ�� DMA ���Ա�����
            {
                return;
            }            
            DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7);       //��ձ�־λ
            DMA2_Stream7->M0AR = (uint32_t)ShutdownOrder;       //��������
            DMA_SetCurrDataCounter(DMA2_Stream7, PCDATALENGTH); //���ݴ�����
            DMA_Cmd(DMA2_Stream7, ENABLE);                      //���� DMA ����
        }
//    }
}


/**
  * @brief  ����Ŀ����ɫ
  * @param  'R' ��ɫ      'B' ��ɫ      ���� ��ɫ
  * @retval void
  */
void SendEnemyColor(char Co)
{
    if('B' == Co)
    {
        DMA_Cmd(DMA2_Stream7, DISABLE);                     //�ر� DMA ����
        if (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE)      //ȷ�� DMA ���Ա�����
        {
            return;
        }            
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7);       //��ձ�־λ
        DMA2_Stream7->M0AR = (uint32_t)BlueEnemyOrder;      //��������
        DMA_SetCurrDataCounter(DMA2_Stream7, PCDATALENGTH); //���ݴ�����
        DMA_Cmd(DMA2_Stream7, ENABLE);                      //���� DMA ����
    }
    else
    {
        DMA_Cmd(DMA2_Stream7, DISABLE);                     //�ر� DMA ����
        if (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE)      //ȷ�� DMA ���Ա�����
        {
            return;
        }            
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7);       //��ձ�־λ
        DMA2_Stream7->M0AR = (uint32_t)RedEnemyOrder;       //��������
        DMA_SetCurrDataCounter(DMA2_Stream7, PCDATALENGTH); //���ݴ�����
        DMA_Cmd(DMA2_Stream7, ENABLE);                      //���� DMA ����
    }
}






