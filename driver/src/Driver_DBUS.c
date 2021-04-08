#include "main.h"

uint8_t DBUSFrameCounter;					//DBUS帧计数
uint8_t DBUSFrameRate;					    //DBUS帧率
uint8_t DBUSBuffer[DBUSLength + DBUSBackLength];	
DBUSDecoding_Type DBUS_ReceiveData, LASTDBUS_ReceiveData;


/**
  * @brief  DBUS数据解码
  * @param  void
  * @retval void
  */
void DBUS_DataDecoding(void)
{
	decode_flag = 1;
	data_decoder();
//	LASTDBUS_ReceiveData = DBUS_ReceiveData;
//	
//	DBUS_ReceiveData.ch1 = (DBUSBuffer[0] | DBUSBuffer[1]<<8) & 0x07FF;
//	DBUS_ReceiveData.ch1 -= 1024;
//	DBUS_ReceiveData.ch2 = (DBUSBuffer[1]>>3 | DBUSBuffer[2]<<5 ) & 0x07FF;
//	DBUS_ReceiveData.ch2 -= 1024;
//	DBUS_ReceiveData.ch3 = (DBUSBuffer[2]>>6 | DBUSBuffer[3]<<2 | DBUSBuffer[4]<<10) & 0x07FF;
//	DBUS_ReceiveData.ch3 -= 1024;
//	DBUS_ReceiveData.ch4 = (DBUSBuffer[4]>>1 | DBUSBuffer[5]<<7) & 0x07FF;		
//	DBUS_ReceiveData.ch4 -= 1024;
//	
//	DBUS_ReceiveData.switch_left = ( (DBUSBuffer[5] >> 4)& 0x000C ) >> 2;
//	DBUS_ReceiveData.switch_right =  (DBUSBuffer[5] >> 4)& 0x0003 ;
//	
//	DBUS_ReceiveData.mouse.x = DBUSBuffer[6] | (DBUSBuffer[7] << 8);	//x axis
//	DBUS_ReceiveData.mouse.y = DBUSBuffer[8] | (DBUSBuffer[9] << 8);
//	DBUS_ReceiveData.mouse.z = DBUSBuffer[10]| (DBUSBuffer[11] << 8);
//	
//	DBUS_ReceiveData.mouse.press_left 	= DBUSBuffer[12];	// is pressed?
//	DBUS_ReceiveData.mouse.press_right 	= DBUSBuffer[13];
//	
//	DBUS_ReceiveData.keyBoard.key_code 	= DBUSBuffer[14] | DBUSBuffer[15] << 8; //key borad code

}
