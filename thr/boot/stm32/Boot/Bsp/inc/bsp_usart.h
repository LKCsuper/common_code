/*
 * @Description: 
 * @Version: 2.0
 * @Author: lkc
 * @Date: 2022-11-28 19:28:50
 * @LastEditors: lkc
 * @LastEditTime: 2023-10-13 08:18:46
 */
#ifndef _BSP_USART_H
#define _BSP_USART_H

/* Includes ------------------------------------------------------------------*/
#define UART_TX_BUFSIZE                                                 (256)
#define UART_RX_BUFSIZE                                                 (1280)
/* Private typedef -----------------------------------------------------------*/
typedef struct{
    uint16_t   sendIndex;
    uint16_t   sendLen;
    uint8_t    sendbusy;
    uint32_t   sendTotalLen;
    uint32_t   sendPkgCnt;
    uint32_t   sendPkgErr;
    uint8_t    sendWaitFlag;

    uint16_t   last_recvLen;
    uint16_t   recvLen;
    uint8_t    recvFlag;
    uint32_t   recvTotalLen;
    uint32_t   recvPkgCnt;
}struct_UartCommonParam;

typedef struct{
    struct_UartCommonParam  param;	
    uint8_t    sendData[UART_TX_BUFSIZE];
    uint8_t    recvData[UART_RX_BUFSIZE];	
}struct_UartParam;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern struct_UartParam uart_Msg;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* 使用的基�??外�?�串�?? */
#define EVAL_COM1                        USART3
#define EVAL_COM1_CLK                    RCC_APB1Periph_USART3 
#define EVAL_COM1_TX_PIN                 GPIO_Pin_9
#define EVAL_COM1_TX_GPIO_PORT           GPIOA
#define EVAL_COM1_TX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define EVAL_COM1_TX_SOURCE              GPIO_PinSource9
#define EVAL_COM1_TX_AF                  GPIO_AF_USART1

#define EVAL_COM1_RX_PIN                 GPIO_Pin_10
#define EVAL_COM1_RX_GPIO_PORT           GPIOA
#define EVAL_COM1_RX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define EVAL_COM1_RX_SOURCE              GPIO_PinSource10
#define EVAL_COM1_RX_AF                  GPIO_AF_USART1
#define EVAL_COM1_IRQn                   USART1_IRQn
    
/* �??以使用systick 时钟来�?�算时间 看门狗防止程序跑�?? */
VOID    Bsp_PutChar(UCHAR c);
ULONG   Bsp_GetChar(UCHAR* key);
UCHAR   Bsp_Usart_DebugRead(void);
VOID    Bsp_HardWareInit(void);
VOID    Bsp_USART_Init(ULONG ulBound);
#endif
