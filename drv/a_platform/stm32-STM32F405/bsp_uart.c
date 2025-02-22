/*****************************************************************************
*	模块名称：bsp_uart.c                                         	
*	程序功能：                             
*	编 写 者：                                   
*	编写日期：
                    
* 说明： 
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct rt_ringbuffer uart1Rx;
struct rt_ringbuffer uart1Tx;
static CCRAM rt_uint8_t uart1RxBuf[UART1_RX_BUFSIZE] = {0};
static CCRAM rt_uint8_t uart1TxBuf[UART1_TX_BUFSIZE] = {0};

struct rt_ringbuffer uart2Rx;
struct rt_ringbuffer uart2Tx;
static CCRAM rt_uint8_t uart2RxBuf[UART2_RX_BUFSIZE] = {0};
static CCRAM rt_uint8_t uart2TxBuf[UART2_TX_BUFSIZE] = {0};

struct rt_ringbuffer uart3Rx;
struct rt_ringbuffer uart3Tx;
static CCRAM rt_uint8_t uart3RxBuf[UART3_RX_BUFSIZE] = {0};
static CCRAM rt_uint8_t uart3TxBuf[UART3_TX_BUFSIZE] = {0};

struct rt_ringbuffer uart4Rx;
struct rt_ringbuffer uart4Tx;
static CCRAM rt_uint8_t uart4RxBuf[UART4_RX_BUFSIZE] = {0};
static CCRAM rt_uint8_t uart4TxBuf[UART4_TX_BUFSIZE] = {0};

struct rt_ringbuffer uart5Rx;
struct rt_ringbuffer uart5Tx;
static CCRAM rt_uint8_t uart5RxBuf[UART5_RX_BUFSIZE] = {0};
static CCRAM rt_uint8_t uart5TxBuf[UART5_TX_BUFSIZE] = {0};

struct rt_ringbuffer uart6Rx;
struct rt_ringbuffer uart6Tx;
static CCRAM rt_uint8_t uart6RxBuf[UART6_RX_BUFSIZE] = {0};
static CCRAM rt_uint8_t uart6TxBuf[UART6_RX_BUFSIZE] = {0};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 串口初始化
 * @detail: 
 * @return {*}
 * @author: lkc
 */
int BSP_UART_Init(void)
{
    /* 对于不同的串口创建不同的ringbuf */
    rt_ringbuffer_init(&uart1Rx, (rt_uint8_t *)uart1RxBuf, sizeof(uart1RxBuf));
    rt_ringbuffer_init(&uart1Tx, (rt_uint8_t *)uart1TxBuf, sizeof(uart1TxBuf));

    rt_ringbuffer_init(&uart2Rx, (rt_uint8_t *)uart2RxBuf, sizeof(uart2RxBuf));
    rt_ringbuffer_init(&uart2Tx, (rt_uint8_t *)uart2TxBuf, sizeof(uart2TxBuf));

    rt_ringbuffer_init(&uart3Rx, (rt_uint8_t *)uart3RxBuf, sizeof(uart3RxBuf));
    rt_ringbuffer_init(&uart3Tx, (rt_uint8_t *)uart3TxBuf, sizeof(uart3TxBuf));

    rt_ringbuffer_init(&uart4Rx, (rt_uint8_t *)uart4RxBuf, sizeof(uart4RxBuf));
    rt_ringbuffer_init(&uart4Tx, (rt_uint8_t *)uart4TxBuf, sizeof(uart4TxBuf));

    rt_ringbuffer_init(&uart5Rx, (rt_uint8_t *)uart5RxBuf, sizeof(uart5RxBuf));
    rt_ringbuffer_init(&uart5Tx, (rt_uint8_t *)uart5TxBuf, sizeof(uart5TxBuf));

    rt_ringbuffer_init(&uart6Rx, (rt_uint8_t *)uart6RxBuf, sizeof(uart6RxBuf));
    rt_ringbuffer_init(&uart6Tx, (rt_uint8_t *)uart6TxBuf, sizeof(uart6TxBuf));

    return 0;
}

/**
 * @description: 复位串口
 * @detail: 
 * @return {*}
 * @author: lkc
 */
void BSP_UART_Reset(void)
{
    rt_ringbuffer_reset(&uart1Rx);
    rt_ringbuffer_reset(&uart1Tx);

    rt_ringbuffer_reset(&uart2Rx);
    rt_ringbuffer_reset(&uart2Tx);

    rt_ringbuffer_reset(&uart3Rx);
    rt_ringbuffer_reset(&uart3Tx);

    rt_ringbuffer_reset(&uart4Rx);
    rt_ringbuffer_reset(&uart4Tx);

    rt_ringbuffer_reset(&uart5Rx);
    rt_ringbuffer_reset(&uart5Tx);

    rt_ringbuffer_reset(&uart6Rx);
    rt_ringbuffer_reset(&uart6Tx);
    return;
}

