/*
 * @Description:
 * @detail:
 * @Version: 1.0
 * @Date: 2023-06-28 14:00:27
 * @LastEditTime: 2023-07-21 13:53:02
 * @LastEditors: lkc
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro --------------------------------- ----------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: debug 串口1初始化
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Uart0_DebugInit(VOID)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(DEBUG_COM_GPIO_CLK);

    /* enable USART clock */
    rcu_periph_clock_enable(DEBUG_COM_CLK);

    /* connect port to USARTx_Tx */
    gpio_init(DEBUG_COM_GPIO_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, DEBUG_COM_TX_PIN);

    /* connect port to USARTx_Rx */
    gpio_init(DEBUG_COM_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, DEBUG_COM_RX_PIN);

    /* USART configure */
    usart_deinit(DEBUG_COM);
    usart_baudrate_set(DEBUG_COM, 115200U);
    usart_receive_config(DEBUG_COM, USART_RECEIVE_ENABLE);
    usart_transmit_config(DEBUG_COM, USART_TRANSMIT_ENABLE);
    usart_enable(DEBUG_COM);

    PRINTF("Bsp Uart0 Init\r\n");

    return;
}

/**
 * @description: gps 串口1初始化
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Uart1_GpsInit(VOID)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(GPS_COM_GPIO_CLK);

    /* enable USART clock */
    rcu_periph_clock_enable(GPS_COM_CLK);

    /* connect port to USARTx_Tx */
    gpio_init(GPS_COM_GPIO_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPS_COM_TX_PIN);

    /* connect port to USARTx_Rx */
    gpio_init(GPS_COM_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPS_COM_RX_PIN);

    /* USART configure */
    usart_deinit(GPS_COM);
    usart_baudrate_set(GPS_COM, 115200U);
    usart_receive_config(GPS_COM, USART_RECEIVE_ENABLE);
    usart_transmit_config(GPS_COM, USART_TRANSMIT_ENABLE);
    usart_enable(GPS_COM);

    /* 开启串口3中断,稍微降低gps中断 */
    nvic_irq_enable(GPS_IRQ, 1, 0);
    usart_interrupt_enable(GPS_COM, GPS_IRQ_INT);
    PRINTF("Bsp Uart1 Gps Init\r\n");

    return;
}

/**
 * @description: 4g模组L716初始化
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Uart2_L716Init(VOID)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(L716_COM_GPIO_CLK);

    /* enable USART clock */
    rcu_periph_clock_enable(L716_COM_CLK);

    /* connect port to USARTx_Tx */
    gpio_init(L716_COM_GPIO_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, L716_COM_TX_PIN);

    /* connect port to USARTx_Rx */
    gpio_init(L716_COM_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, L716_COM_RX_PIN);

    /* USART configure */
    usart_deinit(L716_COM);
    usart_baudrate_set(L716_COM, 115200U);
    usart_receive_config(L716_COM, USART_RECEIVE_ENABLE);
    usart_transmit_config(L716_COM, USART_TRANSMIT_ENABLE);
    usart_enable(L716_COM);

    /* 开启串口2中断
    TODO 目前出现串口接收不全情况，目前暂时设置4g中断优先级最高
    */
    nvic_irq_enable(L716_IRQ, 0, 0);
    usart_interrupt_enable(L716_COM, L716_IRQ_INT1); /* 空闲接收中断 */
    PRINTF("Bsp Uart2 L716 Init\r\n");

    return;
}

/**
 * @description: 与核心板通信
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Uart3_MbInit(VOID)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(MB_COM_GPIO_CLK);

    /* enable USART clock */
    rcu_periph_clock_enable(MB_COM_CLK);

    /* connect port to USARTx_Tx */
    gpio_init(MB_COM_GPIO_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, MB_COM_TX_PIN);

    /* connect port to USARTx_Rx */
    gpio_init(MB_COM_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, MB_COM_RX_PIN);

    /* USART configure */
    usart_deinit(MB_COM);
    usart_baudrate_set(MB_COM, 115200U);
    usart_receive_config(MB_COM, USART_RECEIVE_ENABLE);
    usart_transmit_config(MB_COM, USART_TRANSMIT_ENABLE);
    usart_enable(MB_COM);

    /* 开启串口3中断*/
    nvic_irq_enable(MB_IRQ, 0, 0);
    usart_interrupt_enable(MB_COM, MB_IRQ_INT1); /* 空闲接收中断 */
    PRINTF("Bsp Uart3 MB Init\r\n");

    return;
}

/**
 * @description: 串口初始化
 * @detail:
 * @return {*}
 * @author: lkc
 */
int Bsp_Uart_Init(VOID)
{
    Bsp_Uart0_DebugInit();
    Bsp_Uart1_GpsInit();
    Bsp_Uart2_L716Init();
    Bsp_Uart3_MbInit();

    return SUCCESS;
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    CHAR cStr = (CHAR)ch;

    if (cStr == '\n') {
        usart_data_transmit(DEBUG_COM, (uint8_t)'\r');
        while (RESET == usart_flag_get(DEBUG_COM, USART_FLAG_TBE))
            ;
    }
    usart_data_transmit(DEBUG_COM, (uint8_t)cStr);
    while (RESET == usart_flag_get(DEBUG_COM, USART_FLAG_TBE))
        ;
    return ch;
}

#ifdef RT_USING_CONSOLE
void rt_hw_console_output(const char *str)
{
    /* 进入临界段 */
    rt_enter_critical();
    /* 直到字符串结束 */
    while (*str != '\0') {
        /* 换行 */
        if (*str == '\n') {
            usart_data_transmit(DEBUG_COM, (uint8_t)'\r');
            while (RESET == usart_flag_get(DEBUG_COM, USART_FLAG_TBE))
                ;
        }
        usart_data_transmit(DEBUG_COM, (uint8_t)*str++);
        while (RESET == usart_flag_get(DEBUG_COM, USART_FLAG_TBE))
            ;
    }

    /* 退出临界段 */
    rt_exit_critical();
}
#endif

INIT_BOARD_EXPORT(Bsp_Uart_Init);
#ifdef __cplusplus
}
#endif
