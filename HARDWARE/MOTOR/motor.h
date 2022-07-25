#ifndef __MOTOR_H
#define __MOTOR_H

#include "config.h"
#include "atim.h"

//U-: PB13 
//V-: PB14
//W-: PB15
//上管调制，下管开关控制, 上管高电平开关管导通
#define PWM_AL_OFF	PB13_SETLOW()	//端口清0
#define PWM_BL_OFF 	PB14_SETLOW()
#define PWM_CL_OFF 	PB15_SETLOW()

#define PWM_AL_ON 	PB13_SETHIGH()	//端口置位
#define PWM_BL_ON 	PB14_SETHIGH()
#define PWM_CL_ON 	PB15_SETHIGH()

extern const uint8_t STEP_TAB[6];

extern uint16_t Err_Code;		//电机运转错误代码
extern uint16_t Cur_step; 		//当前HALL状态
extern uint16_t Motor_Start_F; 	//电机启动运行标志
extern uint16_t OutPwm;  		//输出PWM

#define RelPWM		(Rel+1)
#define OUTMAXPWM  	RelPWM			//最大占空比
#define OUTMINPWM  	(RelPWM*0.05) 	//最小占空比

void Commutation(uint16_t step, uint16_t Out_Pwm_Value, uint16_t Pwm_On_Flag);
uint8_t HALL_Check(void);
void HALL_Motor_Start(void);

#endif

