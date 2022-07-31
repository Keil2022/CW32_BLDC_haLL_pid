#include "atim.h"

//U+: PA8 
//V+: PA9
//W+: PA10

//U-: PB13 
//V-: PB14
//W-: PB15

//上管调制，下管IO

//========================================================================
// 函数: void ATIM_Configuration(void)
// 描述: 高级定时器ATIM配置函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月8日
//========================================================================
void ATIM_Configuration(void)
{
	ATIM_InitTypeDef 	ATIM_InitStruct;		//定义配置成员
	ATIM_OCInitTypeDef 	ATIM_OCInitStruct;
	GPIO_InitTypeDef 	GPIO_InitStruct;
	
	__RCC_ATIM_CLK_ENABLE();	//开启ATIM时钟
	__RCC_GPIOA_CLK_ENABLE();	//开启GPIOA时钟
	__RCC_GPIOB_CLK_ENABLE();	//开启GPIOB时钟
	
	PA08_AFx_ATIMCH1A();	//PA8复用ATIM通道1
	PA09_AFx_ATIMCH2A();	//PA9复用ATIM通道2
	PA10_AFx_ATIMCH3A();	//PA10复用ATIM通道3
	
	GPIO_InitStruct.IT = GPIO_IT_NONE;
	GPIO_InitStruct.Mode = GPIO_Down_Tube_MODE;		//GPIO模式
	GPIO_InitStruct.Pins = GPIO_Down_Tube_PIN;		//GPIO端口
	GPIO_InitStruct.Speed = GPIO_Down_Tube_SPEED;	//GPIO速度
	GPIO_Init(GPIO_Down_Tube, &GPIO_InitStruct);	//配置GPIO
	
	PWM_AL_OFF;
	PWM_BL_OFF;
	PWM_CL_OFF;
	
	GPIO_InitStruct.IT = GPIO_IT_NONE;
	GPIO_InitStruct.Mode = GPIO_Top_Tube_MODE;		//GPIO模式
	GPIO_InitStruct.Pins = GPIO_Top_Tube_PIN;		//GPIO端口
	GPIO_InitStruct.Speed = GPIO_Top_Tube_SPEED;	//GPIO速度
	GPIO_Init(GPIO_Top_Tube, &GPIO_InitStruct);		//配置GPIO
	
	PWM_AH_OFF;
	PWM_BH_OFF;
	PWM_CH_OFF;
	
	ATIM_InitStruct.BufferState = ENABLE;								//ARR缓存不使能
	ATIM_InitStruct.ClockSelect	= ATIM_CLOCK_PCLK;						//计数时钟选择:PCLK
	ATIM_InitStruct.CounterAlignedMode	= ATIM_COUNT_MODE_EDGE_ALIGN;	//计数对齐模式: 边沿
	ATIM_InitStruct.CounterDirection = ATIM_COUNTING_UP;				//计数方向：向上计数
	ATIM_InitStruct.CounterOPMode = ATIM_OP_MODE_REPETITIVE;			//运行模式：重复
	ATIM_InitStruct.Prescaler = Pre;									//预分频系数
	ATIM_InitStruct.ReloadValue = Rel;									//ARR赋值，取值范围0x0000~0xFFFF
	ATIM_InitStruct.RepetitionCounter = 0;								//重复周期计数值，RCR寄存器
	ATIM_InitStruct.OverFlowMask = ENABLE;								//上溢出屏蔽使能
	ATIM_InitStruct.UnderFlowMask = DISABLE;							//下溢出屏蔽使能
	ATIM_Init(&ATIM_InitStruct);
	
	ATIM_OCInitStruct.BufferState = DISABLE;						//比较缓存不使能
	ATIM_OCInitStruct.OCDMAState = DISABLE;							//比较匹配触发DMA不使能
	ATIM_OCInitStruct.OCInterruptSelect	= ATIM_OC_IT_NONE;			//比较中断模式：无
	ATIM_OCInitStruct.OCInterruptState = DISABLE;					//比较匹配触发中断使能
	ATIM_OCInitStruct.OCMode = ATIM_OCMODE_PWM1;					//比较模式配置：PWM1
	ATIM_OCInitStruct.OCPolarity = ATIM_OCPOLARITY_NONINVERT;	 	//端口极性选择：正向
	ATIM_OC1AInit(&ATIM_OCInitStruct);	//通道比较输出设置
	ATIM_OC2AInit(&ATIM_OCInitStruct);
	ATIM_OC3AInit(&ATIM_OCInitStruct);
	
	ATIM_SetCompare1A(0);	//控制占空比，写入数值为 正占空比
	ATIM_SetCompare2A(0);
	ATIM_SetCompare3A(0);
	
	ATIM_PWMOutputConfig(OCREFA_TYPE_SINGLE, OUTPUT_TYPE_ALONE, -1);	//设置PWM输出的参数，OCREFA_TYPE_SINGLE：单点比较； OUTPUT_TYPE_COMP：PWM输出类型互补； 死区时间：0
	ATIM_CtrlPWMOutputs(ENABLE);										//PWM输出使能
	ATIM_Cmd(ENABLE);													//开启ATIM定时器

	CW_ATIM->MSCR_f.CCDS = 1;    //DMA 总开关
	ATIM_OC4Init(ENABLE, ATIM_OC_IT_UP_COUNTER, ENABLE, DISABLE, DISABLE);	//通道4的比较设置
	ATIM_SetCompare4(Rel/2);    //触发时刻
	ATIM_Cmd(ENABLE); 
}

//========================================================================
// 函数: void ATIM_IRQHandler(void)
// 描述: 定时器中断处理函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月8日
//========================================================================
void ATIM_IRQHandler(void)
{
	if (ATIM_GetITStatus(ATIM_IT_OVF))
	{
		ATIM_ClearITPendingBit(ATIM_IT_OVF);
	}
}

