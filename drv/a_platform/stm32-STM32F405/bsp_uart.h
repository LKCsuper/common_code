/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-09-13 11:34:54
 * @LastEditors: lkc
 * @LastEditTime: 2023-11-10 14:00:11
 * @detail: 
 */
#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "stm32f4xx.h"
#include "usart.h"
#include "stdbool.h"
#include "common.h"
#include "ringbuffer.h"

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

int BSP_UART_Init(void);
void UART_Transmit(USART_TypeDef* USARTx, u8 *p,u16 numtemp);
void TTL_UARTx_Test(uint16_t time);
void BSP_UART_Reset(void);
#endif

