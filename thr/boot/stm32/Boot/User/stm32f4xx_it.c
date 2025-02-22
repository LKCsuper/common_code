/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-10-12 10:07:03
 * @LastEditors: lkc
 * @LastEditTime: 2023-10-26 13:15:03
 * @detail: 
 */
/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-10-12 10:07:03
 * @LastEditors: lkc
 * @LastEditTime: 2023-10-13 15:38:00
 * @detail: 
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 串口3中断
 * @detail: 
 * @return {*}
 * @author: lkc
 */
void USART3_IRQHandler(void)
{
    uint32_t recvLen = 0;
    char data;

    if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
    {
        USART_ReceiveData(USART3);
        USART_ClearFlag(USART3, USART_FLAG_ORE);
    }
    if(USART_GetITStatus(USART3,USART_IT_TC)!= RESET)
    {
        USART_ClearITPendingBit(USART3, USART_IT_TC);
    }
    
    if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
    {
        if (fifo_read(&ringbufTxFifo, (uint8_t *)&data, 1))  
        {
            USART_SendData(USART3, data);
        }
        else
        {
            USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
        }
    }
    
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
    
    if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
    {  
        USART_ReceiveData(USART3);
        DMA_Cmd(DMA1_Stream1,DISABLE);
        uart_Msg.param.last_recvLen = uart_Msg.param.recvLen;
        uart_Msg.param.recvLen= UART_RX_BUFSIZE - DMA_GetCurrDataCounter(DMA1_Stream1);
        uart_Msg.param.recvFlag = 1;	
        uart_Msg.param.recvTotalLen += uart_Msg.param.recvLen;
        uart_Msg.param.recvPkgCnt++;      

        recvLen = UART_RX_BUFSIZE - DMA_GetCurrDataCounter(DMA1_Stream1);
        fifo_write(&ringbufRxFifo, uart_Msg.recvData, recvLen);
        DMA_SetCurrDataCounter(DMA1_Stream1, UART_RX_BUFSIZE);
        DMA_Cmd(DMA1_Stream1,ENABLE);
    }
}

/******************************************************************************/
void NMI_Handler(void)
{}

#if 0
void HardFault_Handler(void)
{
    while (1)
    {
        YMODEM_ERR("Hard Fault\r\n");
    }
}
#endif

void MemManage_Handler(void)
{
  while (1)
  {}
}

void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}


void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
 * @description: 电源检测
 * @detail: 
 * @return {*}
 * @author: lkc
 */
void PVD_IRQHandler(void)
{
    if(PWR_GetFlagStatus(PWR_FLAG_PVDO))
    {
        YMODEM_PRINTF("POWER DOWN\r\n");
    }
    EXTI_ClearITPendingBit(EXTI_Line16); 
}
#ifdef __cplusplus
}
#endif