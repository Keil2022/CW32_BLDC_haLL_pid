#ifndef __ENCODER_H
#define __ENCODER_H

#include "config.h"
#include "motor.h"

#define Encoder_Mode 0

#define ENCODER_CLK_ENABLE  __RCC_GPIOB_CLK_ENABLE();    // 使能GPIO的配置时钟
#define GTIM_CLK_ENABLE		__RCC_GTIM1_CLK_ENABLE();

#if Encoder_Mode
#define GPIO_IT_Mode GPIO_IT_NONE
#else
#define GPIO_IT_Mode GPIO_IT_FALLING
#endif

#define ENCODER_GPIO 		CW_GPIOB
#define ENCODER_GPIO_PIN 	(GPIO_PIN_4)
#define ENCODER_GPIO_SPEED 	GPIO_SPEED_HIGH
#define ENCODER_GPIO_MODE	GPIO_MODE_INPUT_PULLUP

#define ENCODER_GPIO_PIN_1 	(GPIO_PIN_5)

#define GTIM1_ARR			60000

void Encoder_Init(void);
void GTIM1_Configuration(void);
uint32_t GTIM_Reed_CNT_Value(GTIM_TypeDef *GTIMx);
void Calculate_PWM_duty(void);
int GFP_abs(int p);

#endif


