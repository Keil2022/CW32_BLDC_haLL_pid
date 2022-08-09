#include "app.h"
#include "bmp.h"
#include "global.h"

/*
两页菜单：1：设定速度、实时速度； 2：母线电压、电流显示。
 KEY1：按键切换速度及电压电流菜单
 KEY3：电机启停控制，LED3灯亮时，电机处于启动状态； LED3灯来时，电机处于停止状态； 
 电位器调速，切换到速度菜单，可以显示当前设定转速。
 
 功能：
   1. 启停
	    使用按键STR/STP按键启停电机
	 2. 换相：
	    根据霍尔GTIM定时器换相。
	 3. 调速
	    电位器调速，电位器采集到的AD代码值与速度之间是一个线性关系。
	 4. 故障
       霍尔故障、过流保护、过压保护、堵转保护。。。 欠压保护（可以自己加）
   5. 显示
       用PB4按键切换菜单，显示电压电流或者实时速度设定速度。
  
   用状态机控制电机运转过程。	
 
 
 Designed by cassy from UET company
 Tel:18129915976
 2022/5/18
*/

//========================================================================
// 函数: void APP_config(void)
// 描述: 外设配置函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月1日
//========================================================================
void APP_config(void)
{
	BTIM1_Configuration();	//定时器配置
	IWDT_Configuration();	//独立看门狗初始化
	
	LED_Init();				//LED初始化
	KEY_Init();
	Buzzer_Init();
	
	OLED_I2C_Init();		//OLED初始化
	OLED_Init();
	OLED_Clear();
	OLED_Play_1();
	
	ADC_Configuration();		//ADC初始化
	
	UART_Configuration();		//串口初始化
	
	ATIM_Configuration();	//ATIM PWM初始化
	PWM_AL_ON;
	PWM_BL_ON;
	PWM_CL_ON;
	delay1ms(50);
	PWM_AL_OFF;
	PWM_BL_OFF;
	PWM_CL_OFF;
	
	GTIM2_Configuration();	//GTIM 输入捕获初始化
	
	Encoder_Init();			//编码旋钮初始化
	
	KKN = (MAXSPEED - MINSPEED);
	KKN = KKN/(OUTMAXPWM - OUTMINPWM);
	
	DIin=0x500;		//偏置电流设定 0.9V-1.1V中间，认为正确
	if(SampleData[2]>1117 && SampleData[2]<=1500)	DIin = SampleData[2];	
	SampleVI();		//采集母线电压电流，并判断
}

//========================================================================
// 函数: void TasksHandle_xMS(void)
// 描述: 任务处理函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月1日
//========================================================================
void TasksHandle_10MS(void)
{
	KEY_Handle();
}

void TasksHandle_20MS(void)
{
	Set_Speed();
	HALL_Check();	//检查霍尔是否正常
}

void TasksHandle_200MS(void)
{	
	SampleVI();		//采集母线电压电流，并判断
}

void TasksHandle_250MS(void)
{
	PA11_TOG();
	IWDT_Refresh();

//	测试_PWM	
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
	
//	ADC_SoftwareStartConvCmd(ENABLE);	//ADC转换软件启动命令
//	temp = ADC_GetValue();
//	OLED_ShowNum(0,0,temp,1,16);
//	OLED_ShowChar(16,0,'.',16);
//	temp = ((uint8_t)(ADC_GetValue()*10))%10;
//	OLED_ShowNum(32,0,temp,1,16);
//	OLED_Refresh();	//更新显示
	
	//TxDBuffer = 10;
	//OLED_ShowNum(0,48,TxDBuffer,4,16);
	//printf("\r\nCW32F030 UART Printf Example\r\n");
	//printf("%d\r\n",TxDBuffer);
	
//	uint8_t Duty_Load, Duty_Out;
//	
//	Duty_Load = PWM_Duty_Load * 0.03125;
//	Duty_Out = OutPwm * 0.03125;
	
	//显示PWM占空比
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



