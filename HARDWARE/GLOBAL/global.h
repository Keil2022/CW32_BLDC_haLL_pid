//#ifndef __GLOBAL_H
//#define __GLOBAL_H

#include "config.h"

//过压值设定
#define RV1 5.1
#define RV2	200
//输入电压与总线电压之比：为RV1:(RV1+RV2)

#define Kvvalue (float)1.2/RV1*(RV1+RV2)

//过流过压错误认定次数
#define NumErr 15

extern unsigned int DIin; 		//电机电流偏置
extern unsigned int CanshuV;	//母线电压100mV为单位
extern unsigned int CanshuI;	//母线电流mA

extern const uint8_t STEP_TAB[6];

extern uint16_t Err_Code;		//电机运转错误代码
extern uint16_t Cur_step; 		//当前HALL状态
extern uint16_t Motor_Start_F; 	//电机启动运行标志
extern uint16_t OutPwm;  		//输出PWM

extern unsigned int  ISH;	//过流报警值 A
extern unsigned int  VSH;	//过压报警值 V
extern unsigned int  LSH;	//欠压报警值 V

//#endif

