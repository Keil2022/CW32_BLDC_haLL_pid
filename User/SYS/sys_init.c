#include "sys_init.h"
#include "app.h"

void RCC_Configuration(void)
{
	uint8_t reg = 0U;
	
	RCC_AHBPeriphClk_Enable(RCC_AHB_PERIPH_FLASH, ENABLE);   //打开FLASH时钟
	RCC_HSE_Enable( RCC_HSE_MODE_OSC, 8000000, RCC_HSE_DRIVER_NORMAL, RCC_HSE_FLT_CLOSE ); //开启外部高速时钟HSE，实际频率需要根据实际晶体频率进行填写
	
	RCC_HCLKPRS_Config(RCC_HCLK_DIV1);	//设置HCLK的分频系数
	RCC_PCLKPRS_Config(RCC_PCLK_DIV1);	//设置PCLK的分频系数
	
	RCC_PLL_Enable( RCC_PLLSOURCE_HSEOSC, 8000000, RCC_PLL_MUL_8 );     //开启PLL，PLL时钟来源为HSE
	FLASH_SetLatency(FLASH_Latency_3);    								//频率大于24M需要配置FlashWait=2,频率大于48M需要配置FlashWait=3
	
	reg = RCC_SysClk_Switch( RCC_SYSCLKSRC_PLL );                       //切换系统时钟到PLL
	RCC_SystemCoreClockUpdate( 64000000 );								//设置时钟频率，感觉这句可以不要！【1】
	
	if(reg == 0X0U) RCC_HSI_Disable();                                  //切换时钟到PLL后关闭源时钟HSI,上电默认打开HSI
	else
	{
		while(1);
	}
}

//========================================================================
// 函数: int SYS_Init(void)
// 描述: 系统初始化.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月1日
//========================================================================
void SYS_Init(void)
{
	delay1ms(500);
	
	RCC_Configuration();	//时钟初始化
	
	APP_config();	//外设配置
}



