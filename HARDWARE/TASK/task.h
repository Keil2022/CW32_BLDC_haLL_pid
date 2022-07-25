#ifndef __TASK_H
#define __TASK_H

#include "app.h"

typedef struct 
{
	uint8_t Run;               //����״̬��Run/Stop
	uint16_t TIMCount;         //��ʱ������
	uint16_t TRITime;          //���ؼ�����
	void (*TaskHook) (void); //������
} TASK_COMPONENTS;

void Task_Marks_Handler_Callback(void);
void Task_Pro_Handler_Callback(void);

#endif


