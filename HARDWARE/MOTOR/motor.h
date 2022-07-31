#ifndef __MOTOR_H
#define __MOTOR_H

#include "config.h"
#include "atim.h"

extern const uint8_t STEP_TAB[6];

extern uint16_t Err_Code;		//�����ת�������
extern uint16_t Cur_step; 		//��ǰHALL״̬
extern uint16_t Motor_Start_F; 	//����������б�־
extern uint16_t OutPwm;  		//���PWM

#define RelPWM		(Rel+1)
#define OUTMAXPWM  	RelPWM			//���ռ�ձ�
#define OUTMINPWM  	(RelPWM*0.05) 	//��Сռ�ձ�

void Commutation(uint16_t step, uint16_t Out_Pwm_Value, uint16_t Pwm_On_Flag);
uint8_t HALL_Check(void);
void HALL_Motor_Start(void);

#endif

