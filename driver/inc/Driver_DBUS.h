#ifndef __DRIVER_DBUS_H
#define __DRIVER_DBUS_H
#include "main.h"

//DBUS��������֡����
#define DBUSLength		18				//DBUS����֡��
#define DBUSBackLength	1				//����һ���ֽڱ����ȶ�

typedef struct{
	int16_t ch1;          //���ֵ��1684  ��Сֵ��364  �м�ֵ��1024
	int16_t ch2;
	int16_t ch3;
	int16_t ch4;
	
	uint8_t switch_left;  // �ϣ�1 �£�2 �У�3
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
   * ����ͨ��:15   14   13   12   11   10   9   8   7   6     5     4   3   2   1
   *          V    C    X	  Z    G    F    R   E   Q  CTRL  SHIFT  D   A   S   W
************************************************************************************/
		uint16_t key_code;
		uint16_t jumpkey_code;
	}keyBoard;

}DBUSDecoding_Type;


extern uint8_t DBUSBuffer[DBUSLength + DBUSBackLength];				//DBUS,DMA���ջ���
extern DBUSDecoding_Type DBUS_ReceiveData, LASTDBUS_ReceiveData;			//DBUS����������
extern uint8_t DBUSFrameCounter;					//DBUS֡����
extern uint8_t DBUSFrameRate;					    //DBUS֡��

void DBUS_DataDecoding(void);

#endif
