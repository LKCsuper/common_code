#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/***********************************串口中断************************************************** */
/**
 * @description: 串口1中断
 * @detail:
 * @return {*}
 * @author: lkc
 */
void USART1_IRQHandler(void)
{
    rt_interrupt_enter(); /* 进入中断 */
    uint32_t recvLen = 0;
    char data;

    if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET) // 接收寄存器溢出中断。
    {
        USART_ReceiveData(USART1);
        USART_ClearFlag(USART1, USART_FLAG_ORE);
    }
    if (USART_GetITStatus(USART1, USART_IT_TC) != RESET) {
        USART_ClearITPendingBit(USART1, USART_IT_TC);
    }

    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) // 发送中断标志
    {
        if (rt_ringbuffer_get(&uart1Tx, (rt_uint8_t *)&data, 1)) {
            USART_SendData(USART1, data);
        } else {
            USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
        }
    }

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }

    if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) {
        USART_ReceiveData(USART1);
        DMA_Cmd(DMA2_Stream5, DISABLE);

        uart1_Msg.param.last_recvLen = uart1_Msg.param.recvLen;
        uart1_Msg.param.recvLen = UART1_RX_BUFSIZE - DMA_GetCurrDataCounter(DMA2_Stream5);
        uart1_Msg.param.recvFlag = 1;
        uart1_Msg.param.recvTotalLen += uart1_Msg.param.recvLen;
        uart1_Msg.param.recvPkgCnt++;

        /* ringbuf传递数据 */
        recvLen = UART1_RX_BUFSIZE - DMA_GetCurrDataCounter(DMA2_Stream5);
        rt_ringbuffer_put(&uart1Rx, uart1_Msg.recvData, recvLen);
        /* 通过队列传输(数据长度) */
        rt_mq_send(&uart1RxQueue, (void *)&recvLen, sizeof(recvLen));

        DMA_SetCurrDataCounter(DMA2_Stream5, UART1_RX_BUFSIZE);
        DMA_Cmd(DMA2_Stream5, ENABLE);
    }
    rt_interrupt_leave(); /* 退出中断 */
}

/**
 * @description: 串口2中断
 * @detail:
 * @return {*}
 * @author: lkc
 */
void USART2_IRQHandler(void)
{
    rt_interrupt_enter(); /* 进入中断 */
    uint32_t recvLen = 0;
    char data;

    if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET) // 接收寄存器溢出中断。
    {
        USART_ReceiveData(USART2);
        USART_ClearFlag(USART2, USART_FLAG_ORE);
    }
    if (USART_GetITStatus(USART2, USART_IT_TC) != RESET) {
        USART_ClearITPendingBit(USART2, USART_IT_TC);
    }

    if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET) // 发送中断标志
    {
        if (rt_ringbuffer_get(&uart2Tx, (rt_uint8_t *)&data, 1)) {
            USART_SendData(USART2, data);
        } else {
            USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
        }
    }

    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }

    if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) {
        USART_ReceiveData(USART2);
        DMA_Cmd(DMA1_Stream5, DISABLE);

        uart2_Msg.param.last_recvLen = uart2_Msg.param.recvLen;
        uart2_Msg.param.recvLen = UART2_RX_BUFSIZE - DMA_GetCurrDataCounter(DMA1_Stream5);
        uart2_Msg.param.recvFlag = 1;
        uart2_Msg.param.recvTotalLen += uart2_Msg.param.recvLen;
        uart2_Msg.param.recvPkgCnt++;

        /* ringbuf传递数据 */
        recvLen = UART2_RX_BUFSIZE - DMA_GetCurrDataCounter(DMA1_Stream5);
        rt_ringbuffer_put(&uart2Rx, uart2_Msg.recvData, recvLen);
        /* 通过队列传输(数据长度) */
        rt_mq_send(&uart2RxQueue, (void *)&recvLen, sizeof(recvLen));

        DMA_SetCurrDataCounter(DMA1_Stream5, UART2_RX_BUFSIZE);
        DMA_Cmd(DMA1_Stream5, ENABLE);
    }
    rt_interrupt_leave(); /* 退出中断 */
}

/**
 * @description: 串口3中断
 * @detail:
 * @return {*}
 * @author: lkc
 */
