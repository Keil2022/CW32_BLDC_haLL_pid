#include "oled_iic.h"
#include "oledfont.h"

uint8_t OLED_GRAM[144][8];

//========================================================================
// 函数: void OLED_I2C_Init(void)
// 描述: OLED IIC初始化函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月2日
//========================================================================
void OLED_I2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;
	
	__RCC_GPIOB_CLK_ENABLE();	//使能PB时钟
	__RCC_I2C1_CLK_ENABLE();	//使能I2C1时钟
	
	PB06_AFx_I2C1SCL();		//打开复用功能
	PB07_AFx_I2C1SDA();
	
	GPIO_InitStruct.Pins = GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;		//I2C必须为开漏
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;	
	GPIO_Init(CW_GPIOB,&GPIO_InitStruct);
	
	I2C_InitStruct.I2C_BaudEn = ENABLE;	//波特率计数器使能 
	I2C_InitStruct.I2C_Baud = 0X08;		//波特率计数器配置
	I2C_InitStruct.I2C_FLT = DISABLE;	//FLT配置
	I2C_InitStruct.I2C_AA = DISABLE;	//ACK配置
	
	I2C1_DeInit();
	I2C_Master_Init(CW_I2C1, &I2C_InitStruct);
	I2C_Cmd(CW_I2C1,ENABLE);
}

//========================================================================
// 函数: void WriteCmd(void)
// 描述: OLED写命令函数.
// 参数: 命令
// 返回: None.
// 版本: V1.0, 2022年6月2日
//========================================================================
void I2C_MasterWriteByte(I2C_TypeDef *I2Cx,uint8_t u8Addr,uint8_t *pu8Data,uint32_t u32Len)
{
    uint8_t u8i=0,u8State;
	I2C_GenerateSTART(I2Cx, ENABLE);
	while(1)
	{
		while(0 == I2C_GetIrq(I2Cx))
		{;}
		u8State = I2C_GetState(I2Cx);
		switch(u8State)
		{
			case 0x08:   //发送完START信号
				I2C_GenerateSTART(I2Cx, DISABLE);
				I2C_Send7bitAddress(I2Cx, I2C_SLAVEADDRESS_OLED,0X00);  //从设备地址发送
				break;
			case 0x18:   //发送完SLA+W信号,ACK已收到
				I2C_SendData(I2Cx,u8Addr);//从设备内存地址发送
				break;
			case 0x28:   //发送完1字节数据：发送EEPROM中memory地址也会产生，发送后面的数据也会产生	
				I2C_SendData(I2Cx,pu8Data[u8i++]);
				break;
			case 0x20:   //发送完SLA+W后从机返回NACK
			case 0x38:    //主机在发送 SLA+W 阶段或者发送数据阶段丢失仲裁  或者  主机在发送 SLA+R 阶段或者回应 NACK 阶段丢失仲裁
				I2C_GenerateSTART(I2Cx, ENABLE);
				break;
			case 0x30:   //发送完一个数据字节后从机返回NACK
				I2C_GenerateSTOP(I2Cx, ENABLE);
				break;
			default:
				break;
		}			
		if(u8i>u32Len)
		{
			I2C_GenerateSTOP(I2Cx, ENABLE);//此顺序不能调换，出停止条件
			I2C_ClearIrq(I2Cx);
			break;
		}
		I2C_ClearIrq(I2Cx);			
	}
}

//========================================================================
// 函数: void WriteCmd(void)
// 描述: OLED写命令函数.
// 参数: 命令
// 返回: None.
// 版本: V1.0, 2022年6月2日
//========================================================================
void WriteCmd(uint8_t I2C_Command)
{
	I2C_MasterWriteByte(CW_I2C1,COM,&I2C_Command,1);
}

//========================================================================
// 函数: void WriteDat(void)
// 描述: OLED写数据函数.
// 参数: 数据
// 返回: None.
// 版本: V1.0, 2022年6月2日
//========================================================================
void WriteDat(uint8_t I2C_Data)
{
	I2C_MasterWriteByte(CW_I2C1,DAT,&I2C_Data,1);
}

//========================================================================
// 函数: void OLED_Display_On(void)
// 描述: OLED打开显示.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月2日
//========================================================================
void OLED_Display_On(void)
{
	WriteCmd(0x8D);	//电荷泵配置使能
	WriteCmd(0x14);	//开启电荷泵
	WriteCmd(0xAF);	//开启显示
}

