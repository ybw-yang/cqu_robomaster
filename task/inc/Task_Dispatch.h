
#ifndef __TASK_DISPATCH_H
#define __TASK_DISPATCH_H

#include "OSinclude.h"

extern xQueueHandle  Queue_CANSend;

void start_task(void *pvParameters);


#endif
