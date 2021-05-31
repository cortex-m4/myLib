/*
	*Author: //����
	*Version: v1.0
	*Date: 	2021/4/27
	*Description:ģ��SPI����
	*History:	2021/4/27	��ɻ�������
				2021/5/6	���Ʊ�ע
				2021/5/24	���Ƶ�˫��ͨ�� ֻ������ֻ��
*/
#include "software_spi.h"


void InitSPI(void){
	return;
}

/*
 * ��������Master_SPI_WR_Data
 * ����  ����Nrf���ж�д
 * ����  ��dat:��Ҫд�������
 * ���  ��дÿλ����ʱ��״̬
 * ����  ���ڲ�����
 */
uint8_t Master_SPI_WR_Data(uint8_t dat)//����һ�߷�һ����
{  	
	uint8_t count = 0;
	//NRF_CSN_OUT(0);
	/*ÿһ��ѭ��дһλ����,���һ�ȡһλ����*/
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
 * ��������Master_SPI_Read_Data
 * ����  �����豸���ж�
 * ����  ��
 * ���  ������������
 * ����  ���ڲ�����
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
 * ��������Master_SPI_Write_Data
 * ����  �����豸����д
 * ����  ������8bit
 * ���  ����
 * ����  ���ڲ�����
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




