
/*
 * @Description: 
 * @Version: 2.0
 * @Author: lkc
 * @Date: 2022-11-19 10:03:36
 * @LastEditors: lkc
 * @LastEditTime: 2024-07-25 11:49:44
 */
#ifndef _COMMON_TIM_H
#define _COMMON_TIM_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct {
    uint16_t year;      /*!< year*/
    uint8_t week;       /*!< week*/
    uint8_t month;      /*!< month*/
    uint8_t day;        /*!< day*/
    uint8_t hour;       /*!< hour*/
    uint8_t minute;     /*!< minute*/
    uint8_t second;     /*!< second*/
    uint32_t subsecond; /*!< subsecond,us*/
} rtc_calendar_t;
/* Private define ------------------------------------------------------------*/
/* 基础时间，目前按照2000年计算 */
#define TIME_BASE                                                         (1970U)
#define TIMSTAMP_ZONE                                                     (8U)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 判断是否是闰年
 * @detail: 
 * @param {USHORT} usYear
 * @return {*}
 * @author: lkc
 */
UCHAR Common_IsLeap(USHORT usYear);

/**
 * @description: 日历转换为时间戳
 * @detail: 验证
 * @param {rtc_calendar_t} stTim
 * @param {UCHAR} ucIsZone 时区控制
 * @return {*}
 * @author: lkc
 */
ULONG Common_ConvertTimstamp(rtc_calendar_t stTim, UCHAR ucIsZone);

/**
 * @description: 时间戳转换为日历
 * @detail: 验证
 * @param {ULONG} ulTimStamp
 * @param {UCHAR} ucIsZone
 * @return {*}
 * @author: lkc
 */
rtc_calendar_t Common_ConvertCalendar(ULONG ulTimStamp, UCHAR ucIsZone);

rtc_calendar_t Common_ConvertBuildCalendar(void);
#ifdef __cplusplus
}
#endif
#endif
