/*
 * @Description:
 * @Version: 1.0
 * @Author: lkc
 * @detail:
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "common_includes.h"
#include "ringbuffer.h"
#include "drv_uart.h"
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
LONG DRV_UART_Init(void)
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
 * @description: 复位串口 ringbuf
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID DRV_UART_Reset(VOID)
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

/**
 * @description: 串口发送
 * @detail: 
 * @param {ULONG} USART
 * @param {UCHAR} *pucData
 * @param {ULONG} ulDatalen
 * @return {*}
 * @author: lkc
 */
VOID DRV_UART_Transmit(ULONG USART, UCHAR *pucData, ULONG ulDatalen)
{
    switch (USART) {
        case:
            /* code */
            break;

        default:
            break;
    }

    if (NULL == p || 0 == numtemp) {
        return;
    }

    // ! 将数据放入ringbuf中
    // rt_ringbuffer_put(&uart1Tx, (UCHAR *)pucData, (ULONG)ulDatalen);
    // ! 开启发送完成中断
    // USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}
#ifdef __cplusplus
}
#endif