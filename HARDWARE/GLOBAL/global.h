//#ifndef __GLOBAL_H
//#define __GLOBAL_H

#include "config.h"

//��ѹֵ�趨
#define RV1 5.1
#define RV2	200
//�����ѹ�����ߵ�ѹ֮�ȣ�ΪRV1:(RV1+RV2)

#define Kvvalue (float)1.2/RV1*(RV1+RV2)

//������ѹ�����϶�����
#define NumErr 15

#define STATESTARTCHECK 0
#define STATESTARTPID 	1
#define STATERUNPID 	2
#define STATESTOP 		3
#define STATEERROR 		5
#define STATEERROROVER 	6

extern unsigned int DIin; 		//�������ƫ��
extern unsigned int CanshuV;	//ĸ�ߵ�ѹ100mVΪ��λ
extern unsigned int CanshuI;	//ĸ�ߵ���mA

extern const unsigned char STEP_TAB[2][6];

extern uint16_t Err_Code;		//�����ת�������
extern uint16_t Cur_step; 		//��ǰHALL״̬
extern uint16_t Motor_Start_F; 	//����������б�־
extern uint16_t OutPwm;  		//���PWM

extern unsigned char Dir; 		//�������

extern unsigned int  ISH;	//��������ֵ A
extern unsigned int  VSH;	//��ѹ����ֵ V
extern unsigned int  LSH;	//Ƿѹ����ֵ V

extern unsigned int  MPolePairs;  	//������
extern unsigned int  MAXSPEED; 		//�ת��

//�ٶȱջ�����  ʵ����С1000��
extern unsigned int  MINSPEED;  		//���ת��
extern unsigned int SP, SI, SD;

extern float KKN;	//�����λ���ٶ�ʱ��б��

extern unsigned int SetSpeed;	//����ٶ�

extern volatile uint8_t Clock_Counter;
extern volatile uint8_t Count_1, Count_2;

extern unsigned int Out_Pwm_Value;//���ռ�ձ�ֵ

extern int16_t PWM_Duty_Load;
extern uint16_t PWM_Duty_Set;
extern int16_t  CNT_Value, CNT_uValue;

extern uint16_t RealS;		//�ٶ�
extern uint16_t HALLcount, HALLcount1;	//��������

//#endif


