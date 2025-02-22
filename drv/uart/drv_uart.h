#ifndef __DRV_UART_H__
#define __DRV_UART_H__
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define UART1_TX_BUFSIZE  1536
#define UART1_RX_BUFSIZE  512

#define UART2_TX_BUFSIZE  256
#define UART2_RX_BUFSIZE  512

#define UART3_TX_BUFSIZE  256
#define UART3_RX_BUFSIZE  256

#define UART4_TX_BUFSIZE  64
#define UART4_RX_BUFSIZE  256

#define UART5_TX_BUFSIZE  2048
#define UART5_RX_BUFSIZE  2048

#define UART6_TX_BUFSIZE  512
#define UART6_RX_BUFSIZE  256
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern struct rt_ringbuffer uart1Rx;
extern struct rt_ringbuffer uart1Tx;
extern struct rt_ringbuffer uart2Rx;
extern struct rt_ringbuffer uart2Tx;
extern struct rt_ringbuffer uart3Rx;
extern struct rt_ringbuffer uart3Tx;
extern struct rt_ringbuffer uart4Rx;
extern struct rt_ringbuffer uart4Tx;
extern struct rt_ringbuffer uart5Rx;
extern struct rt_ringbuffer uart5Tx;
extern struct rt_ringbuffer uart6Rx;
extern struct rt_ringbuffer uart6Tx;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
int BSP_UART_Init(void);
void UART_Transmit(USART_TypeDef* USARTx, u8 *p,u16 numtemp);
void TTL_UARTx_Test(uint16_t time);
void BSP_UART_Reset(void);
#ifdef __cplusplus
}
#endif
#endif
