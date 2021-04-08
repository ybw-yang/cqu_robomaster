#include "Tast_Filter.h"

float filter_d=0;
float first_order_filter(float *filter,float filter_in ,float filter_out, u16 filter_parameter)
{
	filter[0] = filter[1];
	filter[1] = filter_in;
	filter_d = filter[1]-filter[0];
	if(filter_d > filter_parameter || filter_d < -filter_parameter)	filter_d=0;
	filter_out = filter_d + filter_out ; //Ò»½×ÂË²¨ÂË¼â·åÔëÉù
	return filter_out;
}

float filter_d_gpyo=0;
float first_order_filter_gpyo(float *filter,float filter_in ,float filter_out, u16 filter_parameter)
{
	filter[0] = filter[1];
	filter[1] = filter_in;
	filter_d_gpyo = filter[1]-filter[0];
	if(filter_d_gpyo > filter_parameter || filter_d_gpyo < -filter_parameter)	filter_d_gpyo=0;
	filter_out = filter_d_gpyo + filter_out ; //Ò»½×ÂË²¨ÂË¼â·åÔëÉù
	return filter_out;
}

float GetMedianNum(float * bArray, int iFilterLen ,float mean_in)
{  
	int i,j,ni,k; 
	float bTemp; 
	float temp[6]={0};
	for (ni = 0; ni < iFilterLen-1; ni ++)
		bArray[ni] = bArray[ni+1];

	bArray[iFilterLen-1] = mean_in;
	for(k=0;k<iFilterLen;k++)temp[k] = bArray[k];
    for (j = 0; j < iFilterLen - 1; j ++)  {  
        for (i = 0; i < iFilterLen - j - 1; i ++)  {  
            if (temp[i] > temp[i + 1])  {  
                bTemp = temp[i];  
                temp[i] = temp[i + 1];  
                temp[i + 1] = bTemp;  
            }  
        }  
    }  
       
    if ((iFilterLen & 1) > 0)  
        bTemp = temp[(iFilterLen + 1) / 2];  
    else  
        bTemp = (temp[iFilterLen / 2] + temp[iFilterLen / 2 + 1]) / 2;    
    return bTemp;  
}

float glide_filter(float value_buf[],float ADNum){ 
  float sum=0;
	int mi,count;
	for (mi = 0; mi < 5; mi ++)
			value_buf[mi] = value_buf[mi+1];
	value_buf[5] = ADNum;
  for(count=0;count<6;count++)
		sum+=value_buf[count];
  return (sum/6);
}

int isNaN(float f)
{
	return f!=f;
}

