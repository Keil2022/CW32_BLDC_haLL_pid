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

extern unsigned int DIin; 		//�������ƫ��
extern unsigned int CanshuV;	//ĸ�ߵ�ѹ100mVΪ��λ
extern unsigned int CanshuI;	//ĸ�ߵ���mA

extern const uint8_t STEP_TAB[6];

extern uint16_t Err_Code;		//�����ת�������
extern uint16_t Cur_step; 		//��ǰHALL״̬
extern uint16_t Motor_Start_F; 	//����������б�־
extern uint16_t OutPwm;  		//���PWM

extern unsigned int  ISH;	//��������ֵ A
extern unsigned int  VSH;	//��ѹ����ֵ V
extern unsigned int  LSH;	//Ƿѹ����ֵ V

//#endif

