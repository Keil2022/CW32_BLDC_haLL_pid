#include "config.h"
#include "sys_init.h"
#include "task.h"

//========================================================================
// 函数: int main(void)
// 描述: 主函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月1日
//========================================================================
int main(void)
{
	SYS_Init();
	
//	RCC_PCLK_OUT();  //通过PA03观察PCLK频率
//	RCC_HCLK_OUT();  //通过PA04观察HCLK频率
	
	while (1)
	{					
		Task_Pro_Handler_Callback();
	}
}
