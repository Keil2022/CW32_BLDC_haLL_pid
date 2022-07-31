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

//U+: PA8
//V+: PA9
//C+: PA10
#define PWM_AH_OFF	PA08_SETLOW();
#define PWM_BH_OFF	PA09_SETLOW();
#define PWM_CH_OFF	PA10_SETLOW();

#define PWM_AH_ON	PA08_SETHIGH();
#define PWM_BH_ON	PA09_SETHIGH();
#define PWM_CH_ON	PA10_SETHIGH();

//U-: PB13 
//V-: PB14
//W-: PB15
//�Ϲܵ��ƣ��¹ܿ��ؿ���, �Ϲܸߵ�ƽ���عܵ�ͨ
#define PWM_AL_OFF	PB13_SETLOW()	//�˿���0
#define PWM_BL_OFF 	PB14_SETLOW()
#define PWM_CL_OFF 	PB15_SETLOW()

#define PWM_AL_ON 	PB13_SETHIGH()	//�˿���λ
#define PWM_BL_ON 	PB14_SETHIGH()
#define PWM_CL_ON 	PB15_SETHIGH()

#define Pre ATIM_Prescaler_DIV1		//Ԥ��Ƶϵ��	2^n(0~7)
#define Rel 3199					//ARR��װ��ֵ��ȡֵ��Χ0x0000~0xFFFF	PWM_Frequency = 64M/(PR*(TS+1))	= 20k

void ATIM_Configuration(void);

#endif


