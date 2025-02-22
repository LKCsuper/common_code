/*
 * @Description: Bsp固件
 * @Version: 2.0
 * @Date: 2023-05-24 08:37:49
 * @LastEditors: Please set LastEditors
 * @detail:
 * 建议：初次写bsp的时候，验证每个外设的测试程序最后留着，方便最后为测试出厂
 *
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro --------------------------------- ----------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 初始化操作系统外设RTOS
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Bsp_RTOS_Init(VOID)
{

    return;
}

/**
 * @description: 硬件初始化前半段
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Init_F(VOID)
{
    PRINTF("Hardware Front Init Success\r\n");

    return;
}

/**
 * @description: 硬件初始化后半段
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Init_R(VOID)
{
    PRINTF("Hardware Rear Init Success\r\n");

    return;
}

/**
 * @description: 固件测试
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Test(VOID)
{
    return;
}
#ifdef __cplusplus
}
#endif
