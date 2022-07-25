#include "btim.h"
#include "task.h"
#include "app.h"

//========================================================================
// 函数: void BTIM1_Configuration(void)
// 描述: 基础定时器配置函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月1日
//========================================================================
void BTIM1_Configuration(void)
{
	BTIM_TimeBaseInitTypeDef BTIM_TimeBaseInitStruct;
	__RCC_BTIM_CLK_ENABLE();
	
	__disable_irq();			//关闭中断
	NVIC_EnableIRQ(BTIM1_IRQn);	//使能BTIM1中断
	__enable_irq();				//打开中断
	
	BTIM_TimeBaseInitStruct.BTIM_Mode = BTIM_Mode_TIMER;			//工作模式，00：定时器模式
	BTIM_TimeBaseInitStruct.BTIM_OPMode = BTIM_OPMode_Repetitive;	//连续模式控制
	BTIM_TimeBaseInitStruct.BTIM_Period = 1999;						//重装载值
	BTIM_TimeBaseInitStruct.BTIM_Prescaler = BTIM_PRS_DIV32;		//时钟分频系数，t = PCLK/((2^BTIM_PRS_DIVx)*(Period+1))

	BTIM_TimeBaseInit(CW_BTIM1, &BTIM_TimeBaseInitStruct);			//配置BTIM1定时器
	BTIM_ITConfig(CW_BTIM1, BTIM_IT_OV, ENABLE);					//使能BTIM1中断
	BTIM_Cmd(CW_BTIM1, ENABLE);										//使能定时器
}

//========================================================================
// 函数: void BTIM1_IRQHandleCallback(void)
// 描述: 定时器中断处理函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月1日
//========================================================================
void BTIM1_IRQHandler(void)
{
	if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_OV))			//检查BTIM的状态寄存器的状态位是否置位
	{
		BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_OV);	//清除中断标志位
		Task_Marks_Handler_Callback();					//执行程序
		
		if(Count_1 <255) Count_1++;
//		if(++t >= 100)
//		{
//			t = 0;
//			PC13_TOG();
//			Task_Marks_Handler_Callback();
//		}
	}
}




