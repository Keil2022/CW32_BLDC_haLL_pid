#include "global.h"

unsigned int DIin = 0; 		//�������ƫ��
unsigned int CanshuV = 0;	//ĸ�ߵ�ѹ100mVΪ��λ
unsigned int CanshuI = 0;	//ĸ�ߵ���mA

const uint8_t STEP_TAB[6]={1,3,2,5,0,4};	//{1,3,2,6,4,5}

uint16_t Err_Code = 0;		//�����ת�������
uint16_t Cur_step = 0; 		//��ǰHALL״̬
uint16_t Motor_Start_F = 0; //����������б�־
uint16_t OutPwm = 0;  		//���PWM

unsigned int  ISH = 8;	//��������ֵ A
unsigned int  VSH = 40;	//��ѹ����ֵ V
unsigned int  LSH = 9;	//Ƿѹ����ֵ V


