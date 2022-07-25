#include "iwdt.h"

//========================================================================
// ����: void IWDT_Configuration(void)
// ����: �������Ź���ʼ������.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��7��
//========================================================================
void IWDT_Configuration(void)
{
	IWDT_InitTypeDef IWDT_InitStruct;
	
	__RCC_IWDT_CLK_ENABLE(); //ʹ��IWDTģ��
	
	IWDT_InitStruct.IWDT_ITState = DISABLE;								//���Ź��ж�״̬��ȡֵ��ΧENABLE��DISABLE
	IWDT_InitStruct.IWDT_OverFlowAction = IWDT_OVERFLOW_ACTION_RESET;	//�����Ķ�������λ
	IWDT_InitStruct.IWDT_Pause = IWDT_SLEEP_PAUSE;						//Deepsleepģʽ��IWDT��ͣ����
	IWDT_InitStruct.IWDT_Prescaler = IWDT_Prescaler_DIV128;				//Ԥ����ϵ����ȡֵ��Χ2��n���ݣ�n=2,3..9
	IWDT_InitStruct.IWDT_ReloadValue = 79;  							//���Ź�����ֵ��ȡֵ��ΧС�ڵ���0x0FFF		1S
	IWDT_InitStruct.IWDT_WindowValue = 0xFFF;							//���ڱȽ����Ƚ�ֵ,ȡֵ��ΧС�ڵ���0x0FFF

	IWDT_Init(&IWDT_InitStruct);
	
	IWDT_Cmd();					//����IWDT
	while(!CW_IWDT->SR_f.RUN);	//�ȴ�IWDT����
	IWDT_Refresh();				//ι��
}


