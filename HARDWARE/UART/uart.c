#include "uart.h"

uint8_t TxDBuffer = 10; 
uint8_t RxDBuffer; 

#ifdef __GNUC__
	/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
	set to 'Yes') calls __io_putchar() */
	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
		
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
	USART_SendData_8bit(DEBUG_USARTx, (uint8_t)ch);

	while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);

	return ch;
}

//========================================================================
// 函数: void UART_Configuration(void)
// 描述: 串口初始化配置.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月7日
//========================================================================
void UART_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//串口GPIO配置
	USART_InitTypeDef USART_InitStructure;	//串口配置
	
	RCC_AHBPeriphClk_Enable(DEBUG_USART_GPIO_CLK | DEBUG_USART_DMA_CLK, ENABLE);	//使能相应时钟
	RCC_APBPeriphClk_Enable2(DEBUG_USART_CLK, ENABLE);
	
	DEBUG_USART_AFTX;
	DEBUG_USART_AFRX;
	
	//GPIO
	GPIO_InitStructure.Pins = DEBUG_USART_TX_GPIO_PIN;			//发送端口
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;				//推挽输出
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;					//速度：高
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);	//配置IO口
	
	GPIO_InitStructure.Pins = DEBUG_USART_RX_GPIO_PIN;			//接收端口
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;			//上拉输入
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);	//配置IO口
	
	DEBUG_USARTx->CR1_f.SYNC = 0;			//0-异步全双工通信模式; 1-同步半双工通信模式;
	DEBUG_USARTx->ICR = 0x00;				//清除所有串口中断标志
	
	//UART
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BaudRate;							//波特率
	USART_InitStructure.USART_Over = USART_Over_16;										//采样方式：16倍采样
	USART_InitStructure.USART_Source = USART_Source_PCLK;								//传输时钟源PCLK
	USART_InitStructure.USART_UclkFreq = DEBUG_USART_UclkFreq;							//传输时钟源频率
	USART_InitStructure.USART_StartBit = USART_StartBit_FE;								//起始位判定方式：RXD下降沿
	USART_InitStructure.USART_StopBits = USART_StopBits_1;								//停止位长度：1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No ;								//校验方式：无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//硬件流控：无
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//发送/接收使能
	USART_Init(DEBUG_USARTx, &USART_InitStructure); 									//配置串口

#if USART_DMA	
	DMA_ClearITPendingBit(DMA_IT_TC1 | DMA_IT_TE1);										//清除DMA通道1所有中断标志
	DMA_ClearITPendingBit(DMA_IT_TC2 | DMA_IT_TE2);										//清除DMA通道2所有中断标志
	USART_DMA_Configuration();															//串口DMA配置
	USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);				//使能串口DMA传输
#else
	NVIC_SetPriority(UART1_IRQn, 3);					//优先级：3
	NVIC_EnableIRQ(UART1_IRQn);							//使能串口中断
	USART_ITConfig(DEBUG_USARTx, USART_IT_RC, ENABLE);	//串口接收中断使能
#endif

}

//========================================================================
// 函数: void UART1_IRQHandleCallback(void)
// 描述: 串口中断处理函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月7日
//========================================================================
void UART1_IRQHandleCallback(void)
{
	uint8_t RxBuffer;
	if(USART_GetITStatus(DEBUG_USARTx, USART_IT_RC) != RESET)
	{
		USART_ClearITPendingBit(DEBUG_USARTx, USART_IT_RC);  	//清除中断标志位
		RxBuffer = USART_ReceiveData_8bit(DEBUG_USARTx);		//获取数据
		
		USART_SendData_8bit(DEBUG_USARTx, RxBuffer); 			//发送数据
	}
}

//========================================================================
// 函数: void USART_DMA_Configuration(void)
// 描述: 串口DMA配置函数.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年6月14日
//========================================================================
void USART_DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	//配置 DMA TX
	DMA_InitStructure.DMA_Mode = DMA_MODE_BLOCK;						//DMA模式：BLOCK	每传输完成 1 个数据块后会插入一个传输间隙
	DMA_InitStructure.DMA_TransferWidth = DMA_TRANSFER_WIDTH_8BIT;		//数据位宽:8 Bit
	DMA_InitStructure.DMA_SrcInc = DMA_SrcAddress_Increase;				//源地址增量方式：增加
	DMA_InitStructure.DMA_DstInc = DMA_DstAddress_Fix;					//目标地址增量方式：固定
	DMA_InitStructure.TrigMode = DMA_HardTrig;							//触发方式：	硬件
	DMA_InitStructure.HardTrigSource = DEBUG_USART_TxTrigSource;		//DMA触发源：	USARTx->TDR为空时触发DMA传输
	DMA_InitStructure.DMA_TransferCnt = DEBUG_DMA_BUFFSIZE;				//通道传输数据块数量：	DEBUG_DMA_BUFFSIZE
	DMA_InitStructure.DMA_SrcAddress = (uint32_t)&TxDBuffer;			//传输源地址：			TxRxBuffer
	DMA_InitStructure.DMA_DstAddress = (uint32_t)&DEBUG_USARTx->TDR;	//传输目的地址：		DEBUG_USARTx->TDR
	DMA_Init(DEBUG_USART_TX_DMA, &DMA_InitStructure);
	DMA_Cmd(DEBUG_USART_TX_DMA, ENABLE);								//TX-DMA通道2使能
	
	//配置 DMA RX
	DMA_InitStructure.DMA_Mode = DMA_MODE_BLOCK;
	DMA_InitStructure.DMA_TransferWidth = DMA_TRANSFER_WIDTH_8BIT;
	DMA_InitStructure.DMA_SrcInc = DMA_SrcAddress_Fix;
	DMA_InitStructure.DMA_DstInc = DMA_DstAddress_Increase;
	DMA_InitStructure.TrigMode = DMA_HardTrig;
	DMA_InitStructure.HardTrigSource = DEBUG_USART_RxTrigSource;		//DMA触发源：		串口1接收非空标志
	DMA_InitStructure.DMA_TransferCnt = DEBUG_DMA_BUFFSIZE;
	DMA_InitStructure.DMA_SrcAddress = (uint32_t)&DEBUG_USARTx->RDR;
	DMA_InitStructure.DMA_DstAddress = (uint32_t)&RxDBuffer;
	DMA_Init(DEBUG_USART_RX_DMA, &DMA_InitStructure);
	DMA_Cmd(DEBUG_USART_RX_DMA, ENABLE);								//RX-DMA通道1使能
}

void USART_TX_Succeed(void)
{
	if( CW_DMA->ISR_f.TC2 )										//确认DMA传输完成
	{
		//CW_DMA->ICR_f.TC1 = 0;
		USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, DISABLE);	//关闭串口DMA发送
		if( DEBUG_USARTx->ISR_f.TXBUSY == 0 )					//发送完毕
		{
			USART_Start_TX();
		}
	}
}

void USART_Start_TX(void)
{
	DEBUG_USARTx->ICR = 0x00;							//清除所有串口中断标志
	DMA_ClearITPendingBit(DMA_IT_TC2 | DMA_IT_TE2);		//清除DMA通道1所有中断标志
	
	DEBUG_USART_TX_DMA->CNT=bv16|100;	 				//启动DMA传输
	DEBUG_USART_TX_DMA->CSR_f.EN = 1; 
}

