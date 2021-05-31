/*
	*Author: //作者
	*Version: v1.0
	*Date: 	2021/4/27
	*Description:模拟SPI驱动
	*History:	2021/4/27	完成基础功能
				2021/5/6	完善备注
				2021/5/24	完善单双工通信 只读或者只发
*/
#include "software_spi.h"


void InitSPI(void){
	return;
}

/*
 * 函数名：Master_SPI_WR_Data
 * 描述  ：对Nrf进行读写
 * 输入  ：dat:将要写入的数据
 * 输出  ：写每位数据时的状态
 * 调用  ：内部调用
 */
uint8_t Master_SPI_WR_Data(uint8_t dat)//可以一边发一边收
{  	
	uint8_t count = 0;
	//NRF_CSN_OUT(0);
	/*每一次循环写一位数据,并且获取一位数据*/
	for(count = 0; count < 8; count++){
		
		SPI_MOSI_OUT(dat & 0x80);
		dat <<= 1;
		SPI_CLK_OUT(1);
		dat |= SPI_MISO_IN();	
		SPI_CLK_OUT(0);
	}
	//NRF_CSN_OUT(1);

	return dat;
}

/*
 * 函数名：Master_SPI_Read_Data
 * 描述  ：对设备进行读
 * 输入  ：
 * 输出  ：读出的数据
 * 调用  ：内部调用
 */
 
uint8_t Master_SPI_Read_Data(void){
	uint8_t count=0;
	uint8_t dat=0;
	for(count=0;count<8;count++){
		SPI_CLK_OUT(1);
		data<<=1;
		if(SPI_MISO_IN()){
			data=data|0x01;
		}
		SPI_CLK_OUT(0);
	}
	return dat;
}

/*
 * 函数名：Master_SPI_Write_Data
 * 描述  ：对设备进行写
 * 输入  ：数据8bit
 * 输出  ：无
 * 调用  ：内部调用
 */
 
void Master_SPI_Write_Data(uint8_t dat){
	uint8_t count=0;
	for(count=0;count<8;count++){
		if(dat&0x80){
			SPI_MOSI_OUT(1);
		}else{
			SPI_MOSI_OUT(0);
		}
		SPI_CLK_OUT(1);
		data<<=1;
		SPI_CLK_OUT(0);
	}
	return dat;
}