//========================================================================
// 函数: void OLED_Display_Off(void)
// 描述: OLED关闭显示.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月2日
//========================================================================
void OLED_Display_Off(void)
{
	WriteCmd(0x8D);	//电荷泵配置使能
	WriteCmd(0x10);	//关闭电荷泵
	WriteCmd(0xAE);	//关闭显示	
}

//========================================================================
// 函数: void OLED_Refresh(void)
// 描述: OLED刷新显存函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月2日
//========================================================================
void OLED_Refresh(void)
{
	uint8_t n,i;
	for(i=0; i<8; i++)
	{
		WriteCmd(0xb0+i);	//设置页地址，0xb0是首页，一页有8行
		WriteCmd(0x00);		//设置起始列地址低四位
		WriteCmd(0x10);		//设置起始列地址高四位
		for(n=0; n<128; n++)
			WriteDat(OLED_GRAM[n][i]);
	}
}

//========================================================================
// 函数: void OLED_Clear(void)
// 描述: OLED清屏函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月2日
//========================================================================
void OLED_Clear(void)
{
	uint8_t n,i;
	for(i=0; i<8; i++)
	{
		for(n=0; n<128; n++)
		{
			OLED_GRAM[n][i] = 0x00;
		}
	}
	
	OLED_Refresh();	//更新显示
}

//========================================================================
// 函数: void OLED_Init(void)
// 描述: OLED初始化函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月2日
//========================================================================
void OLED_Init(void)
{
	WriteCmd(0xAE);//关闭显示
	WriteCmd(0x00);//设置低列地址
	WriteCmd(0x10);//设置高列地址
	WriteCmd(0x40);//设置起始行地址  设置映射RAM显示起始线
	WriteCmd(0x81);//设置对比度控制寄存器
	WriteCmd(0xCF);//设置SEG输出电流亮度
	WriteCmd(0xA1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	WriteCmd(0xC8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	WriteCmd(0xA6);//设置正常显示
	WriteCmd(0xA8);//设置复用比(1 ~ 64)--set multiplex ratio(1 to 64)
	WriteCmd(0x3f);//1/64的责任--1/64 duty
	WriteCmd(0xD3);//设置显示抵消-set display offset	移位映射RAM计数器(0x00~0x3F)--Shift Mapping RAM Counter (0x00~0x3F)
	WriteCmd(0x00);//-not offset
	WriteCmd(0xd5);//设置显示时钟分频比/振荡器频率 --set display clock divide ratio/oscillator frequency
	WriteCmd(0x80);//设置分割比，设置时钟为100帧/秒 --set divide ratio, Set Clock as 100 Frames/Sec
	WriteCmd(0xD9);//设置pre-charge时期 --set pre-charge period
	WriteCmd(0xF1);//设置预充为15个时钟，放电为1个时钟 —— Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	WriteCmd(0xDA);//设置com引脚硬件配置 --set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xDB);//--set vcomh
	WriteCmd(0x40);//Set VCOM Deselect Level
	WriteCmd(0x20);//设置页面寻址模式(0x00/0x01/0x02) -Set Page Addressing Mode (0x00/0x01/0x02)
	WriteCmd(0x02);//
	WriteCmd(0x8D);//设置充电泵启用/禁用 --set Charge Pump enable/disable
	WriteCmd(0x14);//--set(0x10) disable
	WriteCmd(0xA4);//关闭全屏显示(0xa4/0xa5) —— Disable Entire Display On (0xa4/0xa5)
	WriteCmd(0xA6);//Disable Inverse Display On (0xa6/a7) —— Disable Inverse Display On (0xa6/a7) 
	WriteCmd(0xAF);
	
	OLED_Display_On();
	OLED_Clear();
}

//========================================================================
// 函数: void OLED_DrawPoint(uint8_t x, uint8_t y)
// 描述: OLED画点函数.
// 参数: (x,y)画点坐标; x:0~127, y:0~63.
// 返回: None.
// 版本: V1.0, 2022年6月6日
//========================================================================
void OLED_DrawPoint(uint8_t x, uint8_t y)
{
	uint8_t i,m,n;
	i = y/8;	//算出在哪一页
	m = y%8;
	n = 1<<m;
	OLED_GRAM[x][i] |= n;
}

//========================================================================
// 函数: void OLED_ClearPoint(uint8_t x, uint8_t y)
// 描述: OLED清除点函数, 
// 参数: (x,y)清除坐标; x:0~127, y:0~63.
// 返回: None.
// 版本: V1.0, 2022年6月6日
//========================================================================
void OLED_ClearPoint(uint8_t x, uint8_t y)
{
	uint8_t i,m,n;
	i = y/8;	//算出在哪一页
	m = y%8;
	n = 1<<m;
	OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
	OLED_GRAM[x][i] |= n;
	OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
}

//========================================================================
// 函数: void OLED_ClearLine(uint8_t x1, uint8_t y1, int8_t x2, uint8_t y2)
// 描述: OLED画线函数.
// 参数: (x1,y1)起始坐标；(x2,y2)终点坐标; x:0~127, y:0~63.
// 返回: None.
// 版本: V1.0, 2022年6月6日
//========================================================================
void OLED_ClearLine(uint8_t x1, uint8_t y1, int8_t x2, uint8_t y2)
{
	uint8_t i,k,k1,k2;
	//if((x1<0) || (x2>127) || (y1<0) || (y2>63) || (x1>x2) || (y1>y2)) return;
	if((x2>127) || (y2>63) || (x1>x2) || (y1>y2)) return;
	if(x1 == x2)	//画竖线
	{
		for(i=0; i<(y2-y1); i++)
			OLED_DrawPoint(x1,y1+i);
	}
	else if(y1 == y2)	//画横线
	{
		for(i=0; i<(x2-x1); i++)
			OLED_DrawPoint(x1+i,y1);
	}
	else	//画斜线
	{
		k1 = y2 - y1;
		k2 = x2 - x1;
		k = k1*10/k2;	//求出斜率，*10取整
		for(i=0; i<(x2-x1); i++)
			OLED_DrawPoint(x1+i, y1+i*k/10);
	}
}

//========================================================================
// 函数: void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r)
// 描述: OLED画圆函数.
// 参数: x,y:圆心坐标；r:圆的半径；x:0~127, y:0~63.
// 返回: None.
// 版本: V1.0, 2022年6月6日
//========================================================================
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r)
{
	int a, c, num;
	a = 0;
	c = r;
	while(2 * c * c >= r * r)	//理想中，画完圆的时候有: 2*c^2 = r^2, 当2*c^2 < r^2时，不再画圆
	{
		OLED_DrawPoint(x + a, y - c);
        OLED_DrawPoint(x - a, y - c);
        OLED_DrawPoint(x - a, y + c);
        OLED_DrawPoint(x + a, y + c);
 
        OLED_DrawPoint(x + c, y + a);
        OLED_DrawPoint(x + c, y - a);
        OLED_DrawPoint(x - c, y - a);
        OLED_DrawPoint(x - c, y + a);
		
		a++;
		
		num = (a*a + c*c) - r*r;	//计算画的点离圆心的距离
		if(num > 0)	//大于0，说明画的点离圆心的距离超出了半径长度，需要修改参数
		{
			c--;
			a--;
		}
	}
}

//========================================================================
// 函数: void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size1)
// 描述: OLED显示一个字符.
// 参数: x,y:起点坐标; chr：字符; size1:选择字体 12/16/24; x:0~127, y:0~63.
// 返回: None.
// 版本: V1.0, 2022年6月6日
//========================================================================
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size1)
{
	uint8_t i,m,temp,size2,chr1;
	uint8_t y0 = y;
	size2 = (size1/8 + ((size1%8)?1:0)) * (size1/2);	//得到字体一个字符对应点阵集所占的字节数
	chr1 = chr - ' ';	//计算偏移后的值
	for(i=0; i<size2; i++)
	{
		if(size1 == 12)
			temp = asc2_1206[chr1][i];
		else if(size1 == 16)
			temp = asc2_1608[chr1][i];
		else if(size1 == 24)
			temp = asc2_2412[chr1][i];
		else return;
		
		for(m=0; m<8; m++)
		{
			if(temp&0x80) OLED_DrawPoint(x,y);
			else OLED_ClearPoint(x,y);
			temp <<= 1;
			y++;
			if((y-y0) == size1)
			{
				y = y0;
				x++;
				break;
			}
		}
	}
}

