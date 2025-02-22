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
/**
 * @description: rtc 获取时间
 * @detail: 
 * @return {*}
 * @author: lkc
 */
rtc_calendar_t Bsp_Rtc_GetTim(VOID)
{
    rtc_calendar_t stTim;
    ULONG ulTimstamp;

    /* 设置时间 */
    ulTimstamp = rtc_counter_get();
    rtc_lwoff_wait();

    stTim = Common_ConvertCalendar(ulTimstamp);

    return stTim;
}

/**
 * @description: rtc 获取时间戳
 * @detail: 
 * @return {*}
 * @author: lkc
 */
ULONG Bsp_Rtc_GetTimStamp(VOID)
{
    ULONG ulTimstamp;

    /* 设置时间 */
    ulTimstamp = rtc_counter_get();
    rtc_lwoff_wait();

    ulTimstamp = ulTimstamp - 8 * 60 * 60;

    return ulTimstamp;
}

/**
 * @description: rtc设置时间
 * @detail: 
 * @param {rtc_calendar_t} stTim
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Rtc_SetTim(rtc_calendar_t stTim)
{
    ULONG ulTimstamp = 0;
    ulTimstamp = Common_ConvertTimstamp(stTim);
    /* 设置时间 */
    rtc_counter_set(ulTimstamp);
    rtc_lwoff_wait();
    
    return;
}

/**
 * @description: rtc设置时间
 * @detail: 
 * @param {rtc_calendar_t} stTim
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Rtc_SetTimStamp(ULONG ulTimstamp)
{
    /* 设置时间 */
    rtc_counter_set(ulTimstamp);
    rtc_lwoff_wait();
    
    return;
}

/**
 * @description: Rtc 初始化
 * @detail: 
 * @return {*}
 * @author: lkc
 */
int Bsp_Rtc_Init(VOID)
{
    /* enable PMU and BKPI clocks */
    rcu_periph_clock_enable(RCU_BKPI);
    rcu_periph_clock_enable(RCU_PMU);
    /* allow access to BKP domain */
    pmu_backup_write_enable();

    /* reset backup domain */
    bkp_deinit();

#if 0 /* 尝试使用该时钟失败 */
    /* enable LXTAL */
    rcu_osci_on(RCU_LXTAL);
    /* wait till LXTAL is ready */
    rcu_osci_stab_wait(RCU_LXTAL);
    
    /* select RCU_LXTAL as RTC clock source */
    rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);
#endif 

    /* enable LXTAL */
    rcu_osci_on(RCU_HXTAL);
    /* wait till LXTAL is ready */
    rcu_osci_stab_wait(RCU_HXTAL);
    
    /* select RCU_LXTAL as RTC clock source */
    rcu_rtc_clock_config(RCU_RTCSRC_HXTAL_DIV_128);

    /* enable RTC Clock */
    rcu_periph_clock_enable(RCU_RTC);

    /* wait for RTC registers synchronization */
    rtc_register_sync_wait();

    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
    rcu_all_reset_flag_clear();

#if 0
    /* enable the RTC second interrupt*/
    rtc_interrupt_enable(RTC_INT_SECOND);
    rtc_interrupt_enable(RTC_INT_ALARM);
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
    /* set RTC prescaler: set RTC period to 1s */
    rtc_prescaler_set(32767);

    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
#endif
    /* 设置默认时间，测试使用 */
	rtc_calendar_t stTim;
    memset(&stTim, 0, sizeof(rtc_calendar_t));
    stTim.year = 2005;
    stTim.month = 5;
    stTim.day = 8;
    stTim.hour = 5;
    stTim.minute = 20;
    stTim.second = 20;
    Bsp_Rtc_SetTim(stTim);

    return 0;
}
INIT_BOARD_EXPORT(Bsp_Rtc_Init);
#ifdef __cplusplus
}
#endif
