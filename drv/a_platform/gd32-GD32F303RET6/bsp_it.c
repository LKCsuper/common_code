/*
 * @Description:
 * @detail:
 * @Version: 1.0
 * @Date: 2023-06-28 14:00:27
 * @LastEditTime: 2023-07-24 11:49:52
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
 * @description:
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Usart1_IRQHandler(VOID)
{
    rt_interrupt_enter();
    if (RESET != usart_interrupt_flag_get(GPS_COM, USART_INT_FLAG_IDLE)) // 空闲中断
    {
        ULONG ulBufNum = 0;
        usart_interrupt_flag_clear(GPS_COM, USART_INT_FLAG_IDLE); /* 清除空闲中断标志位 */
        usart_data_receive(GPS_COM);                              /* 清除接收完成标志位 */
        dma_channel_disable(GPS_DMA, GPS_DMA_CH);                 /* 关闭DMA传输 */

        ulBufNum = sizeof(gaucGpsDmaBuf) - dma_transfer_number_get(GPS_DMA, GPS_DMA_CH);
        memcpy(gaucGpsPorcessBuf, gaucGpsDmaBuf, ulBufNum); /* 转存数据到待处理数据缓冲区*/

        /* 队列携带数据长度 */
        rt_mq_send(&gstGpsQueue, (VOID *)&ulBufNum, sizeof(ulBufNum));

        /* 重新设置DMA传输 */
        dma_memory_address_config(GPS_DMA, GPS_DMA_CH, (ULONG)gaucGpsDmaBuf);
        dma_transfer_number_config(GPS_DMA, GPS_DMA_CH, sizeof(gaucGpsDmaBuf));

        dma_channel_enable(GPS_DMA, GPS_DMA_CH); /* 开启DMA传输 */
    }
    rt_interrupt_leave();

    return;
}

/**
 * @description: L716
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Usart2_IRQHandler(VOID)
{
    UCHAR ucData;
    rt_interrupt_enter();
    if (RESET != usart_interrupt_flag_get(L716_COM, USART_INT_FLAG_IDLE)) /* 空闲中断 */
    {
        ULONG ulBufNum = 0;
        usart_interrupt_flag_clear(L716_COM, USART_INT_FLAG_IDLE); /* 清除空闲中断标志位 */
        usart_data_receive(L716_COM);                              /* 清除接收完成标志位 */
        dma_channel_disable(L716_DMA, L716_DMA_CH);                /* 关闭DMA传输 */

        ulBufNum = sizeof(gaucL716DmaBuf) - dma_transfer_number_get(L716_DMA, L716_DMA_CH);

        /* 放到ringbuf中 */
        Ring_Buf_Put(&stServerRecv, (unsigned char *)gaucL716DmaBuf, ulBufNum); /* 转存数据到待处理数据缓冲区*/

        /* 队列携带数据长度 */
        rt_mq_send(&gstL716Queue, (VOID *)&ulBufNum, sizeof(ulBufNum));

        /* 重新设置DMA传输 */
        dma_memory_address_config(L716_DMA, L716_DMA_CH, (ULONG)gaucL716DmaBuf);
        dma_transfer_number_config(L716_DMA, L716_DMA_CH, sizeof(gaucL716DmaBuf));

        dma_channel_enable(L716_DMA, L716_DMA_CH); /* 开启DMA传输 */
    }
    rt_interrupt_leave();

    /* 传输buf空中断 */
    if (RESET != usart_interrupt_flag_get(L716_COM, USART_INT_FLAG_TBE)) {
        if (Ring_Buf_Get(&stServerSend, &ucData, 1)) {
            usart_data_transmit(L716_COM, ucData);
        } else {
            usart_interrupt_disable(L716_COM, L716_IRQ_INT2); /* 发送完成中断 */
        }
    }

    /* 判断是否存在溢出标志 */
    if (RESET != usart_interrupt_flag_get(L716_COM, USART_INT_FLAG_ERR_ORERR)) {
        ucData = usart_data_receive(L716_COM);
    }

    return;
}

/**
 * @description: Mb通讯中断
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Usart3_IRQHandler(VOID)
{
    UCHAR ucData;
    rt_interrupt_enter();
    if (RESET != usart_interrupt_flag_get(MB_COM, USART_INT_FLAG_IDLE)) /* 空闲中断 */
    {
        ULONG ulBufNum = 0;
        usart_interrupt_flag_clear(MB_COM, USART_INT_FLAG_IDLE); /* 清除空闲中断标志位 */
        usart_data_receive(MB_COM);                              /* 清除接收完成标志位 */
        dma_channel_disable(MB_DMA, MB_DMA_CH);                  /* 关闭DMA传输 */

        ulBufNum = sizeof(gaucMbDmaBuf) - dma_transfer_number_get(MB_DMA, MB_DMA_CH);
        /* 放到ringbuf中 */
        Ring_Buf_Put(&stMbRecv, (unsigned char *)gaucMbDmaBuf, ulBufNum); /* 转存数据到待处理数据缓冲区*/

        /* 队列携带数据长度 */
        rt_mq_send(&gstMbQueue, (VOID *)&ulBufNum, sizeof(ulBufNum));

        /* 重新设置DMA传输 */
        dma_memory_address_config(MB_DMA, MB_DMA_CH, (ULONG)gaucMbDmaBuf);
        dma_transfer_number_config(MB_DMA, MB_DMA_CH, sizeof(gaucMbDmaBuf));

        dma_channel_enable(MB_DMA, MB_DMA_CH); /* 开启DMA传输 */
    }
    rt_interrupt_leave();

    /* 传输buf空中断 */
    if (RESET != usart_interrupt_flag_get(MB_COM, USART_INT_FLAG_TBE)) {
        if (Ring_Buf_Get(&stMbSend, &ucData, 1)) {
            usart_data_transmit(MB_COM, ucData);
        } else {
            usart_interrupt_disable(MB_COM, MB_IRQ_INT2); /* 发送完成中断 */
        }
    }

    /* 判断是否存在溢出标志 */
    if (RESET != usart_interrupt_flag_get(MB_COM, USART_INT_FLAG_ERR_ORERR)) {
        ucData = usart_data_receive(MB_COM);
    }

    return;
}
#ifdef __cplusplus
}
#endif
