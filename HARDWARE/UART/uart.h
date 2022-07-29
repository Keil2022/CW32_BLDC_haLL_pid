#ifndef __UART_H
#define __UART_H

#include "config.h"

#define  DEBUG_USART_AFTX               PB08_AFx_UART1TXD()	//��IO�ڸ���
#define  DEBUG_USART_AFRX               PB09_AFx_UART1RXD()
	
//UART_Config
#define DEBUG_USART_CLK					RCC_APB2_PERIPH_UART1	//����ʱ��
#define DEBUG_USARTx 					CW_UART1				//���ں�
#define DEBUG_USART_BaudRate           	115200					//������
#define DEBUG_USART_UclkFreq           	64000000				//ʱ��ԴƵ��
	
//UART_GPIO
#define DEBUG_USART_GPIO_CLK			RCC_AHB_PERIPH_GPIOB	//����GPIOʱ��
#define DEBUG_USART_TX_GPIO_PORT		CW_GPIOB   				
#define DEBUG_USART_TX_GPIO_PIN        	GPIO_PIN_8					
#define DEBUG_USART_RX_GPIO_PORT       	CW_GPIOB
#define DEBUG_USART_RX_GPIO_PIN        	GPIO_PIN_9

void UART_Configuration(void);
	
#endif

