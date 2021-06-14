#ifndef	__LIB_KEY_H__
#define __LIB_KEY_H__
#include "main.h"


#define KEY1_ON		(0x000D)	//enter
#define KEY2_ON		(0x013B)	//up
#define KEY3_ON		(0x013D)	//down
#define KEY4_ON		(0x013C)	//right

#define KEY1_IN		HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)
#define KEY2_IN		HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)
#define KEY3_IN		HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin)
#define KEY4_IN		HAL_GPIO_ReadPin(KEY4_GPIO_Port,KEY4_Pin)


uint16_t KeysCan(void);



#endif


