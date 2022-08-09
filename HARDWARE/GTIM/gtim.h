#ifndef __GTIM_H
#define __GTIM_H

#include "config.h"
#include "global.h"

#define GPIO_HALL_A 		CW_GPIOA
#define GPIO_HALL_A_MODE	GPIO_MODE_INPUT_PULLUP	//上拉输入
#define GPIO_HALL_A_PIN		(GPIO_PIN_15 | GPIO_PIN_2)
#define GPIO_HALL_A_SPEED	GPIO_SPEED_HIGH

#define GPIO_HALL_B 		CW_GPIOB
#define GPIO_HALL_B_MODE	GPIO_MODE_INPUT_PULLUP	//上拉输入
#define GPIO_HALL_B_PIN		GPIO_PIN_3
#define GPIO_HALL_B_SPEED	GPIO_SPEED_HIGH

#define MotorPoles	2		//极对数

void GTIM2_Configuration(void);
void Speed_Count(void);

#endif


