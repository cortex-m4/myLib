#include "led.h"


void LED_Con(LED_Status sta){
	switch(sta){
		case LED_OFF:HAL_GPIO_WritePin(LED_GPIO_PORT,LED_GPIO_PIN,GPIO_PIN_RESET);break;
		case LED_ON:HAL_GPIO_WritePin(LED_GPIO_PORT,LED_GPIO_PIN,GPIO_PIN_SET);break;
		case LED_TOGGLE:HAL_GPIO_TogglePin(LED_GPIO_PORT,LED_GPIO_PIN);break;
		default:HAL_GPIO_WritePin(LED_GPIO_PORT,LED_GPIO_PIN,GPIO_PIN_RESET);break;
	}
}

