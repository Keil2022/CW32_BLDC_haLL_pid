#include "gtim.h"
#include "app.h"

//HA: PA15
//HB: PB3
//HC: PA2

//========================================================================
// ����: void GTIM2_Configuration(void)
// ����: ��ʱ��GTIM2���ú���.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��8��
//========================================================================
void GTIM2_Configuration(void)
{
	GTIM_InitTypeDef 	GTIM_InitStruct;
	GTIM_ICInitTypeDef 	GTIM_ICInitStruct;
	GPIO_InitTypeDef 	GPIO_InitStruct;
	
	__RCC_GTIM2_CLK_ENABLE();	//����GTIM2ʱ��
	__RCC_GPIOA_CLK_ENABLE();	//����GPIOAʱ��
	__RCC_GPIOB_CLK_ENABLE();	//����GPIOBʱ��
	
	PA15_AFx_GTIM2CH1();		//  GTIM2CH1();	�������ù���
	PB03_AFx_GTIM2CH2();		//  GTIM2CH2();
	PA02_AFx_GTIM2CH3();		//	GTIM2CH3();		
	
	GPIO_InitStruct.IT = GPIO_IT_NONE;
	GPIO_InitStruct.Mode = GPIO_HALL_A_MODE;		//GPIOģʽ
	GPIO_InitStruct.Pins = GPIO_HALL_A_PIN;			//GPIO�˿�
	GPIO_InitStruct.Speed = GPIO_HALL_A_SPEED;		//GPIO�ٶ�
	GPIO_Init(GPIO_HALL_A, &GPIO_InitStruct);		//����GPIO
	
	GPIO_InitStruct.IT = GPIO_IT_NONE;
	GPIO_InitStruct.Mode = GPIO_HALL_B_MODE;		//GPIOģʽ
	GPIO_InitStruct.Pins = GPIO_HALL_B_PIN;			//GPIO�˿�
	GPIO_InitStruct.Speed = GPIO_HALL_B_SPEED;		//GPIO�ٶ�
	GPIO_Init(GPIO_HALL_B, &GPIO_InitStruct);		//����GPIO
	
	__disable_irq(); 
	NVIC_SetPriority(GTIM2_IRQn, 0);	//���ȼ���0�����
	NVIC_EnableIRQ(GTIM2_IRQn); 		//�����ж�
	__enable_irq();
	
	GTIM_InitStruct.Mode = GTIM_MODE_TIME;				//GTIM��ʱ��ģʽ
	GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;	//GTIM����������ģʽ	
	GTIM_InitStruct.Prescaler = GTIM_PRESCALER_DIV1;	//GTIM��Ԥ��Ƶϵ����
	GTIM_InitStruct.ReloadValue = 0xFFFF;				//GTIM������ֵ��
	GTIM_InitStruct.ToggleOutState = DISABLE;			//GTIM�ķ�ת�����ʹ�ܡ�
	GTIM_TimeBaseInit(CW_GTIM2, &GTIM_InitStruct); 		//����GTIM2��ʱ��
	
	GTIM_ICInitStruct.CHx = GTIM_CHANNEL1;						//ͨ��1
	GTIM_ICInitStruct.ICFilter = GTIM_CHx_FILTER_PCLK_N2;		//�����ź��˲����ã�ʱ�ӣ�PCLK; ����������2
	GTIM_ICInitStruct.ICInvert = GTIM_CHx_INVERT_OFF;			//�رշ�ת
	GTIM_ICInitStruct.ICPolarity = GTIM_ICPolarity_BothEdge;	//˫��
	GTIM_ICInit(CW_GTIM2, &GTIM_ICInitStruct);					//����Ƚϳ�ʼ��

	GTIM_ICInitStruct.CHx = GTIM_CHANNEL2;						//ͨ��2
	GTIM_ICInit(CW_GTIM2, &GTIM_ICInitStruct);					//����Ƚϳ�ʼ��

	GTIM_ICInitStruct.CHx = GTIM_CHANNEL3;						//ͨ��3
	GTIM_ICInit(CW_GTIM2, &GTIM_ICInitStruct);					//����Ƚϳ�ʼ��

	GTIM_ITConfig(CW_GTIM2, GTIM_IT_CC1 | GTIM_IT_CC2 | GTIM_IT_CC3, ENABLE);	//�ж�ʹ��
	GTIM_Cmd(CW_GTIM2, ENABLE);													//��ʱ��ʹ��
}

//========================================================================
// ����: void GTIM2_IRQHandler(void)
// ����: �жϴ�����.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��8��
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
	
	HALLcount++;HALLcount1++;						//�����������	
	x = HALL_Check(); 								//��ȡ����״̬
	Cur_step = STEP_TAB[Dir][x-1];					//��ȡ������λ		
	if( Motor_Start_F==1 && Err_Code==0 ) 			//������ͣ״̬ ����
		Commutation(Cur_step,Motor_Start_F);  
}

//========================================================================
// ����: void Speed_Count(void)
// ����: �ٶȼ��㺯��.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��10��
//========================================================================
void Speed_Count(void)
{
	//�����ٶ�
	RealS = HALLcount*50 / MotorPoles;	//����ʵ����RPM
	HALLcount = 0;
	
	if( MOTORSTATE == STATESTARTPID || MOTORSTATE == STATERUNPID )
	{
		if(RealS < 120)
		{
			DZCount++;
			if(DZCount >= 10)	//2S����
			{ 								
				DZCount = 0;
				Err_Code = 7;	//��ת �ٶȵ͵Ĺ����ж�
			}
		}
		else DZCount = 0;
	}
}


