/*
	*Author: //作者
	*Version: v1.0
	*Date: 	2021/4/27
	*Description:nrf2401软件驱动
	*History:	2021/4/27	完成基础功能
				2021/5/6	完善备注
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
		SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR, rxAddr, TX_ADR_WIDTH);//设置TX_ADDR地址 
		SPI_NRF_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0, rxAddr, RX_ADR_WIDTH);//设置RX节点地址,主要为了使能ACK   
		SPI_NRF_WriteReg(NRF_WRITE_REG+EN_AA, 0x01);     //使能通道0的自动应答    
		SPI_NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR, 0x01); //使能通道0的接收地址 
		//SPI_NRF_WriteReg(NRF_WRITE_REG+SETUP_AW, 0x03);//地址为5位的
		//SPI_NRF_WriteReg(NRF_WRITE_REG+SETUP_RETR, 0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
		SPI_NRF_WriteReg(NRF_WRITE_REG+RF_CH, 0);       //设置RF通道为CHANAL
		SPI_NRF_WriteReg(NRF_WRITE_REG+RX_PW_P0, nrf_init->datLen);//选择通道0的有效数据宽度      
		SPI_NRF_WriteReg(NRF_WRITE_REG+RF_SETUP, 0x07);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
		SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG, 0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发射模式,开启所有中断
	}else{
		SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,rxAddr,TX_ADR_WIDTH);
		SPI_NRF_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0, rxAddr, RX_ADR_WIDTH);//设置RX_ADDR_P0的地址
		SPI_NRF_WriteReg(NRF_WRITE_REG+EN_AA, 0x01);    //使能通道0的自动应答    
		SPI_NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR, 0x01);//使能通道0的接收地址    
		//SPI_NRF_WriteReg(NRF_WRITE_REG+SETUP_AW, 0x03);//地址为5位的   3
		SPI_NRF_WriteReg(NRF_WRITE_REG+RF_CH, nrf_init->channel); //设置RF通信频率    
		SPI_NRF_WriteReg(NRF_WRITE_REG+RX_PW_P0, nrf_init->datLen);//选择通道的有效数据宽度      
		SPI_NRF_WriteReg(NRF_WRITE_REG+RF_SETUP, 0x07); //F设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
		SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG, 0x0f);  //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
	}
	
	NRF2401_Delay(120);
	//NRF_CE_OUT(1);
}


/*
 * 函数名：NRF_Check
 * 描述  ：主要用于NRF与MCU是否正常连接
 * 输入  ：无	
 * 输出  ：1/0 连接正常/连接失败
 * 调用  ：外部调用
 */
uint8_t NRF_Check(void){
	uint8_t buf[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
	uint8_t buf1[5];
	uint8_t i=0; 
	
	NRF_CSN_OUT(1);
	
	/*写入5个字节的地址.  */  
	SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,buf,5);

	/*读出写入的地址 */
	SPI_NRF_ReadBuf(TX_ADDR,buf1,5); 
		
	/*比较*/               
	for(i=0;i<5;i++){
		if(buf1[i]!=0xC2)
			break;
	} 
	       
	if(i==5){
		return 1 ;        //MCU与NRF成功连接 
	}else{
		return 0 ;        //MCU与NRF不正常连接
	}
}

/*
* 函数名：NRF_Tx_Dat
* 描述  ：用于向NRF的发送缓冲区中写入数据
* 输入  ：txBuf：存储了将要发送的数据的数组，外部定义	
* 输出  ：发送结果，成功返回TXDS,失败返回MAXRT或ERROR
* 调用  ：外部调用
*/ 
uint8_t NRF_Tx_Dat(uint8_t *txbuf, uint8_t sendLen)
{
	uint8_t state;
  
	/*ce为低，进入待机模式1*/
	NRF_CE_OUT(0);
	// 装载接收端地址
	SPI_NRF_WriteBuf(NRF_WRITE_REG + RX_ADDR_P0, rxAddr, sendLen); 
	/*写数据到TX BUF 最大 32个字节*/						
	SPI_NRF_WriteBuf(WR_TX_PLOAD,txbuf,sendLen);

	/*CE为高，txbuf非空，发送数据包 */   
	NRF_CE_OUT(1);
	  	
	while(NRF_Read_IRQ()!=0); //可以修改成中断处理
	NRF2401_Delay(30);
		     
	NRF2401_Delay(2000);
	state = SPI_NRF_ReadReg(STATUS);
  if(state&0x10==0x10)
	/*清除TX_DS或MAX_RT中断标志*/                  
	SPI_NRF_WriteReg(NRF_WRITE_REG+STATUS,NOP); 	
	//清除TX FIFO寄存器 
	SPI_NRF_WriteReg(FLUSH_TX,NOP);    
	NRF2401_Delay(2000);
	
	return state;
}

/*
 * 函数名：NRF_Rx_Dat
 * 描述  ：用于从NRF的接收缓冲区中读出数据
 * 输入  ：rxBuf：用于接收该数据的数组，外部定义	
 * 输出  ：接收结果，
 * 调用  ：外部调用
 */ 
uint8_t NRF_Rx_Dat(uint8_t *rxbuf, uint8_t recvLen)
{
	uint8_t state; 
	NRF_CE_OUT(0);	
	NRF2401_Delay(1); 
	NRF_CE_OUT(1);	
	NRF2401_Delay(140); //进入接收状态
//	 /*等待接收中断*/
	while(NRF_Read_IRQ()!=0); 
	
	/*读取status寄存器的值  */               
	state=SPI_NRF_ReadReg(STATUS);
	 
	/* 清除中断标志*/      
	SPI_NRF_WriteReg(NRF_WRITE_REG+STATUS,state);

	/*判断是否接收到数据*/
	if(state&RX_DR){                                 //接收到数据
		NRF_CE_OUT(0);
	  SPI_NRF_ReadBuf(RD_RX_PLOAD,rxbuf,recvLen);//读取数据
		SPI_NRF_WriteReg(FLUSH_RX,NOP);          //清除RX FIFO寄存器
	  return RX_DR; 
	}else{    
		return 0;                    //没收到任何数据
	}
}


