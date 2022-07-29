#include "app.h"
#include "bmp.h"

uint8_t i = 0;

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
	PWM_UL_ON;
	PWM_VL_ON;
	PWM_WL_ON;
	delay1ms(50);
	PWM_UL_OFF;
	PWM_VL_OFF;
	PWM_WL_OFF;
	
	GTIM2_Configuration();	//GTIM 输入捕获初始化
	
	Encoder_Init();			//编码旋钮初始化
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

void TasksHandle_100MS(void)
{	
	static uint8_t Buzzer_Time;
	
	Calculate_PWM_duty();
	
	if(OutPwm < PWM_Duty_Load)
	{
		OutPwm += OUTMINPWM; //缓启动
		if(OutPwm >= PWM_Duty_Load) OutPwm = PWM_Duty_Load;
	}
	else OutPwm = PWM_Duty_Load;  //实现从电位器上读取AD代码值换算到输出占空比。


	if(Motor_Start_F == 1)//电机启动后，及时更新PWM
		Commutation(STEP_TAB[HALL_Check()-1], OutPwm, Motor_Start_F);//此调用主要及时更新输出占空比	
	
	//控制电机启停
	if(PWM_Duty_Load < OUTMINPWM) 	//小于最小输出时，电机停止运行
	{	  
		 if(Motor_Start_F == 1) 	//电机当前在运转
		 {
			Motor_Start_F = 0;		//电机停止状态
			HALL_Motor_Start(); 	//停止电机
			PA12_SETHIGH();			//LED OFF
			PB02_SETHIGH();
			Buzzer_Time = 0;
		 }					 
	}
	else if( (PWM_Duty_Load > OUTMINPWM + 50) && (Motor_Start_F==0) ) //大于最小输出时，开始转
	{			 			
		Motor_Start_F = 1;		//改变启停状态
		OutPwm = OUTMINPWM;
		HALL_Motor_Start();		//启动电机
		PA12_SETLOW();			//LED ON
		PB02_SETHIGH();
		Buzzer_Time = 0;
	}

	if(Buzzer_Time < 255)	Buzzer_Time++;
	if(Buzzer_Time > 3) 	PB02_SETLOW();
}

void TasksHandle_250MS(void)
{
	PA11_TOG();
	IWDT_Refresh();
	
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
	
	
	
	OLED_Refresh();
}





