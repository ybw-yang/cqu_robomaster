#ifndef __IST8310_H__
#define __IST8310_H__
#include "main.h"

static void IST_Reg_Write_By_MPU(uint8_t addr, uint8_t data);
static uint8_t IST_Reg_Read_By_MPU(uint8_t addr);
static void MPU_Auto_Read_IST_config(uint8_t device_address, uint8_t reg_base_addr, uint8_t data_num);
uint8_t IST8310_Init(void);


#endif
