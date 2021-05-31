#include "hc595.h"
#include "utils_def.h"
//送出数据
void HC595SendData(uint8_t SendVal)
{
	uint8_t i;
	_SRCK(0);
	for(i = 0; i < 8; i++)  
	{ 
		_SCK(0);  
		_DATA(SendVal & 0x01);	
		_SCK(1);                    
		SendVal >>= 1;		
	}
	_SRCK(1);
	
}


