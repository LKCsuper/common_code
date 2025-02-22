/*
 * @Description: 
 * @detail: 
 * @Version: 1.0
 * @Date: 2023-06-30 09:07:15
 * @LastEditTime: 2023-07-21 13:54:21
 * @LastEditors: lkc
 */
#ifndef _BSP_UART_H
#define _BSP_UART_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DEBUG_COM                       USART0
#define DEBUG_COM_CLK                   RCU_USART0
#define DEBUG_COM_TX_PIN                GPIO_PIN_9
#define DEBUG_COM_RX_PIN                GPIO_PIN_10
#define DEBUG_COM_GPIO_PORT             GPIOA
#define DEBUG_COM_GPIO_CLK              RCU_GPIOA
#define DEBUG_IRQ                       USART0_IRQn
#define DEBUG_IRQ_INT                   USART_INT_IDLE   

/* gps */
#define GPS_COM                         USART1
#define GPS_COM_CLK                     RCU_USART1
#define GPS_COM_TX_PIN                  GPIO_PIN_2
#define GPS_COM_RX_PIN                  GPIO_PIN_3
#define GPS_COM_GPIO_PORT               GPIOA
#define GPS_COM_GPIO_CLK                RCU_GPIOA
#define GPS_IRQ                         USART1_IRQn
#define GPS_IRQ_INT                     USART_INT_IDLE   

/* 4G IO */
#define L716_COM                        USART2
#define L716_COM_CLK                    RCU_USART2
#define L716_COM_TX_PIN                 GPIO_PIN_10
#define L716_COM_RX_PIN                 GPIO_PIN_11
#define L716_COM_GPIO_PORT              GPIOB
#define L716_COM_GPIO_CLK               RCU_GPIOB
#define L716_IRQ                        USART2_IRQn
#define L716_IRQ_INT1                   USART_INT_IDLE                  /* 空闲中断，用于接收 */
#define L716_IRQ_INT2                   USART_INT_TBE                   /* 发送buf空中断 */
// USART_FLAG_ORERR                                                     /* 接收溢出中断 */ 

/* Mb与核心板通信 IO */
#define MB_COM                        UART3
#define MB_COM_CLK                    RCU_UART3
#define MB_COM_TX_PIN                 GPIO_PIN_10
#define MB_COM_RX_PIN                 GPIO_PIN_11
#define MB_COM_GPIO_PORT              GPIOC
#define MB_COM_GPIO_CLK               RCU_GPIOC
#define MB_IRQ                        UART3_IRQn
#define MB_IRQ_INT1                   USART_INT_IDLE                  /* 空闲中断，用于接收 */
#define MB_IRQ_INT2                   USART_INT_TBE                   /* 发送buf空中断 */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif
