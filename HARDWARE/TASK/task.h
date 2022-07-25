#ifndef __TASK_H
#define __TASK_H

#include "app.h"

typedef struct 
{
	uint8_t Run;               //任务状态：Run/Stop
	uint16_t TIMCount;         //定时计数器
	uint16_t TRITime;          //重载计数器
	void (*TaskHook) (void); //任务函数
} TASK_COMPONENTS;

void Task_Marks_Handler_Callback(void);
void Task_Pro_Handler_Callback(void);

#endif


