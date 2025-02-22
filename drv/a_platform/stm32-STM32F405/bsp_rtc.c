/*
 * @Description: 时间
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-10-10 13:44:44
 * @LastEditors: lkc
 * @LastEditTime: 2023-10-20 17:19:43
 * @detail: 
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_rtc.h"
#include "rtthread.h"

#include "app_flash.h"
#include "bsp_rtc.h"
// #include "common.h"
// #include "common_tim.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: rtc 获取时间
 * @detail: 
 * @return {*}
 * @author: lkc
 */
rtc_calendar_t Bsp_Rtc_GetTim(void)
{
    rtc_calendar_t rtcData;

    RTC_TimeTypeDef   RTC_TimeStructure;
    RTC_DateTypeDef   RTC_DateStructure;

    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

    rtcData.year = RTC_DateStructure.RTC_Year + 2000;
    rtcData.month = RTC_DateStructure.RTC_Month;
    rtcData.day = RTC_DateStructure.RTC_Date;
    rtcData.week = RTC_DateStructure.RTC_WeekDay;

    rtcData.hour = RTC_TimeStructure.RTC_Hours;
    rtcData.minute = RTC_TimeStructure.RTC_Minutes;
    rtcData.second = RTC_TimeStructure.RTC_Seconds;

    return rtcData;
}

/**
 * @description: rtc 设置时间
 * @detail: 
 * RTC_DateStructInit 没有初始化可能会导致时间年错误
 * @return {*}
 * @author: lkc
 */
void Bsp_Rtc_SetTim(rtc_calendar_t stTim)
{
    RTC_DateTypeDef RTC_DateStructure;
    RTC_TimeTypeDef  RTC_TimeStructure;
    
    RTC_TimeStructInit(&RTC_TimeStructure);
    RTC_DateStructInit(&RTC_DateStructure);

    /* Set the time to 05h 20mn 00s AM */
    RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
    RTC_TimeStructure.RTC_Hours   = stTim.hour;
    RTC_TimeStructure.RTC_Minutes = stTim.minute;
    RTC_TimeStructure.RTC_Seconds = stTim.second; 
    
    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);  

    RTC_DateStructure.RTC_Year = stTim.year - 2000;
    RTC_DateStructure.RTC_Month = stTim.month;
    RTC_DateStructure.RTC_Date = stTim.day;
    RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure); 

    return;
}

/**
 * @description: rtc 获取时间
 * @detail: 
 * @return {*}
 * @author: lkc
 */
uint32_t Bsp_Rtc_GetTimStamp(void)
{
    return RTC_GetTimeStampSubSecond();
}
#ifdef __cplusplus
}
#endif


