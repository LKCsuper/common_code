#ifndef _BSP_RTC_H
#define _BSP_RTC_H
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
rtc_calendar_t Bsp_Rtc_GetTim(VOID);
VOID Bsp_Rtc_SetTim(rtc_calendar_t stTim);
ULONG Bsp_Rtc_GetTimStamp(VOID);
VOID Bsp_Rtc_SetTimStamp(ULONG ulTimstamp);
#ifdef __cplusplus
}
#endif
#endif
