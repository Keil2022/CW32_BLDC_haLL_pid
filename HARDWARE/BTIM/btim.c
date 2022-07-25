#include "btim.h"
#include "task.h"
#include "app.h"

//========================================================================
// ����: void BTIM1_Configuration(void)
// ����: ������ʱ�����ú���.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��1��
//========================================================================
void BTIM1_Configuration(void)
{
	BTIM_TimeBaseInitTypeDef BTIM_TimeBaseInitStruct;
	__RCC_BTIM_CLK_ENABLE();
	
	__disable_irq();			//�ر��ж�
	NVIC_EnableIRQ(BTIM1_IRQn);	//ʹ��BTIM1�ж�
	__enable_irq();				//���ж�
	
	BTIM_TimeBaseInitStruct.BTIM_Mode = BTIM_Mode_TIMER;			//����ģʽ��00����ʱ��ģʽ
	BTIM_TimeBaseInitStruct.BTIM_OPMode = BTIM_OPMode_Repetitive;	//����ģʽ����
	BTIM_TimeBaseInitStruct.BTIM_Period = 1999;						//��װ��ֵ
	BTIM_TimeBaseInitStruct.BTIM_Prescaler = BTIM_PRS_DIV32;		//ʱ�ӷ�Ƶϵ����t = PCLK/((2^BTIM_PRS_DIVx)*(Period+1))

	BTIM_TimeBaseInit(CW_BTIM1, &BTIM_TimeBaseInitStruct);			//����BTIM1��ʱ��
	BTIM_ITConfig(CW_BTIM1, BTIM_IT_OV, ENABLE);					//ʹ��BTIM1�ж�
	BTIM_Cmd(CW_BTIM1, ENABLE);										//ʹ�ܶ�ʱ��
}

//========================================================================
// ����: void BTIM1_IRQHandleCallback(void)
// ����: ��ʱ���жϴ�����.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��1��
//========================================================================
void BTIM1_IRQHandler(void)
{
	if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_OV))			//���BTIM��״̬�Ĵ�����״̬λ�Ƿ���λ
	{
		BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_OV);	//����жϱ�־λ
		Task_Marks_Handler_Callback();					//ִ�г���
		
		if(Count_1 <255) Count_1++;
//		if(++t >= 100)
//		{
//			t = 0;
//			PC13_TOG();
//			Task_Marks_Handler_Callback();
//		}
	}
}




