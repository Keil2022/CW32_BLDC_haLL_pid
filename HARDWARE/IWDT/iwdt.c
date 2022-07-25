#include "iwdt.h"

//========================================================================
// 函数: void IWDT_Configuration(void)
// 描述: 独立看门狗初始化配置.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月7日
//========================================================================
void IWDT_Configuration(void)
{
	IWDT_InitTypeDef IWDT_InitStruct;
	
	__RCC_IWDT_CLK_ENABLE(); //使能IWDT模块
	
	IWDT_InitStruct.IWDT_ITState = DISABLE;								//看门狗中断状态，取值范围ENABLE或DISABLE
	IWDT_InitStruct.IWDT_OverFlowAction = IWDT_OVERFLOW_ACTION_RESET;	//溢出后的动作：复位
	IWDT_InitStruct.IWDT_Pause = IWDT_SLEEP_PAUSE;						//Deepsleep模式下IWDT暂停控制
	IWDT_InitStruct.IWDT_Prescaler = IWDT_Prescaler_DIV128;				//预分配系数，取值范围2的n次幂，n=2,3..9
	IWDT_InitStruct.IWDT_ReloadValue = 79;  							//看门狗重载值，取值范围小于等于0x0FFF		1S
	IWDT_InitStruct.IWDT_WindowValue = 0xFFF;							//窗口比较器比较值,取值范围小于等于0x0FFF

	IWDT_Init(&IWDT_InitStruct);
	
	IWDT_Cmd();					//启动IWDT
	while(!CW_IWDT->SR_f.RUN);	//等待IWDT启动
	IWDT_Refresh();				//喂狗
}


