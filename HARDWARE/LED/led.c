#include "led.h"

//========================================================================
// 函数: void LED_Init(void)
// 描述: LED初始化.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月1日
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


