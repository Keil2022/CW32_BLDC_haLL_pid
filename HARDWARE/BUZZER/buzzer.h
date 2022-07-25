#ifndef __BUZZER_H
#define __BUZZER_H

#include "config.h"

#define BUZZER_CLK_ENABLE	__RCC_GPIOB_CLK_ENABLE()

#define BUZZER_GPIO 		CW_GPIOB
#define BUZZER_GPIO_PIN 	(GPIO_PIN_2)
#define BUZZER_GPIO_SPEED 	GPIO_SPEED_HIGH
#define BUZZER_GPIO_MODE	GPIO_MODE_OUTPUT_PP

void Buzzer_Init(void);

#endif


