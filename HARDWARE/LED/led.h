#ifndef __LED_H
#define __LED_H

#include "config.h"

#define LED_CLK_ENABLE	__RCC_GPIOA_CLK_ENABLE()

#define LED_GPIO 		CW_GPIOA
#define LED_GPIO_PIN 	(GPIO_PIN_11 | GPIO_PIN_12)
#define LED_GPIO_SPEED 	GPIO_SPEED_HIGH
#define LED_GPIO_MODE	GPIO_MODE_OUTPUT_PP

void LED_Init(void);

#endif


