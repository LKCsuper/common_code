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
 * @description: 重启
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Misc_Reset(VOID)
{
    ERR("Waiting for Reset\r\n");
    rt_thread_delay(1000);
    NVIC_SystemReset();

    return;
}
/**
 * @description: 中断配置
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Nvic_Configuration(VOID)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
    nvic_irq_enable(RTC_IRQn, 0, 0);

    return;
}

/**
 * @description: 杂项配置
 * @detail:
 * @return {*}
 * @author: lkc
 */
int Bsp_Misc_Init(VOID)
{
    Bsp_Nvic_Configuration();

    return 0;
}
#ifdef __cplusplus
}
#endif
