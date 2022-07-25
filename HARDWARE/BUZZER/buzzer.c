#include "buzzer.h"

//========================================================================
// ����: void Buzzer_Init(void)
// ����: ��������ʼ��.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��14��
//========================================================================
void Buzzer_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	BUZZER_CLK_ENABLE;
	
	GPIO_InitStruct.IT = GPIO_IT_NONE;
	GPIO_InitStruct.Mode = BUZZER_GPIO_MODE;
	GPIO_InitStruct.Pins = BUZZER_GPIO_PIN;
	GPIO_InitStruct.Speed = BUZZER_GPIO_SPEED;
	
	GPIO_Init(BUZZER_GPIO, &GPIO_InitStruct);
	PB02_SETLOW();
}

