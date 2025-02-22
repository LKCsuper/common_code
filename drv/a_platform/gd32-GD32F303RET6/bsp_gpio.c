/*
 * @Description: GPIO
 * @detail:
 * @Version: 1.0
 * @Date: 2023-06-30 13:47:36
 * @LastEditTime: 2023-07-02 10:25:57
 * @LastEditors: lkc
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
 * @description: gpio 初始化
 * @detail:
 * @return {*}
 * @author: lkc
 */
int Bsp_Gpio_Init(VOID)
{
    /* 时钟初始化 */
    rcu_periph_clock_enable(RCU_GPIOC);
    /* 模式初始化 */
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_2);

    /* gps电源控制 */
    gpio_bit_set(GPIOC, GPIO_PIN_0);
    /* gps上电 */
    gpio_bit_reset(GPIOC, GPIO_PIN_6);
    /* 4G模组上电 */
    gpio_bit_set(GPIOC, GPIO_PIN_2);
    PRINTF("Bsp Gpio Init\r\n");
    return 0;
}
INIT_BOARD_EXPORT(Bsp_Gpio_Init);
#ifdef __cplusplus
}
#endif
