#include "global.h"

unsigned int DIin = 0; 		//电机电流偏置
unsigned int CanshuV = 0;	//母线电压100mV为单位
unsigned int CanshuI = 0;	//母线电流mA

const uint8_t STEP_TAB[6]={1,3,2,5,0,4};	//{1,3,2,6,4,5}

uint16_t Err_Code = 0;		//电机运转错误代码
uint16_t Cur_step = 0; 		//当前HALL状态
uint16_t Motor_Start_F = 0; //电机启动运行标志
uint16_t OutPwm = 0;  		//输出PWM

unsigned int  ISH = 8;	//过流报警值 A
unsigned int  VSH = 40;	//过压报警值 V
unsigned int  LSH = 9;	//欠压报警值 V


