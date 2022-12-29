#ifndef __KEY_H
#define __KEY_H

#include "config.h"

#define KEY_CLK_ENABLE	__RCC_GPIOF_CLK_ENABLE()	//Ê¹ÄÜ GPIOF Ê±ÖÓ

#define KEY_GPIO 		CW_GPIOF
#define KEY_GPIO_PIN 	(GPIO_PIN_6 | GPIO_PIN_7)
#define KEY_GPIO_SPEED 	GPIO_SPEED_HIGH
#define KEY_GPIO_MODE	GPIO_MODE_INPUT_PULLUP

#define KEY_1			PF06_GETVALUE()
#define KEY_2			PF07_GETVALUE()

void KEY_Init(void);
void KEY_Handle(void);

extern uint8_t key1_count, key2_count;

#endif

