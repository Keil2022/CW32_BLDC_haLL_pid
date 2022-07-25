#ifndef __OLED_IIC_H
#define __OLED_IIC_H

#include "config.h"
#include "cw32f030_i2c.h"

#define COM 0X00
#define DAT 0X40

void OLED_I2C_Init(void);
void I2C_MasterWriteByte(I2C_TypeDef *I2Cx,uint8_t u8Addr,uint8_t *pu8Data,uint32_t u32Len);
void WriteCmd(uint8_t I2C_Command);
void WriteDat(uint8_t I2C_Data);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_Init(void);

void OLED_DrawPoint(uint8_t x, uint8_t y);
void OLED_ClearPoint(uint8_t x, uint8_t y);
void OLED_ClearLine(uint8_t x1, uint8_t y1, int8_t x2, uint8_t y2);
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size1);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t size1);
uint32_t OLED_Pow(uint8_t m,uint8_t n);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size1);
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t num, uint8_t size1);
void OLED_WR_BP(uint8_t x, uint8_t y);
void OLED_ShowPicture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]);

void OLED_Play_1(void);

#endif