void USART3_IRQHandler(void)
{
    rt_interrupt_enter(); /* 进入中断 */
    uint32_t recvLen = 0;
    char data;

    if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET) // 接收寄存器溢出中断。
    {
        USART_ReceiveData(USART3);
        USART_ClearFlag(USART3, USART_FLAG_ORE);
    }
    if (USART_GetITStatus(USART3, USART_IT_TC) != RESET) {
        USART_ClearITPendingBit(USART3, USART_IT_TC);
    }

    if (USART_GetITStatus(USART3, USART_IT_TXE) != RESET) // 发送中断标志
    {
        if (rt_ringbuffer_get(&uart3Tx, (rt_uint8_t *)&data, 1)) {
            USART_SendData(USART3, data);
        } else {
            USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
        }
    }

    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }

    if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) {
        USART_ReceiveData(USART3);
        DMA_Cmd(DMA1_Stream1, DISABLE);
        uart3_Msg.param.last_recvLen = uart3_Msg.param.recvLen;
        uart3_Msg.param.recvLen = UART3_RX_BUFSIZE - DMA_GetCurrDataCounter(DMA1_Stream1);
        uart3_Msg.param.recvFlag = 1;
        uart3_Msg.param.recvTotalLen += uart3_Msg.param.recvLen;
        uart3_Msg.param.recvPkgCnt++;

        /* ringbuf传递数据 */
        recvLen = UART3_RX_BUFSIZE - DMA_GetCurrDataCounter(DMA1_Stream1);
        rt_ringbuffer_put(&uart3Rx, uart3_Msg.recvData, recvLen);
        /* 通过队列传输(数据长度) */
        // rt_mq_send(&uart3RxQueue, (void *)&recvLen, sizeof(recvLen));

        DMA_SetCurrDataCounter(DMA1_Stream1, UART3_RX_BUFSIZE);

        DMA_Cmd(DMA1_Stream1, ENABLE);
    }
    rt_interrupt_leave(); /* 退出中断 */
}

/**
 * @description: 串口4中断
 * @detail:
 * @return {*}
 * @author: lkc
 */
void UART4_IRQHandler(void)
{
    rt_interrupt_enter(); /* 进入中断 */
    uint32_t recvLen = 0;
    char data;

    if (USART_GetFlagStatus(UART4, USART_FLAG_ORE) != RESET) // 接收寄存器溢出中断。
    {
        USART_ReceiveData(UART4);
        USART_ClearFlag(UART4, USART_FLAG_ORE);
    }
    if (USART_GetITStatus(UART4, USART_IT_TC) != RESET) {
        USART_ClearITPendingBit(UART4, USART_IT_TC);
    }

    if (USART_GetITStatus(UART4, USART_IT_TXE) != RESET) // 发送中断标志
    {
        if (rt_ringbuffer_get(&uart4Tx, (rt_uint8_t *)&data, 1)) {
            USART_SendData(UART4, data);
        } else {
            USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
        }
    }

    if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) {
        USART_ClearITPendingBit(UART4, USART_IT_RXNE);
    }

    if (USART_GetITStatus(UART4, USART_IT_IDLE) != RESET) {
        USART_ReceiveData(UART4);
        DMA_Cmd(DMA1_Stream2, DISABLE);

        uart4_Msg.param.last_recvLen = uart4_Msg.param.recvLen;
        uart4_Msg.param.recvLen = UART4_RX_BUFSIZE - DMA_GetCurrDataCounter(DMA1_Stream2);
        uart4_Msg.param.recvFlag = 1;
        uart4_Msg.param.recvTotalLen += uart4_Msg.param.recvLen;
        uart4_Msg.param.recvPkgCnt++;

        /* ringbuf传递数据 */
        recvLen = UART4_RX_BUFSIZE - DMA_GetCurrDataCounter(DMA1_Stream2);
        rt_ringbuffer_put(&uart4Rx, uart4_Msg.recvData, recvLen);

        /* 通过队列传输(数据长度) */
        rt_mq_send(&uart4RxQueue, (void *)&recvLen, sizeof(recvLen));

        DMA_SetCurrDataCounter(DMA1_Stream2, UART4_RX_BUFSIZE);
        DMA_Cmd(DMA1_Stream2, ENABLE);
    }
    rt_interrupt_leave(); /* 退出中断 */
}

/**
 * @description: 串口5中断
 * @detail:
 * @return {*}
 * @author: lkc
 */
void UART5_IRQHandler(void)
{
    rt_interrupt_enter(); /* 进入中断 */
    uint32_t recvLen = 0;
    char data;

    if (USART_GetFlagStatus(UART5, USART_FLAG_ORE) != RESET) // 接收寄存器溢出中断。
    {
        USART_ReceiveData(UART5);
        USART_ClearFlag(UART5, USART_FLAG_ORE);
    }
    if (USART_GetITStatus(UART5, USART_IT_TC) != RESET) {
        USART_ClearITPendingBit(UART5, USART_IT_TC);
    }

    if (USART_GetITStatus(UART5, USART_IT_TXE) != RESET) // 发送中断标志
    {
        if (rt_ringbuffer_get(&uart5Tx, (rt_uint8_t *)&data, 1)) {
            USART_SendData(UART5, data);
        } else {
            USART_ITConfig(UART5, USART_IT_TXE, DISABLE);
        }
    }

    if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) {
        USART_ClearITPendingBit(UART5, USART_IT_RXNE);
    }

    if (USART_GetITStatus(UART5, USART_IT_IDLE) != RESET) {
        USART_ReceiveData(UART5);
        DMA_Cmd(DMA1_Stream0, DISABLE);

        uart5_Msg.param.last_recvLen = uart5_Msg.param.recvLen;
        uart5_Msg.param.recvLen = UART5_RX_BUFSIZE - DMA_GetCurrDataCounter(DMA1_Stream0);
        uart5_Msg.param.recvFlag = 1;
        uart5_Msg.param.recvTotalLen += uart5_Msg.param.recvLen;
        uart5_Msg.param.recvPkgCnt++;

        /* ringbuf传递数据 */
        recvLen = UART5_RX_BUFSIZE - DMA_GetCurrDataCounter(DMA1_Stream0);
        rt_ringbuffer_put(&uart5Rx, uart5_Msg.recvData, recvLen);
        /* 通过队列传输(数据长度) */
        rt_mq_send(&uart5RxQueue, (void *)&recvLen, sizeof(recvLen));

        DMA_SetCurrDataCounter(DMA1_Stream0, UART5_RX_BUFSIZE);

        DMA_Cmd(DMA1_Stream0, ENABLE);
    }
    rt_interrupt_leave(); /* 退出中断 */
}

