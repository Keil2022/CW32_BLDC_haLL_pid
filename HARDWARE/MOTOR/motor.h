#ifndef __MOTOR_H
#define __MOTOR_H

#include "config.h"
#include "atim.h"
#include "global.h"
#include "encoder.h"

#define RelPWM		(Rel+1)
#define OUTMAXPWM  	RelPWM			//���ռ�ձ�
#define OUTMINPWM  	(RelPWM*0.05) 	//��Сռ�ձ�

void Commutation(uint16_t step, uint16_t Pwm_On_Flag);
uint8_t HALL_Check(void);
void HALL_Motor_Start(void);
void Set_Speed(void);
void UPPWM(void);

#endif

