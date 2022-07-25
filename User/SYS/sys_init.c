#include "sys_init.h"
#include "app.h"

void RCC_Configuration(void)
{
	uint8_t reg = 0U;
	
	RCC_AHBPeriphClk_Enable(RCC_AHB_PERIPH_FLASH, ENABLE);   //��FLASHʱ��
	RCC_HSE_Enable( RCC_HSE_MODE_OSC, 8000000, RCC_HSE_DRIVER_NORMAL, RCC_HSE_FLT_CLOSE ); //�����ⲿ����ʱ��HSE��ʵ��Ƶ����Ҫ����ʵ�ʾ���Ƶ�ʽ�����д
	
	RCC_HCLKPRS_Config(RCC_HCLK_DIV1);	//����HCLK�ķ�Ƶϵ��
	RCC_PCLKPRS_Config(RCC_PCLK_DIV1);	//����PCLK�ķ�Ƶϵ��
	
	RCC_PLL_Enable( RCC_PLLSOURCE_HSEOSC, 8000000, RCC_PLL_MUL_8 );     //����PLL��PLLʱ����ԴΪHSE
	FLASH_SetLatency(FLASH_Latency_3);    								//Ƶ�ʴ���24M��Ҫ����FlashWait=2,Ƶ�ʴ���48M��Ҫ����FlashWait=3
	
	reg = RCC_SysClk_Switch( RCC_SYSCLKSRC_PLL );                       //�л�ϵͳʱ�ӵ�PLL
	RCC_SystemCoreClockUpdate( 64000000 );								//����ʱ��Ƶ�ʣ��о������Բ�Ҫ����1��
	
	if(reg == 0X0U) RCC_HSI_Disable();                                  //�л�ʱ�ӵ�PLL��ر�Դʱ��HSI,�ϵ�Ĭ�ϴ�HSI
	else
	{
		while(1);
	}
}

//========================================================================
// ����: int SYS_Init(void)
// ����: ϵͳ��ʼ��.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��1��
//========================================================================
void SYS_Init(void)
{
	delay1ms(500);
	
	RCC_Configuration();	//ʱ�ӳ�ʼ��
	
	APP_config();	//��������
}



