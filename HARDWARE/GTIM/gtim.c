#include "gtim.h"
#include "app.h"

//HA: PA15
//HB: PB3
//HC: PA2

//========================================================================
// 函数: void GTIM2_Configuration(void)
// 描述: 定时器GTIM2配置函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月8日
//========================================================================
void GTIM2_Configuration(void)
{
	GTIM_InitTypeDef 	GTIM_InitStruct;
	GTIM_ICInitTypeDef 	GTIM_ICInitStruct;
	GPIO_InitTypeDef 	GPIO_InitStruct;
	
	__RCC_GTIM2_CLK_ENABLE();	//开启GTIM2时钟
	__RCC_GPIOA_CLK_ENABLE();	//开启GPIOA时钟
	__RCC_GPIOB_CLK_ENABLE();	//开启GPIOB时钟
	
	PA15_AFx_GTIM2CH1();		//  GTIM2CH1();	开启复用功能
	PB03_AFx_GTIM2CH2();		//  GTIM2CH2();
	PA02_AFx_GTIM2CH3();		//	GTIM2CH3();		
	
	GPIO_InitStruct.IT = GPIO_IT_NONE;
	GPIO_InitStruct.Mode = GPIO_HALL_A_MODE;		//GPIO模式
	GPIO_InitStruct.Pins = GPIO_HALL_A_PIN;			//GPIO端口
	GPIO_InitStruct.Speed = GPIO_HALL_A_SPEED;		//GPIO速度
	GPIO_Init(GPIO_HALL_A, &GPIO_InitStruct);		//配置GPIO
	
	GPIO_InitStruct.IT = GPIO_IT_NONE;
	GPIO_InitStruct.Mode = GPIO_HALL_B_MODE;		//GPIO模式
	GPIO_InitStruct.Pins = GPIO_HALL_B_PIN;			//GPIO端口
	GPIO_InitStruct.Speed = GPIO_HALL_B_SPEED;		//GPIO速度
	GPIO_Init(GPIO_HALL_B, &GPIO_InitStruct);		//配置GPIO
	
	__disable_irq(); 
	NVIC_SetPriority(GTIM2_IRQn, 0);	//优先级：0（最大）
	NVIC_EnableIRQ(GTIM2_IRQn); 		//开启中断
	__enable_irq();
	
	GTIM_InitStruct.Mode = GTIM_MODE_TIME;				//GTIM定时器模式
	GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;	//GTIM的连续计数模式	
	GTIM_InitStruct.Prescaler = GTIM_PRESCALER_DIV1;	//GTIM的预分频系数。
	GTIM_InitStruct.ReloadValue = 0xFFFF;				//GTIM的重载值。
	GTIM_InitStruct.ToggleOutState = DISABLE;			//GTIM的翻转输出不使能。
	GTIM_TimeBaseInit(CW_GTIM2, &GTIM_InitStruct); 		//配置GTIM2定时器
	
	GTIM_ICInitStruct.CHx = GTIM_CHANNEL1;						//通道1
	GTIM_ICInitStruct.ICFilter = GTIM_CHx_FILTER_PCLK_N2;		//输入信号滤波配置，时钟：PCLK; 采样个数：2
	GTIM_ICInitStruct.ICInvert = GTIM_CHx_INVERT_OFF;			//关闭翻转
	GTIM_ICInitStruct.ICPolarity = GTIM_ICPolarity_BothEdge;	//双沿
	GTIM_ICInit(CW_GTIM2, &GTIM_ICInitStruct);					//输入比较初始化

	GTIM_ICInitStruct.CHx = GTIM_CHANNEL2;						//通道2
	GTIM_ICInit(CW_GTIM2, &GTIM_ICInitStruct);					//输入比较初始化

	GTIM_ICInitStruct.CHx = GTIM_CHANNEL3;						//通道3
	GTIM_ICInit(CW_GTIM2, &GTIM_ICInitStruct);					//输入比较初始化

	GTIM_ITConfig(CW_GTIM2, GTIM_IT_CC1 | GTIM_IT_CC2 | GTIM_IT_CC3, ENABLE);	//中断使能
	GTIM_Cmd(CW_GTIM2, ENABLE);													//定时器使能
}

//========================================================================
// 函数: void GTIM2_IRQHandler(void)
// 描述: 中断处理函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月8日
//========================================================================
void GTIM2_IRQHandler(void)
{
	uint16_t x;
	
	if (GTIM_GetITStatus(CW_GTIM2, GTIM_IT_CC1))
	{
		GTIM_ClearITPendingBit(CW_GTIM2, GTIM_IT_CC1);
	}

	if (GTIM_GetITStatus(CW_GTIM2, GTIM_IT_CC2))
	{    
		GTIM_ClearITPendingBit(CW_GTIM2, GTIM_IT_CC2);
	}

	if (GTIM_GetITStatus(CW_GTIM2, GTIM_IT_CC3))
	{    
		GTIM_ClearITPendingBit(CW_GTIM2, GTIM_IT_CC3);
	}
	
	HALLcount++;HALLcount1++;						//霍尔脉冲计数	
	x = HALL_Check(); 								//读取霍尔状态
	Cur_step = STEP_TAB[Dir][x-1];					//获取换相序位		
	if( Motor_Start_F==1 && Err_Code==0 ) 			//根据启停状态 换相
		Commutation(Cur_step,Motor_Start_F);  
}

//========================================================================
// 函数: void Speed_Count(void)
// 描述: 速度计算函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月10日
//========================================================================
void Speed_Count(void)
{
	//计算速度
	RealS = HALLcount*50 / MotorPoles;	//计算实测电机RPM
	HALLcount = 0;
	
	if( MOTORSTATE == STATESTARTPID || MOTORSTATE == STATERUNPID )
	{
		if(RealS < 120)
		{
			DZCount++;
			if(DZCount >= 10)	//2S保护
			{ 								
				DZCount = 0;
				Err_Code = 7;	//堵转 速度低的故障判断
			}
		}
		else DZCount = 0;
	}
}


