#include "task.h"

static TASK_COMPONENTS Task_Comps[] = 
{
	{0, 10,  10,  TasksHandle_10MS},			/* task 1 Period�� 10ms */
	{0, 100, 100, TasksHandle_100MS},			/* task 1 Period�� 100ms */
	{0, 250, 250, TasksHandle_250MS},			/* task 1 Period�� 250ms */
	{0, 500, 500, TasksHandle_500MS}			/* task 1 Period�� 500ms */
};

uint8_t Tasks_Max = sizeof(Task_Comps)/sizeof(Task_Comps[0]);

//========================================================================
// ����: Task_Handler_Callback
// ����: �����ǻص�����.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��1��
//========================================================================
void Task_Marks_Handler_Callback(void)
{
	uint8_t i;
	for(i=0; i<Tasks_Max; i++)	//����ѭ��ִ��
	{
		if(Task_Comps[i].TIMCount)    //�����Ƿ�Ϊ0
		{
			Task_Comps[i].TIMCount--;  //������1
			if(Task_Comps[i].TIMCount == 0)  //����ֵΪ0
			{
				/*Resume the timer value and try again */
				Task_Comps[i].TIMCount = Task_Comps[i].TRITime;  //��װ�ؼ���ֵ
				Task_Comps[i].Run = 1;   						//ִ��״̬��־��1
			}
		}
	}
}

//========================================================================
// ����: Task_Pro_Handler_Callback
// ����: ������ص�����.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��1��
//========================================================================
void Task_Pro_Handler_Callback(void)
{
	uint8_t i;
	for(i=0; i<Tasks_Max; i++)
	{
		if(Task_Comps[i].Run)	//�ж�ִ��״̬ 1ִ�� 0��ִ��
		{
			Task_Comps[i].Run = 0;    	//ִ��״̬��0
			Task_Comps[i].TaskHook();  //ִ�к���
		}
	}
}


