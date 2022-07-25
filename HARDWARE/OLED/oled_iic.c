#include "oled_iic.h"
#include "oledfont.h"

uint8_t OLED_GRAM[144][8];

//========================================================================
// ����: void OLED_I2C_Init(void)
// ����: OLED IIC��ʼ������.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��2��
//========================================================================
void OLED_I2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;
	
	__RCC_GPIOB_CLK_ENABLE();	//ʹ��PBʱ��
	__RCC_I2C1_CLK_ENABLE();	//ʹ��I2C1ʱ��
	
	PB06_AFx_I2C1SCL();		//�򿪸��ù���
	PB07_AFx_I2C1SDA();
	
	GPIO_InitStruct.Pins = GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;		//I2C����Ϊ��©
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;	
	GPIO_Init(CW_GPIOB,&GPIO_InitStruct);
	
	I2C_InitStruct.I2C_BaudEn = ENABLE;	//�����ʼ�����ʹ�� 
	I2C_InitStruct.I2C_Baud = 0X08;		//�����ʼ���������
	I2C_InitStruct.I2C_FLT = DISABLE;	//FLT����
	I2C_InitStruct.I2C_AA = DISABLE;	//ACK����
	
	I2C1_DeInit();
	I2C_Master_Init(CW_I2C1, &I2C_InitStruct);
	I2C_Cmd(CW_I2C1,ENABLE);
}

//========================================================================
// ����: void WriteCmd(void)
// ����: OLEDд�����.
// ����: ����
// ����: None.
// �汾: V1.0, 2022��6��2��
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
			case 0x08:   //������START�ź�
				I2C_GenerateSTART(I2Cx, DISABLE);
				I2C_Send7bitAddress(I2Cx, I2C_SLAVEADDRESS_OLED,0X00);  //���豸��ַ����
				break;
			case 0x18:   //������SLA+W�ź�,ACK���յ�
				I2C_SendData(I2Cx,u8Addr);//���豸�ڴ��ַ����
				break;
			case 0x28:   //������1�ֽ����ݣ�����EEPROM��memory��ַҲ����������ͺ��������Ҳ�����	
				I2C_SendData(I2Cx,pu8Data[u8i++]);
				break;
			case 0x20:   //������SLA+W��ӻ�����NACK
			case 0x38:    //�����ڷ��� SLA+W �׶λ��߷������ݽ׶ζ�ʧ�ٲ�  ����  �����ڷ��� SLA+R �׶λ��߻�Ӧ NACK �׶ζ�ʧ�ٲ�
				I2C_GenerateSTART(I2Cx, ENABLE);
				break;
			case 0x30:   //������һ�������ֽں�ӻ�����NACK
				I2C_GenerateSTOP(I2Cx, ENABLE);
				break;
			default:
				break;
		}			
		if(u8i>u32Len)
		{
			I2C_GenerateSTOP(I2Cx, ENABLE);//��˳���ܵ�������ֹͣ����
			I2C_ClearIrq(I2Cx);
			break;
		}
		I2C_ClearIrq(I2Cx);			
	}
}

//========================================================================
// ����: void WriteCmd(void)
// ����: OLEDд�����.
// ����: ����
// ����: None.
// �汾: V1.0, 2022��6��2��
//========================================================================
void WriteCmd(uint8_t I2C_Command)
{
	I2C_MasterWriteByte(CW_I2C1,COM,&I2C_Command,1);
}

//========================================================================
// ����: void WriteDat(void)
// ����: OLEDд���ݺ���.
// ����: ����
// ����: None.
// �汾: V1.0, 2022��6��2��
//========================================================================
void WriteDat(uint8_t I2C_Data)
{
	I2C_MasterWriteByte(CW_I2C1,DAT,&I2C_Data,1);
}

//========================================================================
// ����: void OLED_Display_On(void)
// ����: OLED����ʾ.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��2��
//========================================================================
void OLED_Display_On(void)
{
	WriteCmd(0x8D);	//��ɱ�����ʹ��
	WriteCmd(0x14);	//������ɱ�
	WriteCmd(0xAF);	//������ʾ
}

//========================================================================
// ����: void OLED_Display_Off(void)
// ����: OLED�ر���ʾ.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��2��
//========================================================================
void OLED_Display_Off(void)
{
	WriteCmd(0x8D);	//��ɱ�����ʹ��
	WriteCmd(0x10);	//�رյ�ɱ�
	WriteCmd(0xAE);	//�ر���ʾ	
}