void UART_Transmit(USART_TypeDef* USARTx, u8 *p,u16 numtemp)
{
    if(USARTx == USART1)
    {
       USART1_Putstring(p,numtemp);
    }
    else if(USARTx == USART2)
    {
       USART2_Putstring(p,numtemp);
    }
    else if(USARTx == USART3)
    {
      USART3_Putstring(p,numtemp);
    }
    else if(USARTx == UART4)
    {
      UART4_Putstring(p,numtemp);
    }
    else if(USARTx == UART5)
    {
      UART5_Putstring(p,numtemp);
    }
    else if(USARTx == USART6)
    {
      USART6_Putstring(p,numtemp);
    }
    else 
    {
    
    }
}


struct_subTimParam Test_UARTxTimParam;
void TTL_UARTx_Test(uint16_t time)
{
    uint8_t len=10;
    uint8_t testUART[10] = {0X00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99};    
    if(GetTimeFlag(&Test_UARTxTimParam,time))
    {       
//      UART_Transmit(USART1,testUART,len);
      UART_Transmit(USART2,testUART,len);
//      UART_Transmit(USART3,testUART,len);
      UART_Transmit(UART4,testUART,len);
//      UART_Transmit(UART5,testUART,len);
//      UART_Transmit(USART6,testUART,len);
    }
}

void TTL_UARTx_Loopout(void)
{
    if(uart1_Msg.param.recvFlag == 1)
    {
       uart1_Msg.param.recvFlag = 0;
       UART_Transmit(USART1,uart1_Msg.recvData,uart1_Msg.param.recvLen);
    }
    
    if(uart2_Msg.param.recvFlag == 1)
    {
       uart2_Msg.param.recvFlag = 0;
       UART_Transmit(USART2,uart2_Msg.recvData,uart2_Msg.param.recvLen);
    }    
    
    if(uart4_Msg.param.recvFlag == 1)
    {
       uart4_Msg.param.recvFlag = 0;
       UART_Transmit(UART4,uart4_Msg.recvData,uart4_Msg.param.recvLen);
    } 
}

void USART1_Putstring(u8 *p,u16 numtemp)
{
    if (NULL != p)
    {
        rt_ringbuffer_put(&uart1Tx, (UCHAR *)p, (ULONG)numtemp);
    }

    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

void USART2_Putstring(u8 *p,u16 numtemp) 
{
    if (NULL != p)
    {
	    rt_ringbuffer_put(&uart2Tx, (UCHAR *)p, (ULONG)numtemp);
    }
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
}

void USART3_Putstring(u8 *p,u16 numtemp)
{
    if (NULL != p)
    {
        rt_ringbuffer_put(&uart3Tx, (UCHAR *)p, (ULONG)numtemp);
    }
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}

void UART4_Putstring(u8 *p,u16 numtemp)
{
    if (NULL != p)
    {
	    rt_ringbuffer_put(&uart4Tx, (UCHAR *)p, (ULONG)numtemp);
    }
	USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
}

void UART5_Putstring(u8 *p,u16 numtemp)
{
    if (NULL != p)
    {
        rt_ringbuffer_put(&uart5Tx, (UCHAR *)p, (ULONG)numtemp);
    }
  	USART_ITConfig(UART5, USART_IT_TXE, ENABLE);
}

void USART6_Putstring(u8 *p,u16 numtemp)
{
    if (NULL != p)
    {
        rt_ringbuffer_put(&uart6Tx, (UCHAR *)p, (ULONG)numtemp);
    }
	USART_ITConfig(USART6, USART_IT_TXE, ENABLE);
}

void rt_hw_console_output(const char *str)
{
    rt_enter_critical();

    while (*str!='\0')
    {
        if (*str=='\n')
        {
            USART_SendData(USART3, '\r');
            while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
        }

        USART_SendData(USART3, *str++);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
    }
    rt_exit_critical();
}

int fputc(int ch, FILE *f)
{
    rt_enter_critical();
    char cStr = (char)ch; 

    if (cStr == '\n')  
    {
        USART_SendData(USART3, (uint8_t)'\r');
        while(RESET == USART_GetFlagStatus(USART3, USART_FLAG_TC));
    }
    USART_SendData(USART3, (uint8_t)cStr);
    while(RESET == USART_GetFlagStatus(USART3, USART_FLAG_TC));
    rt_exit_critical();
    return ch;
}

#include <LowLevelIOInterface.h>
#include "Board.h"
#pragma module_name = "?__write"
size_t __write(int handle, const unsigned char * buffer, size_t size)
{
    return size;

    #if 0
    /*
     * This means that we should flush internal buffers.  Since we
     * don't we just return.  (Remember, "handle" == -1 means that all
     * handles should be flushed.)
    */
    if (buffer == 0)
    {
        return 0;
    }

    /* This template only writes to "standard out" and "standard err",
    * for all other file handles it returns failure. */
    if (handle != _LLIO_STDOUT && handle != _LLIO_STDERR)
    {
        return _LLIO_ERROR;
    }

    rt_enter_critical();
    for(int i = 0; i < size; i++)
    {
        if (buffer[i] == '\n')  
        {
            USART_SendData(USART3, (uint8_t)'\r');
            while(RESET == USART_GetFlagStatus(USART3, USART_FLAG_TC));
        }
        USART_SendData(USART3, (uint8_t)buffer[i]);
        while(RESET == USART_GetFlagStatus(USART3, USART_FLAG_TC));
    }
    rt_exit_critical();
    return size;
#endif
    
}
#ifdef __cplusplus
}
#endif