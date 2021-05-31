/*
	*Author: //作者
	*Version: v1.0
	*Date: 2019/1/21
	*Description://模拟IIC驱动
	*History: 2019/1/21	创建完成文件
				2021/5/6 优化程序以及备注
*/
#ifndef  __I2C_GPIO_H__
#define  __I2C_GPIO_H__

#include "main.h"

#define I2C_WR	0x00
#define I2C_RD	0X01

#define I2C_SDA_INPUT		0X01
#define I2C_SDA_OUTPUT		0X00

#define i2c_gpio_port		GPIOD
											
#define i2c_scl_pin			GPIO_PIN_0
#define i2c_sda_pin			GPIO_PIN_1

#define IIC_SCL_W(a)	if(a) HAL_GPIO_WritePin(i2c_gpio_port,i2c_scl_pin,GPIO_PIN_SET); \
											else  HAL_GPIO_WritePin(i2c_gpio_port,i2c_scl_pin,GPIO_PIN_RESET)
#define IIC_SDA_W(a)	if(a) HAL_GPIO_WritePin(i2c_gpio_port,i2c_sda_pin,GPIO_PIN_SET); \
											else  HAL_GPIO_WritePin(i2c_gpio_port,i2c_sda_pin,GPIO_PIN_RESET)

#define IIC_SDA_R			HAL_GPIO_ReadPin(i2c_gpio_port,i2c_sda_pin)
											
											
											


void i2c_init(void);
uint8_t i2c_transmit_dat(uint8_t *dat,uint8_t len);
uint8_t i2c_write_byte(uint8_t hw_addr,uint8_t addr,uint8_t dat);



#endif


