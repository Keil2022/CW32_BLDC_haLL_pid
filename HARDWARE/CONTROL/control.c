#include "control.h"

void EnDirCheck(void)
{
	if(MOTORSTATE == STATEERROR)	return;
	
	if(KEY_1 == 0)
	{
		if(key1_count <255)	key1_count++;
	}
	else
	{
		if(key1_count == 255)
		{
			startflag = 1-startflag;
			if(startflag == 0)
				LED2_OFF;
			else 
				LED2_ON;
		}
		
		key1_count = 0;
	}
}

void MotorStartCheck(void)
{		
	if(MOTORSTATE == STATEERROR)	return;
		
	if( SetSpeed > 0	&&	startflag == 1 )
	{		
	    MOTORSTATE = STATESTARTPID;		
		Dir = dirflag;			
	}
}

void MotorStartPID(void)
{		
	if(MOTORSTATE == STATEERROR)	return;
 		
	PID_init();
	PIDcompute(MINSPEED,0);
	HALL_Motor_Start();

	TargS1 = SetSpeed;
	MOTORSTATE = STATERUNPID;	
}

void MotorRunPID(void)
{	
	if(MOTORSTATE == STATEERROR)	return;
	if(SetSpeed == 0	||	startflag == 0) //给出停止信号
	{
	 	MOTORSTATE = STATESTOP;	
	}
	
	TargS1 = SetSpeed;	
	if(TimeCountPID >= 20)//计算实时速度并时行PID运算
	{
		TimeCountPID = 0;
		RealS1 = HALLcountTemp*500/MPolePairs;//HALLcount*100*60/6/POLEPAIRS;	
		PIDcompute(TargS1,RealS1);		 
	}
}

void MotorStop(void)
{
	if(MOTORSTATE == STATEERROR)	return;
	Motor_Start_F = 0;	
	MOTOR_STOP0();

	if(RealS == 0) 
	{
		MOTORSTATE = STATESTARTCHECK;	   //停下来后才切换流程 
	}			
}

void MotorError(void)
{
	MOTOR_STOP0();
	Motor_Start_F = 0;
	MOTORSTATE = STATEERROROVER;
}

void MotorErrorOver(void)
{
	unsigned char times=0;	
	
	times=0;
	
	LED1_OFF;
	TimeCountTemp = 0;
	while(TimeCountTemp < 200);
	while(1) //发生故障，则故障指示灯闪烁。需重启
	{	
		if(times < Err_Code)
		{
			if(TimeCountTemp<200)
			{
				LED1_ON;
			}
			else if(TimeCountTemp<=400)//LED flashing
			{
				LED1_OFF;
			}
			else if(TimeCountTemp>400)
			{
				if(times < Err_Code)
				{
					times++;
					TimeCountTemp=0;
					if(times >= Err_Code);
					else LED1_OFF;
				}
			}
		}
		else if(TimeCountTemp<500);
		else
		{
			times=0;
			TimeCountTemp=0;
		}		
	}
}


