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

#define STATESTARTCHECK 0
#define STATESTARTPID 	1
#define STATERUNPID 	2
#define STATESTOP 		3
#define STATEERROR 		5
#define STATEERROROVER 	6

extern unsigned int DIin; 		//电机电流偏置
extern unsigned int CanshuV;	//母线电压100mV为单位
extern unsigned int CanshuI;	//母线电流mA

extern const unsigned char STEP_TAB[2][6];

extern uint16_t Err_Code;		//电机运转错误代码
extern uint16_t Cur_step; 		//当前HALL状态
extern uint16_t Motor_Start_F; 	//电机启动运行标志
extern uint16_t OutPwm;  		//输出PWM

extern unsigned char Dir; 		//方向控制

extern unsigned int  ISH;	//过流报警值 A
extern unsigned int  VSH;	//过压报警值 V
extern unsigned int  LSH;	//欠压报警值 V

extern unsigned int  MPolePairs;  	//极对数
extern unsigned int  MAXSPEED; 		//额定转速

//速度闭环参数  实际缩小1000倍
extern unsigned int  MINSPEED;  		//最低转速
extern unsigned int SP, SI, SD;

extern float KKN;	//计算电位器速度时的斜率

extern unsigned int SetSpeed;	//电机速度

extern volatile uint8_t Clock_Counter;
extern volatile uint8_t Count_1, Count_2;

extern unsigned int Out_Pwm_Value;//输出占空比值

extern int16_t PWM_Duty_Load;
extern uint16_t PWM_Duty_Set;
extern int16_t  CNT_Value, CNT_uValue;

extern uint16_t RealS;		//速度
extern uint16_t HALLcount, HALLcount1;	//霍尔脉冲

//#endif


