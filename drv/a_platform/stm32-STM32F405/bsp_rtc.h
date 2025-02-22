/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-10-10 13:44:51
 * @LastEditors: lkc
 * @LastEditTime: 2023-10-12 13:57:49
 * @detail: 
 */
#ifndef __BSP_RTC_H__
#define __BSP_RTC_H__
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "app_flash.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
rtc_calendar_t Bsp_Rtc_GetTim(void);
uint32_t Bsp_Rtc_GetTimStamp(void);
void Bsp_Rtc_SetTim(rtc_calendar_t stTim);
#ifdef __cplusplus
}
#endif
#endif
