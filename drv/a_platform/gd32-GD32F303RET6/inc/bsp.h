/*
 * @Description: 
 * @Version: 2.0
 * @Date: 2023-06-29 19:51:44
 * @LastEditors: lkc
 * @detail: 
 */
#ifndef _BSP_H
#define _BSP_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* 这里存储中断优先级,方便安排 */
#define IRQ_TIM3                (6)
#define IRQ_DEBUG_IDE           (5)
#define IRQ_DMA2_STREAM2        (1)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
VOID Bsp_Init_R(VOID);
VOID Bsp_Init_F(VOID);
VOID Bsp_Test(VOID);
#ifdef __cplusplus
}
#endif

#endif
