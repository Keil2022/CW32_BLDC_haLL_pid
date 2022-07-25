#include "task.h"

static TASK_COMPONENTS Task_Comps[] = 
{
	{0, 10,  10,  TasksHandle_10MS},			/* task 1 Period： 10ms */
	{0, 100, 100, TasksHandle_100MS},			/* task 1 Period： 100ms */
	{0, 250, 250, TasksHandle_250MS},			/* task 1 Period： 250ms */
	{0, 500, 500, TasksHandle_500MS}			/* task 1 Period： 500ms */
};

uint8_t Tasks_Max = sizeof(Task_Comps)/sizeof(Task_Comps[0]);

//========================================================================
// 函数: Task_Handler_Callback
// 描述: 任务标记回调函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月1日
//========================================================================
void Task_Marks_Handler_Callback(void)
{
	uint8_t i;
	for(i=0; i<Tasks_Max; i++)	//任务循环执行
	{
		if(Task_Comps[i].TIMCount)    //计数是否为0
		{
			Task_Comps[i].TIMCount--;  //计数减1
			if(Task_Comps[i].TIMCount == 0)  //计数值为0
			{
				/*Resume the timer value and try again */
				Task_Comps[i].TIMCount = Task_Comps[i].TRITime;  //重装载计数值
				Task_Comps[i].Run = 1;   						//执行状态标志置1
			}
		}
	}
}

//========================================================================
// 函数: Task_Pro_Handler_Callback
// 描述: 任务处理回调函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月1日
//========================================================================
void Task_Pro_Handler_Callback(void)
{
	uint8_t i;
	for(i=0; i<Tasks_Max; i++)
	{
		if(Task_Comps[i].Run)	//判断执行状态 1执行 0不执行
		{
			Task_Comps[i].Run = 0;    	//执行状态清0
			Task_Comps[i].TaskHook();  //执行函数
		}
	}
}


