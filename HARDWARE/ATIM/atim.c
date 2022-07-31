#include "atim.h"

//U+: PA8 
//V+: PA9
//W+: PA10

//U-: PB13 
//V-: PB14
//W-: PB15

//�Ϲܵ��ƣ��¹�IO

//========================================================================
// ����: void ATIM_Configuration(void)
// ����: �߼���ʱ��ATIM���ú���.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��8��
//========================================================================
void ATIM_Configuration(void)
{
	ATIM_InitTypeDef 	ATIM_InitStruct;		//�������ó�Ա
	ATIM_OCInitTypeDef 	ATIM_OCInitStruct;
	GPIO_InitTypeDef 	GPIO_InitStruct;
	
	__RCC_ATIM_CLK_ENABLE();	//����ATIMʱ��
	__RCC_GPIOA_CLK_ENABLE();	//����GPIOAʱ��
	__RCC_GPIOB_CLK_ENABLE();	//����GPIOBʱ��
	
	PA08_AFx_ATIMCH1A();	//PA8����ATIMͨ��1
	PA09_AFx_ATIMCH2A();	//PA9����ATIMͨ��2
	PA10_AFx_ATIMCH3A();	//PA10����ATIMͨ��3
	
	GPIO_InitStruct.IT = GPIO_IT_NONE;
	GPIO_InitStruct.Mode = GPIO_Down_Tube_MODE;		//GPIOģʽ
	GPIO_InitStruct.Pins = GPIO_Down_Tube_PIN;		//GPIO�˿�
	GPIO_InitStruct.Speed = GPIO_Down_Tube_SPEED;	//GPIO�ٶ�
	GPIO_Init(GPIO_Down_Tube, &GPIO_InitStruct);	//����GPIO
	
	PWM_AL_OFF;
	PWM_BL_OFF;
	PWM_CL_OFF;
	
	GPIO_InitStruct.IT = GPIO_IT_NONE;
	GPIO_InitStruct.Mode = GPIO_Top_Tube_MODE;		//GPIOģʽ
	GPIO_InitStruct.Pins = GPIO_Top_Tube_PIN;		//GPIO�˿�
	GPIO_InitStruct.Speed = GPIO_Top_Tube_SPEED;	//GPIO�ٶ�
	GPIO_Init(GPIO_Top_Tube, &GPIO_InitStruct);		//����GPIO
	
	PWM_AH_OFF;
	PWM_BH_OFF;
	PWM_CH_OFF;
	
	ATIM_InitStruct.BufferState = ENABLE;								//ARR���治ʹ��
	ATIM_InitStruct.ClockSelect	= ATIM_CLOCK_PCLK;						//����ʱ��ѡ��:PCLK
	ATIM_InitStruct.CounterAlignedMode	= ATIM_COUNT_MODE_EDGE_ALIGN;	//��������ģʽ: ����
	ATIM_InitStruct.CounterDirection = ATIM_COUNTING_UP;				//�����������ϼ���
	ATIM_InitStruct.CounterOPMode = ATIM_OP_MODE_REPETITIVE;			//����ģʽ���ظ�
	ATIM_InitStruct.Prescaler = Pre;									//Ԥ��Ƶϵ��
	ATIM_InitStruct.ReloadValue = Rel;									//ARR��ֵ��ȡֵ��Χ0x0000~0xFFFF
	ATIM_InitStruct.RepetitionCounter = 0;								//�ظ����ڼ���ֵ��RCR�Ĵ���
	ATIM_InitStruct.OverFlowMask = ENABLE;								//���������ʹ��
	ATIM_InitStruct.UnderFlowMask = DISABLE;							//���������ʹ��
	ATIM_Init(&ATIM_InitStruct);
	
	ATIM_OCInitStruct.BufferState = DISABLE;						//�Ƚϻ��治ʹ��
	ATIM_OCInitStruct.OCDMAState = DISABLE;							//�Ƚ�ƥ�䴥��DMA��ʹ��
	ATIM_OCInitStruct.OCInterruptSelect	= ATIM_OC_IT_NONE;			//�Ƚ��ж�ģʽ����
	ATIM_OCInitStruct.OCInterruptState = DISABLE;					//�Ƚ�ƥ�䴥���ж�ʹ��
	ATIM_OCInitStruct.OCMode = ATIM_OCMODE_PWM1;					//�Ƚ�ģʽ���ã�PWM1
	ATIM_OCInitStruct.OCPolarity = ATIM_OCPOLARITY_NONINVERT;	 	//�˿ڼ���ѡ������
	ATIM_OC1AInit(&ATIM_OCInitStruct);	//ͨ���Ƚ��������
	ATIM_OC2AInit(&ATIM_OCInitStruct);
	ATIM_OC3AInit(&ATIM_OCInitStruct);
	
	ATIM_SetCompare1A(0);	//����ռ�ձȣ�д����ֵΪ ��ռ�ձ�
	ATIM_SetCompare2A(0);
	ATIM_SetCompare3A(0);
	
	ATIM_PWMOutputConfig(OCREFA_TYPE_SINGLE, OUTPUT_TYPE_ALONE, -1);	//����PWM����Ĳ�����OCREFA_TYPE_SINGLE������Ƚϣ� OUTPUT_TYPE_COMP��PWM������ͻ����� ����ʱ�䣺0
	ATIM_CtrlPWMOutputs(ENABLE);										//PWM���ʹ��
	ATIM_Cmd(ENABLE);													//����ATIM��ʱ��

	CW_ATIM->MSCR_f.CCDS = 1;    //DMA �ܿ���
	ATIM_OC4Init(ENABLE, ATIM_OC_IT_UP_COUNTER, ENABLE, DISABLE, DISABLE);	//ͨ��4�ıȽ�����
	ATIM_SetCompare4(Rel/2);    //����ʱ��
	ATIM_Cmd(ENABLE); 
}

//========================================================================
// ����: void ATIM_IRQHandler(void)
// ����: ��ʱ���жϴ�����.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��8��
//========================================================================
void ATIM_IRQHandler(void)
{
	if (ATIM_GetITStatus(ATIM_IT_OVF))
	{
		ATIM_ClearITPendingBit(ATIM_IT_OVF);
	}
}

