/*
 * @Description: 
 * @detail: 
 * @Version: 1.0
 * @Date: 2023-06-30 11:50:18
 * @LastEditTime: 2023-07-02 11:11:28
 * @LastEditors: lkc
 */
#ifndef _BSP_IT_H
#define _BSP_IT_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
VOID Bsp_Usart1_IRQHandler(VOID);
VOID Bsp_Usart2_IRQHandler(VOID);
VOID Bsp_Usart3_IRQHandler(VOID);
#ifdef __cplusplus
}
#endif
#endif
