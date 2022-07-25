#ifndef __ADC_H
#define __ADC_H

#include "config.h"

#define ADC_GPIO_CLK_ENABLE		__RCC_GPIOA_CLK_ENABLE()
#define ADC_CLK_ENABLE			__RCC_ADC_CLK_ENABLE()
#define ADC_DMA_CLK_ENABLE		__RCC_DMA_CLK_ENABLE()

//配置ADC测试IO口
#define ADC_GPIO_ANALOG_ENABLE	    PA03_ANALOG_ENABLE(); \
									PA04_ANALOG_ENABLE(); \
									PA06_ANALOG_ENABLE(); \
									PA07_ANALOG_ENABLE()

extern uint16_t SampleData[4];	//母线电压 NTC 瞬时电流 平均电流 

void ADC_Configuration(void);

#endif