//========================================================================
// ����: void OLED_Refresh(void)
// ����: OLEDˢ���Դ溯��.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��2��
//========================================================================
void OLED_Refresh(void)
{
	uint8_t n,i;
	for(i=0; i<8; i++)
	{
		WriteCmd(0xb0+i);	//����ҳ��ַ��0xb0����ҳ��һҳ��8��
		WriteCmd(0x00);		//������ʼ�е�ַ����λ
		WriteCmd(0x10);		//������ʼ�е�ַ����λ
		for(n=0; n<128; n++)
			WriteDat(OLED_GRAM[n][i]);
	}
}

//========================================================================
// ����: void OLED_Clear(void)
// ����: OLED��������.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��2��
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
	
	OLED_Refresh();	//������ʾ
}

//========================================================================
// ����: void OLED_Init(void)
// ����: OLED��ʼ������.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��2��
//========================================================================
void OLED_Init(void)
{
	WriteCmd(0xAE);//�ر���ʾ
	WriteCmd(0x00);//���õ��е�ַ
	WriteCmd(0x10);//���ø��е�ַ
	WriteCmd(0x40);//������ʼ�е�ַ  ����ӳ��RAM��ʾ��ʼ��
	WriteCmd(0x81);//���öԱȶȿ��ƼĴ���
	WriteCmd(0xCF);//����SEG�����������
	WriteCmd(0xA1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	WriteCmd(0xC8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	WriteCmd(0xA6);//����������ʾ
	WriteCmd(0xA8);//���ø��ñ�(1 ~ 64)--set multiplex ratio(1 to 64)
	WriteCmd(0x3f);//1/64������--1/64 duty
	WriteCmd(0xD3);//������ʾ����-set display offset	��λӳ��RAM������(0x00~0x3F)--Shift Mapping RAM Counter (0x00~0x3F)
	WriteCmd(0x00);//-not offset
	WriteCmd(0xd5);//������ʾʱ�ӷ�Ƶ��/����Ƶ�� --set display clock divide ratio/oscillator frequency
	WriteCmd(0x80);//���÷ָ�ȣ�����ʱ��Ϊ100֡/�� --set divide ratio, Set Clock as 100 Frames/Sec
	WriteCmd(0xD9);//����pre-chargeʱ�� --set pre-charge period
	WriteCmd(0xF1);//����Ԥ��Ϊ15��ʱ�ӣ��ŵ�Ϊ1��ʱ�� ���� Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	WriteCmd(0xDA);//����com����Ӳ������ --set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xDB);//--set vcomh
	WriteCmd(0x40);//Set VCOM Deselect Level
	WriteCmd(0x20);//����ҳ��Ѱַģʽ(0x00/0x01/0x02) -Set Page Addressing Mode (0x00/0x01/0x02)
	WriteCmd(0x02);//
	WriteCmd(0x8D);//���ó�������/���� --set Charge Pump enable/disable
	WriteCmd(0x14);//--set(0x10) disable
	WriteCmd(0xA4);//�ر�ȫ����ʾ(0xa4/0xa5) ���� Disable Entire Display On (0xa4/0xa5)
	WriteCmd(0xA6);//Disable Inverse Display On (0xa6/a7) ���� Disable Inverse Display On (0xa6/a7) 
	WriteCmd(0xAF);
	
	OLED_Display_On();
	OLED_Clear();
}

//========================================================================
// ����: void OLED_DrawPoint(uint8_t x, uint8_t y)
// ����: OLED���㺯��.
// ����: (x,y)��������; x:0~127, y:0~63.
// ����: None.
// �汾: V1.0, 2022��6��6��
//========================================================================
void OLED_DrawPoint(uint8_t x, uint8_t y)
{
	uint8_t i,m,n;
	i = y/8;	//�������һҳ
	m = y%8;
	n = 1<<m;
	OLED_GRAM[x][i] |= n;
}

//========================================================================
// ����: void OLED_ClearPoint(uint8_t x, uint8_t y)
// ����: OLED����㺯��, 
// ����: (x,y)�������; x:0~127, y:0~63.
// ����: None.
// �汾: V1.0, 2022��6��6��
//========================================================================
void OLED_ClearPoint(uint8_t x, uint8_t y)
{
	uint8_t i,m,n;
	i = y/8;	//�������һҳ
	m = y%8;
	n = 1<<m;
	OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
	OLED_GRAM[x][i] |= n;
	OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
}

//========================================================================
// ����: void OLED_ClearLine(uint8_t x1, uint8_t y1, int8_t x2, uint8_t y2)
// ����: OLED���ߺ���.
// ����: (x1,y1)��ʼ���ꣻ(x2,y2)�յ�����; x:0~127, y:0~63.
// ����: None.
// �汾: V1.0, 2022��6��6��
//========================================================================
void OLED_ClearLine(uint8_t x1, uint8_t y1, int8_t x2, uint8_t y2)
{
	uint8_t i,k,k1,k2;
	//if((x1<0) || (x2>127) || (y1<0) || (y2>63) || (x1>x2) || (y1>y2)) return;
	if((x2>127) || (y2>63) || (x1>x2) || (y1>y2)) return;
	if(x1 == x2)	//������
	{
		for(i=0; i<(y2-y1); i++)
			OLED_DrawPoint(x1,y1+i);
	}
	else if(y1 == y2)	//������
	{
		for(i=0; i<(x2-x1); i++)
			OLED_DrawPoint(x1+i,y1);
	}
	else	//��б��
	{
		k1 = y2 - y1;
		k2 = x2 - x1;
		k = k1*10/k2;	//���б�ʣ�*10ȡ��
		for(i=0; i<(x2-x1); i++)
			OLED_DrawPoint(x1+i, y1+i*k/10);
	}
}

//========================================================================
// ����: void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r)
// ����: OLED��Բ����.
// ����: x,y:Բ�����ꣻr:Բ�İ뾶��x:0~127, y:0~63.
// ����: None.
// �汾: V1.0, 2022��6��6��
//========================================================================
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r)
{
	int a, c, num;
	a = 0;
	c = r;
	while(2 * c * c >= r * r)	//�����У�����Բ��ʱ����: 2*c^2 = r^2, ��2*c^2 < r^2ʱ�����ٻ�Բ
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
		
		num = (a*a + c*c) - r*r;	//���㻭�ĵ���Բ�ĵľ���
		if(num > 0)	//����0��˵�����ĵ���Բ�ĵľ��볬���˰뾶���ȣ���Ҫ�޸Ĳ���
		{
			c--;
			a--;
		}
	}
}

