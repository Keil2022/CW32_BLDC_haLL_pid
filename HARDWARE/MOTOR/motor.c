#include "motor.h"

//========================================================================
// ����: void Commutation(uint16_t step, uint16_t Out_Pwm_Value, uint16_t Pwm_On_Flag)
// ����: �������.
// ����: steP��Ϊ��ǰ������ţ� OutPwmValue�����PWMֵ�� PWM_ON_flag=1ʱ����PWM���.
// ����: None.
// �汾: V1.0, 2022��6��9��
//========================================================================
void Commutation(uint16_t step, uint16_t Pwm_On_Flag)
{
	if(Pwm_On_Flag == 0)	//�ر����
	{
		ATIM_SetCompare1A(0);			//����ռ�ձȣ�д����ֵΪ ��ռ�ձ�
		ATIM_SetCompare2A(0);
		ATIM_SetCompare3A(0);
		
		ATIM_CtrlPWMOutputs(DISABLE);	//�ر�PWM���ʹ��
		
		PWM_AL_OFF;						//�¹ܹر�
		PWM_BL_OFF;
		PWM_CL_OFF;
		
		return;							//�˳��˴κ���
	}
	
	PWM_AL_OFF;				//�¹ܹر�
	PWM_BL_OFF;
	PWM_CL_OFF;
	
	//�������
	if(step==0 || step==1)	{	 ATIM_SetCompare1A(Out_Pwm_Value); ATIM_SetCompare2A(0); ATIM_SetCompare3A(0);	} //0:AB; 1:AC; A�������PWM
	if(step==2 || step==3)	{	 ATIM_SetCompare1A(0); ATIM_SetCompare2A(Out_Pwm_Value); ATIM_SetCompare3A(0);	} //2:BC; 3:BA; B�������PWM
	if(step==4 || step==5)	{	 ATIM_SetCompare1A(0); ATIM_SetCompare2A(0); ATIM_SetCompare3A(Out_Pwm_Value);	} //4:CA; 5:CB; C�������PWM

	//�������
	if(step==0 || step==5)		{	PWM_AL_OFF;	PWM_CL_OFF;	PWM_BL_ON;	}	//0:AB; 5:CB; B���ŵ�ͨ
	else if(step==1 || step==2)	{	PWM_AL_OFF;	PWM_BL_OFF;	PWM_CL_ON;	}	//1:AC; 2:BC; C���ŵ�ͨ 
	else if(step==3 || step==4)	{	PWM_BL_OFF;	PWM_CL_OFF;	PWM_AL_ON;	}	//2:BC;	4:CA; A���ŵ�ͨ

	ATIM_CtrlPWMOutputs(ENABLE);	 //PWM���ʹ��
}

//========================================================================
// ����: uint8_t HALL_Check(void)
// ����: ��ȡ����״̬.
// ����: None.
// ����: ����״̬��.
// �汾: V1.0, 2022��6��9��
//========================================================================
uint8_t HALL_Check(void)
{
	static uint8_t Hall_Err_Num;	//����״̬�������
	uint8_t x = 0;					//����״̬��
			
	//HA: PA15
	//HB: PB3
	//HC: PA2
	if(PA15_GETVALUE() != 0) x = 1;			//�ж�IO��,�͵�ƽ��Ӧλ��1
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
// ����: void HALL_Motor_Start(void)
// ����: �������״̬.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��9��
//========================================================================
void HALL_Motor_Start(void)
{
	uint16_t x;
	
	x = HALL_Check();	//��ȡ����״̬��
	
	if( x==0 || x==7 )	//�����쳣ʱ�����һ�ʹ�����ת����
		x = 1;
	
	Cur_step = STEP_TAB[Dir][x-1];						//�õ���������
	Commutation(Cur_step, Motor_Start_F);
}

//�����ٶ�
void Set_Speed(void)
{
	if(PWM_Duty_Load < OUTMINPWM) 		SetSpeed = 0;
	else if(PWM_Duty_Load <= OUTMAXPWM)
	{
		SetSpeed = MINSPEED + KKN*(PWM_Duty_Load - OUTMINPWM);
	}
	else SetSpeed = MAXSPEED;
}

