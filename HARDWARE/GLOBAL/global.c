#include "global.h"

unsigned int DIin = 0; 		//�������ƫ��
unsigned int CanshuV = 0;	//ĸ�ߵ�ѹ100mVΪ��λ
unsigned int CanshuI = 0;	//ĸ�ߵ���mA

const unsigned char STEP_TAB[2][6]={{4,0,5,2,3,1},{1,3,2,5,0,4}};

uint16_t Err_Code = 0;		//�����ת�������
uint16_t Cur_step = 0; 		//��ǰHALL״̬
uint16_t Motor_Start_F = 0; //����������б�־
uint16_t OutPwm = 0;  		//���PWM

unsigned char Dir = 0; 		//�������

unsigned int  ISH = 8;	//��������ֵ A
unsigned int  VSH = 40;	//��ѹ����ֵ V
unsigned int  LSH = 9;	//Ƿѹ����ֵ V

unsigned int  MPolePairs = 2;  	//������
unsigned int  MAXSPEED = 5000; 	//�ת��

//�ٶȱջ�����  ʵ����С1000��
unsigned int  MINSPEED = 150;  		//���ת��
unsigned int SP=50, SI=50, SD=50;
float KKN = 0;	//�����λ���ٶ�ʱ��б��

unsigned int SetSpeed = 0;	//����ٶ�

volatile uint8_t Clock_Counter;
volatile uint8_t Count_1 = 255, Count_2 = 255;

unsigned int Out_Pwm_Value = 0;//���ռ�ձ�ֵ

int16_t PWM_Duty_Load = 0;	//PWM����ֵ
uint16_t PWM_Duty_Set;		//PWM�仯ֵ
int16_t  CNT_Value, CNT_uValue;

uint16_t HALLcount = 0, HALLcount1 = 0;		//��������
uint16_t RealS;								//�ٶ�


