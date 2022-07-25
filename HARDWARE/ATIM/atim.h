#ifndef __ATIM_H
#define __ATIM_H

#include "config.h"

//GPIOB
#define GPIO_Down_Tube			CW_GPIOB
#define GPIO_Down_Tube_MODE		GPIO_MODE_OUTPUT_PP
#define GPIO_Down_Tube_PIN		(GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15)
#define GPIO_Down_Tube_SPEED	GPIO_SPEED_HIGH

//GPIOA
#define GPIO_Top_Tube			CW_GPIOA
#define GPIO_Top_Tube_MODE		GPIO_MODE_OUTPUT_PP
#define GPIO_Top_Tube_PIN		(GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10)
#define GPIO_Top_Tube_SPEED		GPIO_SPEED_HIGH

#define Pre ATIM_Prescaler_DIV1		//预分频系数	2^n(0~7)
#define Rel 3199					//ARR重装载值，取值范围0x0000~0xFFFF	PWM_Frequency = 64M/(PR*(TS+1))	= 20k

void ATIM_Configuration(void);

#endif


