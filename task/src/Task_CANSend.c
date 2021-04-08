#include "main.h"
#include "OSinclude.h"


/**
  * @brief  CAN��������
  * @param  unused
  * @retval void
  */
void Task_CANSend(void *Parameters)
{
    CanSend_Type    CANSendData;
    
    for(;;)
    {
			xQueueReceive(Queue_CANSend, &CANSendData, portMAX_DELAY);
        
         if(CANSendData.CANx == 1)
        {
            
//            do
//            {
//                //CAN��������
//                if(CAN1->ESR)
//                {
//                    CAN1->MCR |= 0x02;
//                    CAN1->MCR &= 0xFD;
//                }
//            }while(!(CAN1->TSR & 0x1C000000));
            
            CAN_Transmit(CAN1, &(CANSendData.SendCanTxMsg));
        }
        else
        {
            do
            {
                //CAN��������
                if(CAN2->ESR)
                {
                    CAN2->MCR |= 0x02;
                    CAN2->MCR &= 0xFD;
                }
            }while(!(CAN2->TSR & 0x1C000000));
            
            CAN_Transmit(CAN2, &(CANSendData.SendCanTxMsg));
        }
        
    }
}