//========================================================================
// 函数: void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t size1)
// 描述: OLED显示字符串.
// 参数: x,y:起点坐标; chr：字符; size1:选择字体 12/16/24; x:0~127, y:0~63.
// 返回: None.
// 版本: V1.0, 2022年6月6日
//========================================================================
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t size1)
{
	while((*chr >= ' ') && (*chr <= '~'))	//判断是不是非法字符
	{
		OLED_ShowChar(x, y, *chr, size1);
		x += size1/2;
		if(x > 127-size1)	//换行
		{
			x = 0;
			y += size1;
		}
		
		chr++;
	}
}

//========================================================================
// 函数: u32 OLED_Pow(u8 m,u8 n)
// 描述: 算数 m^n.
// 参数: m:底数; n:幂
// 返回: 求幂结果.
// 版本: V1.0, 2022年6月6日
//========================================================================
uint32_t OLED_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}
	
//========================================================================
// 函数: void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size1)
// 描述: OLED显示数字函数.
// 参数: x,y :起点坐标; len :数字的位数; size:字体大小.
// 返回: None.
// 版本: V1.0, 2022年6月6日
//========================================================================
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size1)
{
	uint8_t t, temp;
	for(t=0; t<len; t++)
	{
		temp = (num/OLED_Pow(10,len-t-1))%10;	//从最左位开始逐一取出一个数字
		if(temp == 0)
		{
			OLED_ShowChar(x+(size1/2)*t, y, '0', size1);
		}
		else
		{
			OLED_ShowChar(x+(size1/2)*t, y, temp+'0', size1);
		}
	}
}

//========================================================================
// 函数: void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t num, uint8_t size1)
// 描述: OLED显示汉字函数.
// 参数: x,y :起点坐标; num:汉字对应的序号; size:字体大小.
// 返回: None.
// 版本: V1.0, 2022年6月6日
//========================================================================
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t num, uint8_t size1)
{
	uint8_t i, m, n=0, temp, chr1;
	uint8_t x0 = x;
	uint8_t y0 = y;
	uint8_t size3 = size1/8;
	
	while(size3--)
	{
		chr1 = num * size1/8 + n;
		n++;
		for(i=0; i<size1; i++)
		{
			if(size1 == 16) temp = Hzk1[chr1][i];
			else if(size1 == 24) temp = Hzk2[chr1][i];
			else if(size1 == 32) temp = Hzk3[chr1][i];
			else if(size1 == 64) temp = Hzk4[chr1][i];
			else return;
			
			for(m=0; m<8; m++)
			{
				if(temp&0x01) OLED_DrawPoint(x,y);
				else OLED_ClearPoint(x,y);
				temp >>= 1;
				y++;
			}
			
			x++;
			if((x-x0) == size1)
			{
				x = x0;
				y0 = y0+8;
			}
			y = y0;
		}
	}
}

//配置写入数据的起始位置
//x:0~127; y:0~7
void OLED_WR_BP(uint8_t x, uint8_t y)
{
	WriteCmd(0xb0 + y);	//设置起始页地址
	WriteCmd(((x&0xf0) >> 4) | 0x10);
	WriteCmd((x&0x0f));
}

//========================================================================
// 函数: void OLED_Showpicture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[])
// 描述: OLED显示图片函数.
// 参数: x0,y0 :起点坐标; x1,y1 :终点坐标; BMP[] :图片数组.
// 返回: None.
// 版本: V1.0, 2022年6月7日
//========================================================================
void OLED_ShowPicture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[])
{
	uint32_t j = 0;
	uint8_t x = 0, y = 0;
	if(y%8 == 0) y = 0;
	else y += 1;
	
	for(y=y0; y<y1; y++)
	{
		OLED_WR_BP(x0, y);
		for(x=x0; x<x1; x++)
		{
			WriteDat(BMP[j]);
			j++;
		}
	}
}

//========================================================================
// 函数: void OLED_Play_1(void)
// 描述: OLED显示函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月13日
//========================================================================
void OLED_Play_1(void)
{
	OLED_ShowString(0,0, "PWM  :",16);
	OLED_ShowChar(72,0, '%',16);
	
//	OLED_ShowString(0,16,"PWM_Duty:",16);
	OLED_ShowString(0,16,"RealS:",16);
	OLED_ShowString(0,32,"ADC  :",16);
	OLED_Refresh();
}


