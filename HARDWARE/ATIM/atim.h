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

#define PWM_UH_ON	PA08_SETHIGH();
#define PWM_UH_OFF	PA08_SETLOW();

#define PWM_VH_ON	PA09_SETHIGH();
#define PWM_VH_OFF	PA09_SETLOW();

#define PWM_WH_ON	PA10_SETHIGH();
#define PWM_WH_OFF	PA10_SETLOW();

#define PWM_UL_ON	PB13_SETHIGH();
#define PWM_UL_OFF	PB13_SETLOW();

#define PWM_VL_ON	PB14_SETHIGH();
#define PWM_VL_OFF	PB14_SETLOW();

#define PWM_WL_ON	PB15_SETHIGH();
#define PWM_WL_OFF	PB15_SETLOW();

#define Pre ATIM_Prescaler_DIV1		//预分频系数	2^n(0~7)
#define Rel 3199					//ARR重装载值，取值范围0x0000~0xFFFF	PWM_Frequency = 64M/(PR*(TS+1))	= 20k

void ATIM_Configuration(void);

#endif


