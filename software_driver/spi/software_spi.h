#ifndef	__software_spi_h
#define __software_spi_h


#define SPI_CLK_OUT(n)		if(n)	\
							else
							
#define SPI_MOSI_OUT(n)		if(n)	\
							else 
							
#define SPI_MISO_IN()

#define SPI_CSN_OUT(n)		if(n)	\
							else



void InitSPI(void);
uint8_t Master_SPI_WR_Data(uint8_t dat);
uint8_t Master_SPI_Read_Data(void);
void Master_SPI_Write_Data(uint8_t dat);

#endif
