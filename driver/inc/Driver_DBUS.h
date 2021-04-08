#ifndef __DRIVER_DBUS_H
#define __DRIVER_DBUS_H
#include "main.h"

//DBUS接收数据帧长度
#define DBUSLength		18				//DBUS数据帧长
#define DBUSBackLength	1				//增加一个字节保持稳定

typedef struct{
	int16_t ch1;          //最大值：1684  最小值：364  中间值：1024
	int16_t ch2;
	int16_t ch3;
	int16_t ch4;
	
	uint8_t switch_left;  // 上：1 下：2 中：3
	uint8_t switch_right;
	
  struct
	{
		int16_t x;
		int16_t y;
		int16_t z;
		
		uint8_t press_left;
		uint8_t press_right;
	}mouse;
	
	struct
	{
/**********************************************************************************
   * 键盘通道:15   14   13   12   11   10   9   8   7   6     5     4   3   2   1
   *          V    C    X	  Z    G    F    R   E   Q  CTRL  SHIFT  D   A   S   W
************************************************************************************/
		uint16_t key_code;
		uint16_t jumpkey_code;
	}keyBoard;

}DBUSDecoding_Type;


extern uint8_t DBUSBuffer[DBUSLength + DBUSBackLength];				//DBUS,DMA接收缓存
extern DBUSDecoding_Type DBUS_ReceiveData, LASTDBUS_ReceiveData;			//DBUS解码后的数据
extern uint8_t DBUSFrameCounter;					//DBUS帧计数
extern uint8_t DBUSFrameRate;					    //DBUS帧率

void DBUS_DataDecoding(void);

#endif
