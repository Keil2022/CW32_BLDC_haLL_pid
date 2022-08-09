#include "motor.h"

//========================================================================
// 函数: void Commutation(uint16_t step, uint16_t Out_Pwm_Value, uint16_t Pwm_On_Flag)
// 描述: 电机控制.
// 参数: steP：为当前换相序号； OutPwmValue：输出PWM值； PWM_ON_flag=1时启动PWM输出.
// 返回: None.
// 版本: V1.0, 2022年6月9日
//========================================================================
void Commutation(uint16_t step, uint16_t Pwm_On_Flag)
{
	if(Pwm_On_Flag == 0)	//关闭输出
	{
		ATIM_SetCompare1A(0);			//控制占空比，写入数值为 正占空比
		ATIM_SetCompare2A(0);
		ATIM_SetCompare3A(0);
		
		ATIM_CtrlPWMOutputs(DISABLE);	//关闭PWM输出使能
		
		PWM_AL_OFF;						//下管关闭
		PWM_BL_OFF;
		PWM_CL_OFF;
		
		return;							//退出此次函数
	}
	
	PWM_AL_OFF;				//下管关闭
	PWM_BL_OFF;
	PWM_CL_OFF;
	
	//输出上桥
	if(step==0 || step==1)	{	 ATIM_SetCompare1A(Out_Pwm_Value); ATIM_SetCompare2A(0); ATIM_SetCompare3A(0);	} //0:AB; 1:AC; A上桥输出PWM
	if(step==2 || step==3)	{	 ATIM_SetCompare1A(0); ATIM_SetCompare2A(Out_Pwm_Value); ATIM_SetCompare3A(0);	} //2:BC; 3:BA; B上桥输出PWM
	if(step==4 || step==5)	{	 ATIM_SetCompare1A(0); ATIM_SetCompare2A(0); ATIM_SetCompare3A(Out_Pwm_Value);	} //4:CA; 5:CB; C上桥输出PWM

	//输出下桥
	if(step==0 || step==5)		{	PWM_AL_OFF;	PWM_CL_OFF;	PWM_BL_ON;	}	//0:AB; 5:CB; B下桥导通
	else if(step==1 || step==2)	{	PWM_AL_OFF;	PWM_BL_OFF;	PWM_CL_ON;	}	//1:AC; 2:BC; C下桥导通 
	else if(step==3 || step==4)	{	PWM_BL_OFF;	PWM_CL_OFF;	PWM_AL_ON;	}	//2:BC;	4:CA; A下桥导通

	ATIM_CtrlPWMOutputs(ENABLE);	 //PWM输出使能
}

//========================================================================
// 函数: uint8_t HALL_Check(void)
// 描述: 获取霍尔状态.
// 参数: None.
// 返回: 霍尔状态码.
// 版本: V1.0, 2022年6月9日
//========================================================================
uint8_t HALL_Check(void)
{
	static uint8_t Hall_Err_Num;	//霍尔状态错误次数
	uint8_t x = 0;					//霍尔状态码
			
	//HA: PA15
	//HB: PB3
	//HC: PA2
	if(PA15_GETVALUE() != 0) x = 1;			//判断IO口,低电平对应位置1
	if(PB03_GETVALUE() != 0) x |= 0x02;		
	if(PA02_GETVALUE() != 0) x |= 0x04;
	
	if( x==0 || x==7 )
	{
		if(++Hall_Err_Num > 10)
		{
			Hall_Err_Num = 10;
			Err_Code = 2;
		}
	}
	else Hall_Err_Num = 0;
	
	return x;
}


//========================================================================
// 函数: void HALL_Motor_Start(void)
// 描述: 霍尔电机状态.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月9日
//========================================================================
void HALL_Motor_Start(void)
{
	uint16_t x;
	
	x = HALL_Check();	//获取霍尔状态码
	
	if( x==0 || x==7 )	//霍尔异常时，输出一项，使电机先转起来
		x = 1;
	
	Cur_step = STEP_TAB[Dir][x-1];						//得到换相序列
	Commutation(Cur_step, Motor_Start_F);
}

//设置速度
void Set_Speed(void)
{
	if(PWM_Duty_Load < OUTMINPWM) 		SetSpeed = 0;
	else if(PWM_Duty_Load <= OUTMAXPWM)
	{
		SetSpeed = MINSPEED + KKN*(PWM_Duty_Load - OUTMINPWM);
	}
	else SetSpeed = MAXSPEED;
}

