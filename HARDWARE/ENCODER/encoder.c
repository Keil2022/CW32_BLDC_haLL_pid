#include "encoder.h"

volatile uint8_t Clock_Counter;
volatile uint8_t Count_1 = 255, Count_2 = 255;

int16_t PWM_Duty_Load = 0;	//PWM加载值
uint16_t PWM_Duty_Set;		//PWM变化值
int16_t  CNT_Value, CNT_uValue;

//========================================================================
// 函数: void Encoder_Init(void)
// 描述: 旋转编码器初始化.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月9日
//========================================================================
void Encoder_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

    ENCODER_CLK_ENABLE;    // 使能GPIO的配置时钟

    GPIO_InitStruct.IT = GPIO_IT_Mode;				//下降沿触发中断
    GPIO_InitStruct.Mode = ENCODER_GPIO_MODE;		//GPIO输入
    GPIO_InitStruct.Pins = ENCODER_GPIO_PIN;		//GPIO端口
    GPIO_InitStruct.Speed = ENCODER_GPIO_SPEED;		//GPIO速度
    GPIO_Init(ENCODER_GPIO, &GPIO_InitStruct);		

	GPIO_InitStruct.IT = GPIO_IT_NONE;				//无触发中断
	GPIO_InitStruct.Pins = ENCODER_GPIO_PIN_1;		//GPIO端口
	GPIO_Init(ENCODER_GPIO, &GPIO_InitStruct);

#if Encoder_Mode
	GTIM1_Configuration();
#else
    //配置中断滤波
    GPIO_ConfigFilter(ENCODER_GPIO, ENCODER_GPIO_PIN, GPIO_FLTCLK_RC150K);

    //清除PA00中断标志并使能NVIC
	__disable_irq(); 
    GPIOB_INTFLAG_CLR(ENCODER_GPIO_PIN);
	NVIC_SetPriority(GPIOB_IRQn, 1);	//优先级：1
    NVIC_EnableIRQ(GPIOB_IRQn);
	__enable_irq();
#endif
}

//========================================================================
// 函数: void GPIOB_IRQHandler(void)
// 描述: GPIOB外部中断处理函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月1日
//========================================================================
void GPIOB_IRQHandler(void)
{
//    if (CW_GPIOB->ISR_f.PIN5)
//    {
//        //GPIOB_INTFLAG_CLR(bv5);
//		GPIOB_INTFLAG_CLR(GPIO_PIN_5);
//		
//		//Clock_Counter = 1;
//		
//		if( PB04_GETVALUE() == 1 )
//		{
//			Clock_Counter = 1;
//		}
//    }
	
    if (CW_GPIOB->ISR_f.PIN4)
    {
        //GPIOB_INTFLAG_CLR(bv4);
		GPIOB_INTFLAG_CLR(GPIO_PIN_4);
		
		//Clock_Counter = 0;
		
		if( PB05_GETVALUE() == 0 )
		{
			Clock_Counter = 0;
		}
		else
		{
			Clock_Counter = 1;
		}
		
		//	GPIOB_INTFLAG_CLR(bv5 | bv4);
		//	if( PB04_GETVALUE() == 0 )
		//		if( PB05_GETVALUE() == 1 )
		//			Clock_Counter = 1;
		//		
		//	if( PB05_GETVALUE() == 0 )
		//		if( PB04_GETVALUE() == 1 )
		//			Clock_Counter = 0;
			
		Count_2 = Count_2*3/4 + Count_1/4;
		Count_1 = 0;
		
		if( Count_2 > 200 )			PWM_Duty_Set = 5;
		else if( Count_2 > 120 )	PWM_Duty_Set = 20;
		else if( Count_2 > 60 )		PWM_Duty_Set = 40;
		else if( Count_2 > 30 )		PWM_Duty_Set = 80;
		else if( Count_2 > 15 )		PWM_Duty_Set = 160;
		else 						PWM_Duty_Set = 320;
		
		
		if( Clock_Counter )
		{
			PWM_Duty_Load += PWM_Duty_Set;
			PWM_Duty_Load = PWM_Duty_Load > OUTMAXPWM ? OUTMAXPWM : PWM_Duty_Load;
		}
		else
		{
			PWM_Duty_Load -= PWM_Duty_Set;
			PWM_Duty_Load = PWM_Duty_Load < 0 ? 0 : PWM_Duty_Load;
		}
    }
}


