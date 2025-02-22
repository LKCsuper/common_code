#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct_UartParam uart_Msg;
BOOL bEnterFlag = RESET;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
 /**
 * @brief		ymodem串口初始化
 * @details	    This is the detail description.
 * @param[in]	ulBound 波特率
 * @param[out]	None
 * @retval		None
 * @par  time 2022/8/5
 * @par  name Ymodem_UartInit
 */
VOID Bsp_USART_Init(ULONG ulBound)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);

	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate =  ulBound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_IDLE , ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
	USART_ClearITPendingBit(USART3, USART_IT_TC);

	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	
	DMA_DeInit(DMA1_Stream1);
	DMA_StructInit( &DMA_InitStructure);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART3->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)uart_Msg.recvData;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = UART_RX_BUFSIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
	DMA_Cmd(DMA1_Stream1, ENABLE);
	
	USART_Cmd(USART3, ENABLE);
    GPIO_SetBits(GPIOC,GPIO_Pin_13);

	/* 有些串口,可能上电会有杂乱码,所以先进行清空 */
	while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
	{
		USART_ReceiveData(EVAL_COM1);
	}

	return;
}

/**
  * @brief  杈撳嚭鍒扮粓绔?涓€涓?瀛楃??
  * @param  c: 杈撳嚭瀛楋拷??
  * @retval None
  */
VOID Bsp_PutChar(UCHAR c)
{
	USART_SendData(EVAL_COM1, c);
	while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE) == RESET){ }
	// fifo_write(&ringbufTxFifo, &c, 1);
	// USART_ITConfig(EVAL_COM1, USART_IT_TXE, ENABLE);
	return;
}

/**
 * @description: 获取一个字符
 * @detail: 
 * @param {UCHAR*} key
 * @return {*}
 * @author: lkc
 */
ULONG Bsp_GetChar(OUT UCHAR* key)
{
	if (0 != fifo_read(&ringbufRxFifo, key, 1))
	{
		return YMODEM_SUCCESS;
	}
	else
	{
		return YMODEM_ERROR;
	}
}

/**
 * @description: debug读取数据
 * @detail: 
 * @return {*}
 * @author: lkc
 */
UCHAR Bsp_Usart_DebugRead(void)
{
	UCHAR ucData;

	if (0 != fifo_read(&ringbufRxFifo, &ucData, 1))
	{
		return ucData;
	}
	else
	{
		return 0;
	}
}

#ifndef WORK_MDK
/**
 * @description: 重定向(IAR)
 * @detail: 
 * @param {int} handle
 * @param {unsigned char *} buffer
 * @param {size_t} size
 * @return {*}
 * @author: lkc
 */
size_t __write(int handle, const unsigned char * buffer, size_t size)
{  
	for (int i =0; i < size; i++)
	{
	    USART_SendData(EVAL_COM1, (u8)buffer[i]);
        while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET);
	}
	
	return size;
}
#else
/**
 * @description: 重定向
 * @detail description: 
 * @param {int} ch
 * @param {FILE} *p
 * @return {*}
 */
int fputc(int ch, FILE *p)
{
	USART_SendData(EVAL_COM1, (u8)ch);
	while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET);
	return ch;
}
#endif
#ifdef __cplusplus
};
#endif
