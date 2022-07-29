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
// ����: void UART_Configuration(void)
// ����: ���ڳ�ʼ������.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��7��
//========================================================================
void UART_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//����GPIO����
	USART_InitTypeDef USART_InitStructure;	//��������
	
	RCC_AHBPeriphClk_Enable(DEBUG_USART_GPIO_CLK, ENABLE);	//ʹ����Ӧʱ��
	RCC_APBPeriphClk_Enable2(DEBUG_USART_CLK, ENABLE);
	
	DEBUG_USART_AFTX;
	DEBUG_USART_AFRX;
	
	//GPIO
	GPIO_InitStructure.Pins = DEBUG_USART_TX_GPIO_PIN;			//���Ͷ˿�
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;				//�������
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;					//�ٶȣ���
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);	//����IO��
	
	GPIO_InitStructure.Pins = DEBUG_USART_RX_GPIO_PIN;			//���ն˿�
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;			//��������
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);	//����IO��
	
	DEBUG_USARTx->CR1_f.SYNC = 0;			//0-�첽ȫ˫��ͨ��ģʽ; 1-ͬ����˫��ͨ��ģʽ;
	DEBUG_USARTx->ICR = 0x00;				//������д����жϱ�־
	
	//UART
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BaudRate;							//������
	USART_InitStructure.USART_Over = USART_Over_16;										//������ʽ��16������
	USART_InitStructure.USART_Source = USART_Source_PCLK;								//����ʱ��ԴPCLK
	USART_InitStructure.USART_UclkFreq = DEBUG_USART_UclkFreq;							//����ʱ��ԴƵ��
	USART_InitStructure.USART_StartBit = USART_StartBit_FE;								//��ʼλ�ж���ʽ��RXD�½���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;								//ֹͣλ���ȣ�1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ;								//У�鷽ʽ����У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//Ӳ�����أ���
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//����/����ʹ��
	USART_Init(DEBUG_USARTx, &USART_InitStructure); 									//���ô���

	NVIC_SetPriority(UART1_IRQn, 3);					//���ȼ���3
	NVIC_EnableIRQ(UART1_IRQn);							//ʹ�ܴ����ж�
	USART_ITConfig(DEBUG_USARTx, USART_IT_RC, ENABLE);	//���ڽ����ж�ʹ��
}

//========================================================================
// ����: void UART1_IRQHandleCallback(void)
// ����: �����жϴ�����.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��6��7��
//========================================================================
void UART1_IRQHandleCallback(void)
{
	uint8_t RxBuffer;
	if(USART_GetITStatus(DEBUG_USARTx, USART_IT_RC) != RESET)
	{
		USART_ClearITPendingBit(DEBUG_USARTx, USART_IT_RC);  	//����жϱ�־λ
		RxBuffer = USART_ReceiveData_8bit(DEBUG_USARTx);		//��ȡ����
		
		USART_SendData_8bit(DEBUG_USARTx, RxBuffer); 			//��������
	}
}

