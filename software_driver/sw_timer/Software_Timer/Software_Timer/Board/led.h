#ifndef	__LED_H__
#define __LED_H__
#include "main.h"


#define LED_GPIO_PORT		GPIOA
#define LED_GPIO_PIN		GPIO_PIN_5
typedef enum{
	LED_OFF=0,
	LED_ON=1,
	LED_TOGGLE=2
}LED_Status;
void LED_Con(LED_Status sta);

#endif

