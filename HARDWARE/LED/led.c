#include "led.h"

//========================================================================
// ����: void LED_Init(void)
// ����: LED��ʼ��.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��1��
//========================================================================
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	LED_CLK_ENABLE;
	
	GPIO_InitStruct.IT = GPIO_IT_NONE;
	GPIO_InitStruct.Mode = LED_GPIO_MODE;
	GPIO_InitStruct.Pins = LED_GPIO_PIN;
	GPIO_InitStruct.Speed = LED_GPIO_SPEED;
	
	GPIO_Init(LED_GPIO, &GPIO_InitStruct);
	LED1_OFF;
	LED2_OFF;
}


