/*
	*Author: //����
	*Version: v1.0
	*Date: 	2021/4/27
	*Description:nrf2401�ӿ�
	*History:	2021/4/27	��������
				2021/5/6	���Ʊ�ע
*/
#include "nrf2401_port.h"
#include "utils_def.h"

void NRF2401_HW_Init(void){

	NRF_CSN_OUT(1);
	NRF_CLK_OUT(0);
	InitSPI();	
}


/*
	SPI_NRF ���Ĵ����ӿ�
	
*/
uint8_t SPI_NRF_ReadReg(uint8_t reg){
	
	uint8_t reg_val;
	
	Master_SPI_WR_Data(reg);
	reg_val=Master_SPI_WR_Data(0xff);
	return reg_val;
	
}
/*
	SPI_NRF д�Ĵ����ӿ�
	����ֵ���Ĵ�����ֵ
*/
uint8_t SPI_NRF_WriteReg(uint8_t reg,uint8_t dat){
	
	uint8_t status;
	NRF_CSN_OUT(0);
	status=Master_SPI_WR_Data(reg);
	Master_SPI_WR_Data(dat);
	NRF_CSN_OUT(1);
	return status;
	
}
/*
	SPI_NRF ��buf	�ӿ�
*/
uint8_t SPI_NRF_ReadBuf(uint8_t reg,uint8_t *pBuf,uint8_t bytes){
	
	uint8_t status,i;
	
	NRF_CSN_OUT(0);
	status=Master_SPI_WR_Data(reg);
	for(i=0;i<bytes;i++){
		pBuf[i]=Master_SPI_WR_Data(0xff);
	}
	NRF_CSN_OUT(1);
	return status;
}
/*
	SPI_NRF дbuf	�ӿ�
*/
uint8_t SPI_NRF_WriteBuf(uint8_t reg,uint8_t *pBuf,uint8_t bytes){
	uint8_t status,i;
	
	NRF_CSN_OUT(0);
	status=Master_SPI_WR_Data(reg);
	for(i=0;i<bytes;i++){
		Master_SPI_WR_Data(*pBuf++);
	}
	NRF_CSN_OUT(1);
	return status;
}

