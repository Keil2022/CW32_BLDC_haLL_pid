#include "config.h"
#include "sys_init.h"
#include "task.h"

//========================================================================
// ����: int main(void)
// ����: ������.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��1��
//========================================================================
int main(void)
{
	SYS_Init();
	
//	RCC_PCLK_OUT();  //ͨ��PA03�۲�PCLKƵ��
//	RCC_HCLK_OUT();  //ͨ��PA04�۲�HCLKƵ��
	
	while (1)
	{					
		Task_Pro_Handler_Callback();
	}
}
