#ifndef __MOTOR_H
#define __MOTOR_H

#include "config.h"
#include "atim.h"
#include "global.h"

#define RelPWM		(Rel+1)
#define OUTMAXPWM  	RelPWM			//最大占空比
#define OUTMINPWM  	(RelPWM*0.05) 	//最小占空比

void Commutation(uint16_t step, uint16_t Out_Pwm_Value, uint16_t Pwm_On_Flag);
uint8_t HALL_Check(void);
void HALL_Motor_Start(void);

#endif

