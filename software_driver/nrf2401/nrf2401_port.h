#ifndef	__nrf2401_port_h__
#define __nrf2401_port_h__


#include "reg52.h"

#define USE_HW_SPI		0

#if	USE_HW_SPI
#include "c51_hw_spi.h"
#else
#include "software_spi.h"
#endif

sbit nrf_ce_pin=P1^3;
sbit nrf_irq_pin=P3^3;

sbit nrf_mosi_pin=P1^5;
sbit nrf_miso_pin=P1^6;
sbit nrf_clk_pin=P1^7;
sbit nrf_csn_pin=P1^4;


//PIN CONTROL

#define NRF_CSN_OUT(n)	if(n)nrf_csn_pin=1;else nrf_csn_pin=0

#define NRF_CE_OUT(n)		if(n)nrf_ce_pin=1; else nrf_ce_pin=0

#define NRF_Read_IRQ()	nrf_irq_pin

#define NRF_MOSI_OUT(n)	if(n)nrf_mosi_pin=1;else nrf_mosi_pin=0

#define NRF_Read_MISO()	nrf_miso_pin

#define NRF_CLK_OUT(n)	if(n)nrf_clk_pin=1;else nrf_clk_pin=0



void NRF2401_HW_Init(void);
uint8_t SPI_NRF_ReadReg(uint8_t reg);
uint8_t SPI_NRF_WriteReg(uint8_t reg,uint8_t dat);
uint8_t SPI_NRF_ReadBuf(uint8_t reg,uint8_t *pBuf,uint8_t bytes);
uint8_t SPI_NRF_WriteBuf(uint8_t reg,uint8_t *pBuf,uint8_t bytes);

#endif

