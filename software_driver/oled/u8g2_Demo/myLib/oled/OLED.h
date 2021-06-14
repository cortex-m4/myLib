#ifndef			__OLED_H__
#define 		__OLED_H__
#include "main.h"
#include "software_i2c.h"
#include "codetab.h"

#define OLED_ADDRESS	0x78
#define OLED_I2C_WR	0		/* д����bit */
#define OLED_I2C_RD	1		/* ������bit */

uint8_t OLED_CheckDevice(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);

#ifdef	USE_U8G2
#include "u8g2.h"
#include "u8x8.h"

uint8_t u8x8_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
    U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
    U8X8_UNUSED void *arg_ptr);
void u8g2Init(u8g2_t *u8g2);
#endif

#endif


