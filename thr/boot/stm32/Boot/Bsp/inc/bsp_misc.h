/*
 * @Description:
 * @Version: 2.0
 * @Author: lkc
 * @Date: 2022-11-28 19:28:50
 * @LastEditors: lkc
 * @LastEditTime: 2023-10-11 21:28:27
 */
#ifndef _BSP_MISC_H
#define _BSP_MISC_H

/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Bsp_Misc_Iwdg(UCHAR prv, USHORT rlv);
void Bsp_Delay_MS(volatile uint32_t nms);
void Bsp_Misc_IWdgtReload(VOID);
void Bsp_Misc_Reset(void);
VOID Bsp_Misc_Jump(IN ULONG ulAddr);
#endif
