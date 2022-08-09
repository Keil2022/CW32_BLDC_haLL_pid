#include "app.h"
#include "bmp.h"
#include "global.h"

/*
��ҳ�˵���1���趨�ٶȡ�ʵʱ�ٶȣ� 2��ĸ�ߵ�ѹ��������ʾ��
 KEY1�������л��ٶȼ���ѹ�����˵�
 KEY3�������ͣ���ƣ�LED3����ʱ�������������״̬�� LED3����ʱ���������ֹͣ״̬�� 
 ��λ�����٣��л����ٶȲ˵���������ʾ��ǰ�趨ת�١�
 
 ���ܣ�
   1. ��ͣ
	    ʹ�ð���STR/STP������ͣ���
	 2. ���ࣺ
	    ���ݻ���GTIM��ʱ�����ࡣ
	 3. ����
	    ��λ�����٣���λ���ɼ�����AD����ֵ���ٶ�֮����һ�����Թ�ϵ��
	 4. ����
       �������ϡ�������������ѹ��������ת���������� Ƿѹ�����������Լ��ӣ�
   5. ��ʾ
       ��PB4�����л��˵�����ʾ��ѹ��������ʵʱ�ٶ��趨�ٶȡ�
  
   ��״̬�����Ƶ����ת���̡�	
 
 
 Designed by cassy from UET company
 Tel:18129915976
 2022/5/18
*/

//========================================================================
// ����: void APP_config(void)
// ����: �������ú���.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��1��
//========================================================================
void APP_config(void)
{
	BTIM1_Configuration();	//��ʱ������
	IWDT_Configuration();	//�������Ź���ʼ��
	
	LED_Init();				//LED��ʼ��
	KEY_Init();
	Buzzer_Init();
	
	OLED_I2C_Init();		//OLED��ʼ��
	OLED_Init();
	OLED_Clear();
	OLED_Play_1();
	
	ADC_Configuration();		//ADC��ʼ��
	
	UART_Configuration();		//���ڳ�ʼ��
	
	ATIM_Configuration();	//ATIM PWM��ʼ��
	PWM_AL_ON;
	PWM_BL_ON;
	PWM_CL_ON;
	delay1ms(50);
	PWM_AL_OFF;
	PWM_BL_OFF;
	PWM_CL_OFF;
	
	GTIM2_Configuration();	//GTIM ���벶���ʼ��
	
	Encoder_Init();			//������ť��ʼ��
	
	KKN = (MAXSPEED - MINSPEED);
	KKN = KKN/(OUTMAXPWM - OUTMINPWM);
	
	DIin=0x500;		//ƫ�õ����趨 0.9V-1.1V�м䣬��Ϊ��ȷ
	if(SampleData[2]>1117 && SampleData[2]<=1500)	DIin = SampleData[2];	
	SampleVI();		//�ɼ�ĸ�ߵ�ѹ���������ж�
}

//========================================================================
// ����: void TasksHandle_xMS(void)
// ����: ��������.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��1��
//========================================================================
void TasksHandle_10MS(void)
{
	KEY_Handle();
}

void TasksHandle_20MS(void)
{
	Set_Speed();
	HALL_Check();	//�������Ƿ�����
}

void TasksHandle_200MS(void)
{	
	SampleVI();		//�ɼ�ĸ�ߵ�ѹ���������ж�
}

void TasksHandle_250MS(void)
{
	PA11_TOG();
	IWDT_Refresh();

//	����_PWM	
//	ATIM_SetCompare1A(0);
//	ATIM_SetCompare2A(1600);
//	ATIM_SetCompare3A(0);
//	
//	PWM_BL_OFF;PWM_AL_ON;PWM_CL_ON;
}

void TasksHandle_500MS(void)
{
//	uint8_t temp;
	//OLED_ShowPicture(0,0,128,8,BMP1);
	
//	ADC_SoftwareStartConvCmd(ENABLE);	//ADCת�������������
//	temp = ADC_GetValue();
//	OLED_ShowNum(0,0,temp,1,16);
//	OLED_ShowChar(16,0,'.',16);
//	temp = ((uint8_t)(ADC_GetValue()*10))%10;
//	OLED_ShowNum(32,0,temp,1,16);
//	OLED_Refresh();	//������ʾ
	
	//TxDBuffer = 10;
	//OLED_ShowNum(0,48,TxDBuffer,4,16);
	//printf("\r\nCW32F030 UART Printf Example\r\n");
	//printf("%d\r\n",TxDBuffer);
	
//	uint8_t Duty_Load, Duty_Out;
//	
//	Duty_Load = PWM_Duty_Load * 0.03125;
//	Duty_Out = OutPwm * 0.03125;
	
	//��ʾPWMռ�ձ�
	OLED_ShowNum(48,0,PWM_Duty_Load/32,3,16);
//	OLED_ShowNum(72,16,OutPwm,4,16);

	//ADC_Result = ADC_GetConversionValue();

	Speed_Count();
	OLED_ShowNum(48,16,RealS,4,16);
//	OLED_ShowNum(0,48,HALL_Check(),1,16);
//	OLED_ShowNum(0,48,GTIM_Reed_CNT_Value(CW_GTIM1),5,16);
//	OLED_ShowChar(0,48,RxDBuffer[0],16);
	
	OLED_ShowNum(48,32,SampleData[0],4,16);
	OLED_ShowNum(88,32,SampleData[1],4,16);
	OLED_ShowNum(48,48,SampleData[2],4,16);
	OLED_ShowNum(88,48,SampleData[3],4,16);
	
	OLED_Refresh();
}

void State_Machine(void)
{
	
}