//========================================================================
// ����: void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size1)
// ����: OLED��ʾһ���ַ�.
// ����: x,y:�������; chr���ַ�; size1:ѡ������ 12/16/24; x:0~127, y:0~63.
// ����: None.
// �汾: V1.0, 2022��6��6��
//========================================================================
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size1)
{
	uint8_t i,m,temp,size2,chr1;
	uint8_t y0 = y;
	size2 = (size1/8 + ((size1%8)?1:0)) * (size1/2);	//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr1 = chr - ' ';	//����ƫ�ƺ��ֵ
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
// ����: void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t size1)
// ����: OLED��ʾ�ַ���.
// ����: x,y:�������; chr���ַ�; size1:ѡ������ 12/16/24; x:0~127, y:0~63.
// ����: None.
// �汾: V1.0, 2022��6��6��
//========================================================================
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t size1)
{
	while((*chr >= ' ') && (*chr <= '~'))	//�ж��ǲ��ǷǷ��ַ�
	{
		OLED_ShowChar(x, y, *chr, size1);
		x += size1/2;
		if(x > 127-size1)	//����
		{
			x = 0;
			y += size1;
		}
		
		chr++;
	}
}

//========================================================================
// ����: u32 OLED_Pow(u8 m,u8 n)
// ����: ���� m^n.
// ����: m:����; n:��
// ����: ���ݽ��.
// �汾: V1.0, 2022��6��6��
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
// ����: void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size1)
// ����: OLED��ʾ���ֺ���.
// ����: x,y :�������; len :���ֵ�λ��; size:�����С.
// ����: None.
// �汾: V1.0, 2022��6��6��
//========================================================================
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size1)
{
	uint8_t t, temp;
	for(t=0; t<len; t++)
	{
		temp = (num/OLED_Pow(10,len-t-1))%10;	//������λ��ʼ��һȡ��һ������
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
// ����: void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t num, uint8_t size1)
// ����: OLED��ʾ���ֺ���.
// ����: x,y :�������; num:���ֶ�Ӧ�����; size:�����С.
// ����: None.
// �汾: V1.0, 2022��6��6��
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

//����д�����ݵ���ʼλ��
//x:0~127; y:0~7
void OLED_WR_BP(uint8_t x, uint8_t y)
{
	WriteCmd(0xb0 + y);	//������ʼҳ��ַ
	WriteCmd(((x&0xf0) >> 4) | 0x10);
	WriteCmd((x&0x0f));
}

//========================================================================
// ����: void OLED_Showpicture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[])
// ����: OLED��ʾͼƬ����.
// ����: x0,y0 :�������; x1,y1 :�յ�����; BMP[] :ͼƬ����.
// ����: None.
// �汾: V1.0, 2022��6��7��
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
// ����: void OLED_Play_1(void)
// ����: OLED��ʾ����.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��13��
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