//========================================================================
// 函数: void GTIM1_Configuration(void)
// 描述: 定时器GTIM1配置函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月14日
//========================================================================
void GTIM1_Configuration(void)
{
	GTIM_EncodeInitTypeDef GTIM_EncodeInitStruct;
	GTIM_InitTypeDef GTIM_InitStruct;
	
	GTIM_CLK_ENABLE;
	
	PB04_AFx_GTIM1CH1();
	PB05_AFx_GTIM1CH2();
	
	GTIM_InitStruct.Mode = GTIM_MODE_TIME;				//GTIM定时器模式
	GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;	//GTIM的连续计数模式	
	GTIM_InitStruct.Prescaler = GTIM_PRESCALER_DIV1;	//GTIM的预分频系数。
	GTIM_InitStruct.ReloadValue = GTIM1_ARR;			//GTIM的重载值。
	GTIM_InitStruct.ToggleOutState = DISABLE;			//GTIM的翻转输出不使能。
	GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct); 		//配置GTIMx定时器
	
	GTIM_EncodeInitStruct.EncodeMode = GTIM_ENCODE_MODE3;				//00：定时器功能由 MODE 位配置
																		//01：编码计数模式 1，CH1 信号变化沿计数
																		//10：编码计数模式 2，CH2 信号变化沿计数
																		//11：编码计数模式 3，CH1/CH2 信号变化沿计数
	GTIM_EncodeInitStruct.EncodeReloadMode = GTIM_ENCODE_RESET_NONE;	//不 重载计数器 CNT 的值
	GTIM_EncodeInitStruct.EncodeResetMode = GTIM_ENCODE_RESET_NONE;		//不 复位计数器 CNT 的值
	
	GTIM_EncodeInitStruct.CH1Filter = GTIM_CHx_FILTER_PCLK_N2;
	GTIM_EncodeInitStruct.CH1Invert = GTIM_CHx_INVERT_ON;
	
	GTIM_EncodeInitStruct.CH2Filter = GTIM_CHx_FILTER_PCLK_N2;
	GTIM_EncodeInitStruct.CH2Invert = GTIM_CHx_INVERT_ON;
	GTIM_EncodeInit(CW_GTIM1, &GTIM_EncodeInitStruct);
	
	GTIM_ITConfig(CW_GTIM1, GTIM_IT_DIRCHANGE, ENABLE);					//开启方向改变中断
	GTIM_Cmd(CW_GTIM1, ENABLE);
	
	//清除中断标志并使能NVIC
	__disable_irq(); 
	NVIC_SetPriority(GTIM1_IRQn, 2);			//优先级：2
    NVIC_EnableIRQ(GTIM1_IRQn);
	__enable_irq();
}


//========================================================================
// 函数: void GTIM1_IRQHandler(void)
// 描述: 定时器GTIM1中断处理函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月14日
//========================================================================
void GTIM1_IRQHandler(void)
{
	if(GTIM_GetITStatus(CW_GTIM1, GTIM_IT_DIRCHANGE))
	{
		GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_DIRCHANGE);
	}
}


//========================================================================
// 函数: uint32_t GTIM_SetCounterValue(GTIM_TypeDef *GTIMx)
// 描述: 读取计数值.
// 参数: GTIMx 定时器.
// 返回: 计数值.
// 版本: V1.0, 2022年6月14日
//========================================================================
uint32_t GTIM_Reed_CNT_Value(GTIM_TypeDef *GTIMx)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));

    return GTIMx->CNT;
}

/*绝对值函数*/
int GFP_abs(int p)
{
	int q;
	q=p>0?p:(-p);
	return q;
}

//========================================================================
// 函数: void Calculate_PWM_duty(void)
// 描述: 计算PWM占空比.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月19日
//========================================================================
void Calculate_PWM_duty(void)
{
#if Encoder_Mode
	
	CNT_Value = GTIM_Reed_CNT_Value(CW_GTIM1);
	GTIM_SetCounterValue(CW_GTIM1, 0);
	CNT_uValue = GFP_abs(CNT_Value);
	if(CNT_uValue > 10)		PWM_Duty_Set = 320;
	else if(CNT_uValue > 7)	PWM_Duty_Set = 224;
	else if(CNT_uValue > 4)	PWM_Duty_Set = 160;
	else if(CNT_uValue > 2)	PWM_Duty_Set = 24;
	else if(CNT_uValue > 0)	PWM_Duty_Set = 8;
	else {/*do nothing*/;}
	
	if( CNT_Value > 0 )			PWM_Duty_Load += PWM_Duty_Set;
	else if( CNT_Value < 0 )	PWM_Duty_Load -= PWM_Duty_Set;
	else {/*do nothing*/;}

	PWM_Duty_Load = PWM_Duty_Load > OUTMAXPWM ? OUTMAXPWM : PWM_Duty_Load;
	PWM_Duty_Load = PWM_Duty_Load < 0 ? 0 : PWM_Duty_Load;
#endif
}

