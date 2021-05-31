/*
	*Author: //作者
	*Version: v1.0
	*Date: 2019/1/21
	*Description://模拟IIC驱动
	*History: 2019/1/21	创建完成文件
				2021/5/6 	优化程序以及备注
				2021/5/29 	添加 i2c_write_byte i2c_transmit_dat 
*/
#include "software_i2c.h"

GPIO_InitTypeDef i2c_gpio_init_t = {0};

void i2c_init(void){
	__HAL_RCC_GPIOD_CLK_ENABLE();
	i2c_gpio_init_t.Pin=i2c_scl_pin|i2c_sda_pin;
	i2c_gpio_init_t.Mode=GPIO_MODE_OUTPUT_PP;
	i2c_gpio_init_t.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(i2c_gpio_port,&i2c_gpio_init_t);
}

static void i2c_delay(void){
	uint8_t i;
	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
		工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	for (i = 0; i < 5; i++);
}

/*
	sda gpio mod选择
	0:output
	1:input
*/
static void i2c_sda_mod(uint8_t mod){
	if(mod){
		i2c_gpio_init_t.Mode=GPIO_MODE_INPUT;
	}else{
		i2c_gpio_init_t.Mode=GPIO_MODE_OUTPUT_PP;
	}
	HAL_GPIO_Init(i2c_gpio_port,&i2c_gpio_init_t);
}


static void i2c_start(void){
		/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
		IIC_SDA_W(1);
		IIC_SCL_W(1);
		i2c_delay();
		IIC_SDA_W(0);
		i2c_delay();
		IIC_SCL_W(0);
		i2c_delay();
}
static void i2c_stop(void){
		IIC_SDA_W(0);
		IIC_SCL_W(1);
		i2c_delay();
		IIC_SDA_W(1);
}
static void i2c_send_data(uint8_t dat){
		uint8_t i;
		i2c_sda_mod(I2C_SDA_OUTPUT);
		for (i = 0; i < 8; i++){
			if (dat& 0x80){
				IIC_SDA_W(1);
			}else{
				IIC_SDA_W(0);
			}
			i2c_delay();
			IIC_SCL_W(1);
			i2c_delay();
			IIC_SCL_W(0);
			dat<<= 1;	/* 左移一个bit */
			i2c_delay();
		}
		IIC_SDA_W(1); // 释放总线
}
static uint8_t i2c_read_data(void){
		uint8_t i;
		uint8_t value=0;
		i2c_sda_mod(I2C_SDA_INPUT);
		for(i=0;i<8;i++){
			value<<=1;
			IIC_SCL_W(1);
			i2c_delay();
			if(IIC_SDA_R){
				value|=0x01;
			}
			IIC_SCL_W(0);
			i2c_delay();
		}
		return value;
}
/*
	返回值 0 表示正确， 返回1表示未探测到
*/
static uint8_t i2c_wait_ack(void){
		uint8_t re;
		i2c_sda_mod(I2C_SDA_OUTPUT);
		IIC_SDA_W(1);
		i2c_delay();
		IIC_SCL_W(1);
		i2c_delay();
		i2c_sda_mod(I2C_SDA_INPUT);
		if(IIC_SDA_R)re=1;
		else re=0;
		i2c_sda_mod(I2C_SDA_OUTPUT);
		IIC_SCL_W(0);
		i2c_delay();
		return re;
}

static void i2c_ack(uint8_t n){//n=1 ack    n=0  not ack
		i2c_sda_mod(I2C_SDA_OUTPUT);
		if(n){
			IIC_SDA_W(0);
		}else{
			IIC_SDA_W(1);
		}
		i2c_delay();
		IIC_SCL_W(1);
		i2c_delay();
		IIC_SCL_W(0);
		i2c_delay();
		IIC_SDA_W(1);
}
/**
* @brief  i2c_transmit_dat，i2c 发送数据
* @param  	hw_addr：	设备地址
*			addr		寄存器地址
			dat			数据
			len			长度
* @retval 0 成功  1失败
*/
uint8_t i2c_transmit_dat(uint8_t *dat,uint8_t len){
	
	uint8_t i=0;
	
	i2c_start();//开启I2C总线
	
	for(i=0;i<len;i++){
		i2c_send_data(*dat);
		dat++;
		/*等待ACK */
		if (i2c_wait_ack() != 0)
		{
			goto cmd_fail;	/* 器件无应答 */
		}
	}
 /* 发送I2C总线停止信号 */
	i2c_stop();
	return 0;
	cmd_fail:{ /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
		/* 发送I2C总线停止信号 */
		i2c_stop();
		return 1;
	}
}
/**
* @brief  i2c_write_byte，i2c 发送一个byte
* @param  	hw_addr：设备地址
			addr：寄存器地址
*			dat：要写入的数据
* @retval 无
*/
uint8_t i2c_write_byte(uint8_t hw_addr,uint8_t addr,uint8_t dat){
	uint8_t buf[3];
	buf[0]=hw_addr;
	buf[1]=addr;
	buf[2]=dat;
	return i2c_transmit_dat(buf,3);
}


