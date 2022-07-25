#include "key.h"

uint8_t key1_count = 0, key2_count = 0;

//========================================================================
// ����: void KEY_Init(void)
// ����: ������ʼ��.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��9��
//========================================================================
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	KEY_CLK_ENABLE;

	GPIO_InitStruct.IT = GPIO_IT_NONE;
	GPIO_InitStruct.Mode = KEY_GPIO_MODE;
	GPIO_InitStruct.Pins = KEY_GPIO_PIN;
	GPIO_InitStruct.Speed = KEY_GPIO_SPEED;
	
	GPIO_Init(KEY_GPIO, &GPIO_InitStruct);
}

//========================================================================
// ����: void KEY_Handle(void)
// ����: ��������.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��9��
//========================================================================
void KEY_Handle(void)
{
	if(KEY_1 == 0)
	{
		if(key1_count < 255) key1_count++;
		if(key1_count > 1)
		{
			
		}
	}
	else key1_count = 0;
	
	if(KEY_2 == 0)
	{
		if(key2_count < 255) key2_count++;
		if(++key2_count > 1)
		{
			
		}
	}
	else key2_count = 0;
}


