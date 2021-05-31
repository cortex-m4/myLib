/*
	*Author: //����
	*Version: v1.0
	*Date: 	2021/4/27
	*Description:nrf2401�������
	*History:	2021/4/27	��ɻ�������
				2021/5/6	���Ʊ�ע
*/
#include "nrf2401.h"
#include "main.h"

uint8_t rxAddr[]={0x31, 0x32, 0x33, 0x34, 0x35};

static void NRF2401_Delay(uint16_t nCount){
	for(; nCount != 0; nCount--){
	
	}
}


void NRF2401_Init(NRF2401_InitTypeDef *nrf_init){
	uint8_t ret=0;
	NRF2401_Delay(100);
	

	NRF2401_HW_Init();
	NRF_CE_OUT(0);

	ret=NRF_Check();
	if(ret){
		LED=0;
		printf("nrf2401 init success!\r\n");
	}else{
		LED=1;
		printf("nrf2401 init failed!\r\n");
		return ;
	}
	
	if(nrf_init->mod==NRF2401_TX_MOD){
		SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR, rxAddr, TX_ADR_WIDTH);//����TX_ADDR��ַ 
		SPI_NRF_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0, rxAddr, RX_ADR_WIDTH);//����RX�ڵ��ַ,��ҪΪ��ʹ��ACK   
		SPI_NRF_WriteReg(NRF_WRITE_REG+EN_AA, 0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
		SPI_NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR, 0x01); //ʹ��ͨ��0�Ľ��յ�ַ 
		//SPI_NRF_WriteReg(NRF_WRITE_REG+SETUP_AW, 0x03);//��ַΪ5λ��
		//SPI_NRF_WriteReg(NRF_WRITE_REG+SETUP_RETR, 0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
		SPI_NRF_WriteReg(NRF_WRITE_REG+RF_CH, 0);       //����RFͨ��ΪCHANAL
		SPI_NRF_WriteReg(NRF_WRITE_REG+RX_PW_P0, nrf_init->datLen);//ѡ��ͨ��0����Ч���ݿ��      
		SPI_NRF_WriteReg(NRF_WRITE_REG+RF_SETUP, 0x07);  //����TX�������,0db����,2Mbps,���������濪��   
		SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG, 0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	}else{
		SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,rxAddr,TX_ADR_WIDTH);
		SPI_NRF_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0, rxAddr, RX_ADR_WIDTH);//����RX_ADDR_P0�ĵ�ַ
		SPI_NRF_WriteReg(NRF_WRITE_REG+EN_AA, 0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
		SPI_NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR, 0x01);//ʹ��ͨ��0�Ľ��յ�ַ    
		//SPI_NRF_WriteReg(NRF_WRITE_REG+SETUP_AW, 0x03);//��ַΪ5λ��   3
		SPI_NRF_WriteReg(NRF_WRITE_REG+RF_CH, nrf_init->channel); //����RFͨ��Ƶ��    
		SPI_NRF_WriteReg(NRF_WRITE_REG+RX_PW_P0, nrf_init->datLen);//ѡ��ͨ������Ч���ݿ��      
		SPI_NRF_WriteReg(NRF_WRITE_REG+RF_SETUP, 0x07); //F����TX�������,0db����,2Mbps,���������濪��   
		SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG, 0x0f);  //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
	}
	
	NRF2401_Delay(120);
	//NRF_CE_OUT(1);
}


/*
 * ��������NRF_Check
 * ����  ����Ҫ����NRF��MCU�Ƿ���������
 * ����  ����	
 * ���  ��1/0 ��������/����ʧ��
 * ����  ���ⲿ����
 */
uint8_t NRF_Check(void){
	uint8_t buf[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
	uint8_t buf1[5];
	uint8_t i=0; 
	
	NRF_CSN_OUT(1);
	
	/*д��5���ֽڵĵ�ַ.  */  
	SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,buf,5);

	/*����д��ĵ�ַ */
	SPI_NRF_ReadBuf(TX_ADDR,buf1,5); 
		
	/*�Ƚ�*/               
	for(i=0;i<5;i++){
		if(buf1[i]!=0xC2)
			break;
	} 
	       
	if(i==5){
		return 1 ;        //MCU��NRF�ɹ����� 
	}else{
		return 0 ;        //MCU��NRF����������
	}
}

/*
* ��������NRF_Tx_Dat
* ����  ��������NRF�ķ��ͻ�������д������
* ����  ��txBuf���洢�˽�Ҫ���͵����ݵ����飬�ⲿ����	
* ���  �����ͽ�����ɹ�����TXDS,ʧ�ܷ���MAXRT��ERROR
* ����  ���ⲿ����
*/ 
uint8_t NRF_Tx_Dat(uint8_t *txbuf, uint8_t sendLen)
{
	uint8_t state;
  
	/*ceΪ�ͣ��������ģʽ1*/
	NRF_CE_OUT(0);
	// װ�ؽ��ն˵�ַ
	SPI_NRF_WriteBuf(NRF_WRITE_REG + RX_ADDR_P0, rxAddr, sendLen); 
	/*д���ݵ�TX BUF ��� 32���ֽ�*/						
	SPI_NRF_WriteBuf(WR_TX_PLOAD,txbuf,sendLen);

	/*CEΪ�ߣ�txbuf�ǿգ��������ݰ� */   
	NRF_CE_OUT(1);
	  	
	while(NRF_Read_IRQ()!=0); //�����޸ĳ��жϴ���
	NRF2401_Delay(30);
		     
	NRF2401_Delay(2000);
	state = SPI_NRF_ReadReg(STATUS);
  if(state&0x10==0x10)
	/*���TX_DS��MAX_RT�жϱ�־*/                  
	SPI_NRF_WriteReg(NRF_WRITE_REG+STATUS,NOP); 	
	//���TX FIFO�Ĵ��� 
	SPI_NRF_WriteReg(FLUSH_TX,NOP);    
	NRF2401_Delay(2000);
	
	return state;
}

/*
 * ��������NRF_Rx_Dat
 * ����  �����ڴ�NRF�Ľ��ջ������ж�������
 * ����  ��rxBuf�����ڽ��ո����ݵ����飬�ⲿ����	
 * ���  �����ս����
 * ����  ���ⲿ����
 */ 
uint8_t NRF_Rx_Dat(uint8_t *rxbuf, uint8_t recvLen)
{
	uint8_t state; 
	NRF_CE_OUT(0);	
	NRF2401_Delay(1); 
	NRF_CE_OUT(1);	
	NRF2401_Delay(140); //�������״̬
//	 /*�ȴ������ж�*/
	while(NRF_Read_IRQ()!=0); 
	
	/*��ȡstatus�Ĵ�����ֵ  */               
	state=SPI_NRF_ReadReg(STATUS);
	 
	/* ����жϱ�־*/      
	SPI_NRF_WriteReg(NRF_WRITE_REG+STATUS,state);

	/*�ж��Ƿ���յ�����*/
	if(state&RX_DR){                                 //���յ�����
		NRF_CE_OUT(0);
	  SPI_NRF_ReadBuf(RD_RX_PLOAD,rxbuf,recvLen);//��ȡ����
		SPI_NRF_WriteReg(FLUSH_RX,NOP);          //���RX FIFO�Ĵ���
	  return RX_DR; 
	}else{    
		return 0;                    //û�յ��κ�����
	}
}


