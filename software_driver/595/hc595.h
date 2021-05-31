#ifndef	__HC595_H_
#define __HC595_H_

#include "reg52.h"
#include "utils_def.h"

sbit hc595_srck=P1^1;
sbit hc595_dat=P1^0;
sbit hc595_sck=P1^2;


#define _SRCK(n)	if(n) hc595_srck=1; else hc595_srck=0

#define _SCK(n)		if(n) hc595_sck=1; else hc595_sck=0

#define _DATA(n)	if(n) hc595_dat=1; else hc595_dat=0


void HC595SendData(uint8_t SendVal);

#endif
