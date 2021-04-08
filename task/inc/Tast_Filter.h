#ifndef __TASK_FILTER_H__
#define __TASK_FILTER_H__

#include "main.h"

float first_order_filter(float *filter,float filter_in ,float filter_out1, u16 filter_parameter);
float GetMedianNum(float * bArray, int iFilterLen, float mean_in);
float glide_filter(float value_buf[],float ADNum);
float first_order_filter_gpyo(float *filter,float filter_in ,float filter_out, u16 filter_parameter);
int isNaN(float f);
	
#endif
