
#ifndef __TASN_CANSEND_H
#define __TASN_CANSEND_H

#include "main.h"
#include "OSinclude.h"

typedef struct
{
    uint8_t     CANx;               //CAN���     1 CAN1      2 CAN2
    CanTxMsg    SendCanTxMsg;       //��������
}CanSend_Type;


void Task_CANSend(void *Parameters);


#endif
