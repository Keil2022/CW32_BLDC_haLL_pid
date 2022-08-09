#include "global.h"

unsigned int DIin = 0; 		//电机电流偏置
unsigned int CanshuV = 0;	//母线电压100mV为单位
unsigned int CanshuI = 0;	//母线电流mA

const unsigned char STEP_TAB[2][6]={{4,0,5,2,3,1},{1,3,2,5,0,4}};

uint16_t Err_Code = 0;		//电机运转错误代码
uint16_t Cur_step = 0; 		//当前HALL状态
uint16_t Motor_Start_F = 0; //电机启动运行标志
uint16_t OutPwm = 0;  		//输出PWM

unsigned char Dir = 0; 		//方向控制

unsigned int  ISH = 8;	//过流报警值 A
unsigned int  VSH = 40;	//过压报警值 V
unsigned int  LSH = 9;	//欠压报警值 V

unsigned int  MPolePairs = 2;  	//极对数
unsigned int  MAXSPEED = 5000; 	//额定转速

//速度闭环参数  实际缩小1000倍
unsigned int  MINSPEED = 150;  		//最低转速
unsigned int SP=50, SI=50, SD=50;
float KKN = 0;	//计算电位器速度时的斜率

unsigned int SetSpeed = 0;	//电机速度

volatile uint8_t Clock_Counter;
volatile uint8_t Count_1 = 255, Count_2 = 255;

unsigned int Out_Pwm_Value = 0;//输出占空比值

int16_t PWM_Duty_Load = 0;	//PWM加载值
uint16_t PWM_Duty_Set;		//PWM变化值
int16_t  CNT_Value, CNT_uValue;

uint16_t HALLcount = 0, HALLcount1 = 0;		//霍尔脉冲
uint16_t RealS;								//速度