/**
 * @description: 串口6中断
 * @detail:
 * @return {*}
 * @author: lkc
 */
void USART6_IRQHandler(void)
{
    rt_interrupt_enter(); /* 进入中断 */
    uint32_t recvLen = 0;
    char data;

    if (USART_GetFlagStatus(USART6, USART_FLAG_ORE) != RESET) // 接收寄存器溢出中断。
    {
        USART_ReceiveData(USART6);
        USART_ClearFlag(USART6, USART_FLAG_ORE);
    }
    if (USART_GetITStatus(USART6, USART_IT_TC) != RESET) {
        USART_ClearITPendingBit(USART6, USART_IT_TC);
    }

    if (USART_GetITStatus(USART6, USART_IT_TXE) != RESET) // 发送中断标志
    {
        if (rt_ringbuffer_get(&uart6Tx, (rt_uint8_t *)&data, 1)) {
            USART_SendData(USART6, data);

        } else {
            USART_ITConfig(USART6, USART_IT_TXE, DISABLE);
        }
    }

    if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET) {
        USART_ClearITPendingBit(USART6, USART_IT_RXNE);
    }

    if (USART_GetITStatus(USART6, USART_IT_IDLE) != RESET) {
        USART_ReceiveData(USART6);
        DMA_Cmd(DMA2_Stream2, DISABLE);
        uart6_Msg.param.last_recvLen = uart6_Msg.param.recvLen;
        uart6_Msg.param.recvLen = UART6_RX_BUFSIZE - DMA_GetCurrDataCounter(DMA2_Stream2);
        uart6_Msg.param.recvFlag = 1;
        uart6_Msg.param.recvTotalLen += uart6_Msg.param.recvLen;
        uart6_Msg.param.recvPkgCnt++;

        /* ringbuf传递数据 */
        recvLen = UART6_RX_BUFSIZE - DMA_GetCurrDataCounter(DMA2_Stream2);
        rt_ringbuffer_put(&uart6Rx, uart6_Msg.recvData, recvLen);
        /* 通过队列传输(数据长度) */
        rt_mq_send(&uart6RxQueue, (void *)&recvLen, sizeof(recvLen));

        DMA_SetCurrDataCounter(DMA2_Stream2, UART6_RX_BUFSIZE);

        DMA_Cmd(DMA2_Stream2, ENABLE);
    }
    rt_interrupt_leave(); /* 退出中断 */
}
/***********************************串口中断***************************************************/

/***********************************掉电检测***************************************************/
/**
 * @description: 掉电检测
 * @detail:
 * @return {*}
 * @author: lkc
 */
void PVD_IRQHandler(void)
{
    rt_enter_critical();
    if (PWR_GetFlagStatus(PWR_FLAG_PVDO)) {
        // TODO 下电存储有固定两个乱码,暂时未定位
        LOG_Store("PD", sizeof("PD")); /* 记录设备下电时间 */

        // TODO 这里存储flash参数是否满足，时间上

        PRINTF("POWER DOWN\r\n");
        /*
            参数保存，写到Flash中
        */
    }
    EXTI_ClearITPendingBit(EXTI_Line16);
    rt_interrupt_leave(); /* 退出中断 */
}
/***********************************掉电检测***************************************************/

/***********************************dma adc转换***************************************************/
/**
 * @description: DMA中断
 * @detail:
 * @return {*}
 * @author: lkc
 */
void DMA2_Stream0_IRQHandler(void)
{
    // !这里调用开关中断函数 直接报错 shall not be used in ISR,可能是CCRAM内存不能在DMA使用
    // rt_enter_critical();
    UCHAR i = 0;
    if (DMA_GetFlagStatus(DMA2_Stream0, DMA_IT_TCIF0) == SET) {
        for (i = 0; i < ADC_CH_CNT; i++) {
            AdcParam.adcData[i] = AdcData.data[i];
        }

        DMA_ClearFlag(DMA2_Stream0, DMA_IT_TCIF0);
    }
    // rt_interrupt_leave();                               /* 退出中断 */
}
/***********************************dma adc转换***************************************************/
#ifdef __cplusplus
}
#endif