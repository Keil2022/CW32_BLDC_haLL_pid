#ifndef __UART_H
#define __UART_H

#include "config.h"

#define USART_DMA 1

#define  DEBUG_USART_AFTX               PB08_AFx_UART1TXD()	//打开IO口复用
#define  DEBUG_USART_AFRX               PB09_AFx_UART1RXD()
	
//UART_Config
#define DEBUG_USART_CLK					RCC_APB2_PERIPH_UART1	//串口时钟
#define DEBUG_USARTx 					CW_UART1				//串口号
#define DEBUG_USART_BaudRate           	115200					//波特率
#define DEBUG_USART_UclkFreq           	64000000				//时钟源频率
	
//UART_GPIO
#define DEBUG_USART_GPIO_CLK			RCC_AHB_PERIPH_GPIOB	//串口GPIO时钟
#define DEBUG_USART_TX_GPIO_PORT		CW_GPIOB   				
#define DEBUG_USART_TX_GPIO_PIN        	GPIO_PIN_8					
#define DEBUG_USART_RX_GPIO_PORT       	CW_GPIOB
#define DEBUG_USART_RX_GPIO_PIN        	GPIO_PIN_9

//DMA
#define DEBUG_USART_DMA_CLK 			RCC_AHB_PERIPH_DMA				//DMA时钟
#define DEBUG_USART_TX_DMA             	CW_DMACHANNEL2					//DMA通道2	通道号越小优先级越高，通道号越大优先级越低。
#define DEBUG_USART_RX_DMA             	CW_DMACHANNEL1					//DMA通道1	
#define DEBUG_USART_TxTrigSource       	DMA_HardTrig_UART1_TXBufferE	
#define DEBUG_USART_RxTrigSource       	DMA_HardTrig_UART1_RXBufferNE
#define DEBUG_DMA_BUFFSIZE             	100

extern uint8_t TxDBuffer; 
extern uint8_t RxDBuffer; 

void UART_Configuration(void);
void UART1_IRQHandleCallback(void);
void USART_DMA_Configuration(void);
void USART_TX_Succeed(void);
void USART_Start_TX(void);
	
#endif

