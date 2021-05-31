/*
	*Author: //����
	*Version: v1.0
	*Date: 2019/1/21
	*Description://ģ��IIC����
	*History: 2019/1/21	��������ļ�
				2021/5/6 	�Ż������Լ���ע
				2021/5/29 	��� i2c_write_byte i2c_transmit_dat 
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
	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
		����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�
  
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
	*/
	for (i = 0; i < 5; i++);
}

/*
	sda gpio modѡ��
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
		/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
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
			dat<<= 1;	/* ����һ��bit */
			i2c_delay();
		}
		IIC_SDA_W(1); // �ͷ�����
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
	����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
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
* @brief  i2c_transmit_dat��i2c ��������
* @param  	hw_addr��	�豸��ַ
*			addr		�Ĵ�����ַ
			dat			����
			len			����
* @retval 0 �ɹ�  1ʧ��
*/
uint8_t i2c_transmit_dat(uint8_t *dat,uint8_t len){
	
	uint8_t i=0;
	
	i2c_start();//����I2C����
	
	for(i=0;i<len;i++){
		i2c_send_data(*dat);
		dat++;
		/*�ȴ�ACK */
		if (i2c_wait_ack() != 0)
		{
			goto cmd_fail;	/* ������Ӧ�� */
		}
	}
 /* ����I2C����ֹͣ�ź� */
	i2c_stop();
	return 0;
	cmd_fail:{ /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
		/* ����I2C����ֹͣ�ź� */
		i2c_stop();
		return 1;
	}
}
/**
* @brief  i2c_write_byte��i2c ����һ��byte
* @param  	hw_addr���豸��ַ
			addr���Ĵ�����ַ
*			dat��Ҫд�������
* @retval ��
*/
uint8_t i2c_write_byte(uint8_t hw_addr,uint8_t addr,uint8_t dat){
	uint8_t buf[3];
	buf[0]=hw_addr;
	buf[1]=addr;
	buf[2]=dat;
	return i2c_transmit_dat(buf,3);
}


