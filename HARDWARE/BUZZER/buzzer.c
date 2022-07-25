#include "buzzer.h"

//========================================================================
// 函数: void Buzzer_Init(void)
// 描述: 蜂鸣器初始化.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月14日
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

