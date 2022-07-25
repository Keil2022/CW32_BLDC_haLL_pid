#include "encoder.h"

volatile uint8_t Clock_Counter;
volatile uint8_t Count_1 = 255, Count_2 = 255;

int16_t PWM_Duty_Load = 0;	//PWM����ֵ
uint16_t PWM_Duty_Set;		//PWM�仯ֵ
int16_t  CNT_Value, CNT_uValue;

//========================================================================
// ����: void Encoder_Init(void)
// ����: ��ת��������ʼ��.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��9��
//========================================================================
void Encoder_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

    ENCODER_CLK_ENABLE;    // ʹ��GPIO������ʱ��

    GPIO_InitStruct.IT = GPIO_IT_Mode;				//�½��ش����ж�
    GPIO_InitStruct.Mode = ENCODER_GPIO_MODE;		//GPIO����
    GPIO_InitStruct.Pins = ENCODER_GPIO_PIN;		//GPIO�˿�
    GPIO_InitStruct.Speed = ENCODER_GPIO_SPEED;		//GPIO�ٶ�
    GPIO_Init(ENCODER_GPIO, &GPIO_InitStruct);		

	GPIO_InitStruct.IT = GPIO_IT_NONE;				//�޴����ж�
	GPIO_InitStruct.Pins = ENCODER_GPIO_PIN_1;		//GPIO�˿�
	GPIO_Init(ENCODER_GPIO, &GPIO_InitStruct);

#if Encoder_Mode
	GTIM1_Configuration();
#else
    //�����ж��˲�
    GPIO_ConfigFilter(ENCODER_GPIO, ENCODER_GPIO_PIN, GPIO_FLTCLK_RC150K);

    //���PA00�жϱ�־��ʹ��NVIC
	__disable_irq(); 
    GPIOB_INTFLAG_CLR(ENCODER_GPIO_PIN);
	NVIC_SetPriority(GPIOB_IRQn, 1);	//���ȼ���1
    NVIC_EnableIRQ(GPIOB_IRQn);
	__enable_irq();
#endif
}

//========================================================================
// ����: void GPIOB_IRQHandler(void)
// ����: GPIOB�ⲿ�жϴ�����.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��1��
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
// ����: void GTIM1_Configuration(void)
// ����: ��ʱ��GTIM1���ú���.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��14��
//========================================================================
void GTIM1_Configuration(void)
{
	GTIM_EncodeInitTypeDef GTIM_EncodeInitStruct;
	GTIM_InitTypeDef GTIM_InitStruct;
	
	GTIM_CLK_ENABLE;
	
	PB04_AFx_GTIM1CH1();
	PB05_AFx_GTIM1CH2();
	
	GTIM_InitStruct.Mode = GTIM_MODE_TIME;				//GTIM��ʱ��ģʽ
	GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;	//GTIM����������ģʽ	
	GTIM_InitStruct.Prescaler = GTIM_PRESCALER_DIV1;	//GTIM��Ԥ��Ƶϵ����
	GTIM_InitStruct.ReloadValue = GTIM1_ARR;			//GTIM������ֵ��
	GTIM_InitStruct.ToggleOutState = DISABLE;			//GTIM�ķ�ת�����ʹ�ܡ�
	GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct); 		//����GTIMx��ʱ��
	
	GTIM_EncodeInitStruct.EncodeMode = GTIM_ENCODE_MODE3;				//00����ʱ�������� MODE λ����
																		//01���������ģʽ 1��CH1 �źű仯�ؼ���
																		//10���������ģʽ 2��CH2 �źű仯�ؼ���
																		//11���������ģʽ 3��CH1/CH2 �źű仯�ؼ���
	GTIM_EncodeInitStruct.EncodeReloadMode = GTIM_ENCODE_RESET_NONE;	//�� ���ؼ����� CNT ��ֵ
	GTIM_EncodeInitStruct.EncodeResetMode = GTIM_ENCODE_RESET_NONE;		//�� ��λ������ CNT ��ֵ
	
	GTIM_EncodeInitStruct.CH1Filter = GTIM_CHx_FILTER_PCLK_N2;
	GTIM_EncodeInitStruct.CH1Invert = GTIM_CHx_INVERT_ON;
	
	GTIM_EncodeInitStruct.CH2Filter = GTIM_CHx_FILTER_PCLK_N2;
	GTIM_EncodeInitStruct.CH2Invert = GTIM_CHx_INVERT_ON;
	GTIM_EncodeInit(CW_GTIM1, &GTIM_EncodeInitStruct);
	
	GTIM_ITConfig(CW_GTIM1, GTIM_IT_DIRCHANGE, ENABLE);					//��������ı��ж�
	GTIM_Cmd(CW_GTIM1, ENABLE);
	
	//����жϱ�־��ʹ��NVIC
	__disable_irq(); 
	NVIC_SetPriority(GTIM1_IRQn, 2);			//���ȼ���2
    NVIC_EnableIRQ(GTIM1_IRQn);
	__enable_irq();
}


//========================================================================
// ����: void GTIM1_IRQHandler(void)
// ����: ��ʱ��GTIM1�жϴ�����.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��14��
//========================================================================
void GTIM1_IRQHandler(void)
{
	if(GTIM_GetITStatus(CW_GTIM1, GTIM_IT_DIRCHANGE))
	{
		GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_DIRCHANGE);
	}
}


//========================================================================
// ����: uint32_t GTIM_SetCounterValue(GTIM_TypeDef *GTIMx)
// ����: ��ȡ����ֵ.
// ����: GTIMx ��ʱ��.
// ����: ����ֵ.
// �汾: V1.0, 2022��6��14��
//========================================================================
uint32_t GTIM_Reed_CNT_Value(GTIM_TypeDef *GTIMx)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));

    return GTIMx->CNT;
}

/*����ֵ����*/
int GFP_abs(int p)
{
	int q;
	q=p>0?p:(-p);
	return q;
}

//========================================================================
// ����: void Calculate_PWM_duty(void)
// ����: ����PWMռ�ձ�.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��19��
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

