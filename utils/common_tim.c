/*
 * @Description:
 * @Version: 2.0
 * @Author: lkc
 * @Date: 2022-11-19 09:57:21
 * @LastEditors: lkc
 * @LastEditTime: 2023-07-25 16:41:07
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static const uint8_t SecondsInMinute = 60;
static const uint16_t SecondsInHour = 3600;
static const uint32_t SecondsInDay = 86400;
/* static const uint8_t HoursInDay = 24; */
/* 闰年一年多少s */
static const uint32_t SecondsInLeapYear = 31622400;
/* 非闰年一年多少s */
static const uint32_t SecondsInYear = 31536000;
static const uint8_t DaysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const uint8_t DaysInMonthLeapYear[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
/**
 * @description: 判断是否是闰年
 * @detail:
 * @param {USHORT} usYear
 * @return {*}
 * @author: lkc
 */
UCHAR Common_IsLeap(USHORT usYear)
{
    return (usYear) % 4 == 0 && ((usYear) % 100 != 0 || (usYear) % 400 == 0);
}

/**
 * @description: 日历转换为时间戳
 * @detail: 验证
 * @param {rtc_calendar_t} stTim
 * @param {UCHAR} ucIsZone 时区控制
 * @return {*}
 * @author: lkc
 */
ULONG Common_ConvertTimstamp(rtc_calendar_t stTim, UCHAR ucIsZone)
{
    ULONG ulTimeCounterTemp = 0;

    for (int16_t i = TIME_BASE; i < stTim.year; i++) {
        if (Common_IsLeap(i)) {
            ulTimeCounterTemp += SecondsInLeapYear;
        } else {
            ulTimeCounterTemp += SecondsInYear;
        }
    }

    /* 判断当年是否是闰年 */
    if (Common_IsLeap(stTim.year)) {
        for (uint8_t i = 0; i < (stTim.month - 1); i++) {
            ulTimeCounterTemp += (uint32_t)(DaysInMonthLeapYear[i] * SecondsInDay);
        }
    } else {
        for (uint8_t i = 0; i < (stTim.month - 1); i++) {
            ulTimeCounterTemp += (uint32_t)(DaysInMonth[i] * SecondsInDay);
        }
    }

    ulTimeCounterTemp += stTim.second + (stTim.minute * SecondsInMinute) + (stTim.hour * SecondsInHour) +
                         ((stTim.day - 1) * SecondsInDay);

    if (1 == ucIsZone) {
        // 加入时区调整
        ulTimeCounterTemp -= TIMSTAMP_ZONE * 60 * 60; // 注意这里是减去时区
    }

    return (ulTimeCounterTemp);
}

/**
 * @description: 时间戳转换为日历
 * @detail: 验证
 * @param {ULONG} ulTimStamp
 * @param {UCHAR} ucIsZone
 * @return {*}
 * @author: lkc
 */
rtc_calendar_t Common_ConvertCalendar(ULONG ulTimStamp, UCHAR ucIsZone)
{
    rtc_calendar_t stTim;
    ULONG j, iDay;
    ULONG lDay;

    /* 清零结构体 */
    memset(&stTim, 0, sizeof(rtc_calendar_t));

    if (1 == ucIsZone) {
        ulTimStamp += TIMSTAMP_ZONE * 60 * 60;
    }

    /* 计算天和s */
    lDay = ulTimStamp / SecondsInDay;
    ulTimStamp = ulTimStamp % SecondsInDay;

    stTim.year = TIME_BASE;
    /* 计算之前多少年 */
    while (lDay > 365) {
        if (Common_IsLeap(stTim.year)) // 闰年
        {
            lDay -= 366;
        } else {
            lDay -= 365;
        }
        stTim.year++;
    }

    /* 计算上今年 */
    if ((lDay == 365) && !(Common_IsLeap(stTim.year))) {
        lDay -= 365;
        stTim.year++;
    }

    for (j = 0; j < 12; j++) {
        if ((j == 1) && (Common_IsLeap(stTim.year))) {
            iDay = 29;
        } else {
            iDay = DaysInMonth[j];
        }
        if (lDay >= iDay) {
            lDay -= iDay;
        } else {
            break;
        }
    }
    stTim.month = j + 1;
    stTim.day = lDay + 1;
    stTim.hour = (ulTimStamp / 3600) % 24;
    stTim.minute = (ulTimStamp % 3600) / 60;
    stTim.second = (ulTimStamp % 3600) % 60;

    return stTim;
}

/**
 * @description: ×ª»»±àÒëÈÕÀú
 * @detail:
 * @return {*}
 * @author: lkc
 */
rtc_calendar_t Common_ConvertBuildCalendar(void)
{
    char arrDate[36]; // Jul 03 2018
    char arrTime[36]; // 06:17:05
    char pDest[36];
    rtc_calendar_t stTime;

    sprintf(arrDate, "%s", __DATE__); // Jul 03 2018
    sprintf(arrTime, "%s", __TIME__); // 06:17:05

    if (STRSTR(__DATE__, "Jan") != NULL) {
        stTime.month = 1;
    } else if (STRSTR(__DATE__, "Feb") != NULL) {
        stTime.month = 2;
    } else if (STRSTR(__DATE__, "Mar") != NULL) {
        stTime.month = 3;
    } else if (STRSTR(__DATE__, "Apr") != NULL) {
        stTime.month = 4;
    } else if (STRSTR(__DATE__, "May") != NULL) {
        stTime.month = 5;
    } else if (STRSTR(__DATE__, "Jun") != NULL) {
        stTime.month = 6;
    } else if (STRSTR(__DATE__, "Jul") != NULL) {
        stTime.month = 7;
    } else if (STRSTR(__DATE__, "Aug") != NULL) {
        stTime.month = 8;
    } else if (STRSTR(__DATE__, "Sep") != NULL) {
        stTime.month = 9;
    } else if (STRSTR(__DATE__, "Oct") != NULL) {
        stTime.month = 10;
    } else if (STRSTR(__DATE__, "Nov") != NULL) {
        stTime.month = 11;
    } else if (STRSTR(__DATE__, "Dec") != NULL) {
        stTime.month = 12;
    } else {
        stTime.month = 1;
    }

    sprintf(pDest, "%s", STRNCPY(pDest, arrDate + 4, 2));
    stTime.day = atoi(pDest);

    sprintf(pDest, "%s", STRNCPY(pDest, arrDate + 4 + 3, 4));
    stTime.year = atoi(pDest);

    STRNCPY(pDest, arrTime, 2);
    pDest[2] = '\0';
    stTime.hour = atoi(pDest);
    STRNCPY(pDest, arrTime + 3, 2);
    pDest[2] = '\0';
    stTime.minute = atoi(pDest);
    STRNCPY(pDest, arrTime + 3 + 3, 2);
    pDest[2] = '\0';
    stTime.second = atoi(pDest);

    return stTime;
}
#ifdef __cplusplus
}
#endif
