/*
 * @Description: bsp初始化
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-10-12 10:06:27
 * @LastEditors: lkc
 * @LastEditTime: 2023-11-09 17:21:51
 * @detail: 
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: bsp外设初始化
 * @detail: 
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Init(VOID)
{
    /* 初始化gpio */
    GPIO_BOARD_Init();

    /* 因为有些情况lora起不来或者异常,软复位必须先断电 */
    LORA_PWR_OFF();
    HWT_PWR_EN_DIS();
    G4_PWR_EN_DIS();
    GPS_PWR_OFF();

    PWR_RSV_OFF();
    PWR_CAMERA_OFF();
    PWR_LASER_OFF();
    PWR_LIGHT_OFF();
    BUZZER_OFF();
    PWR_NANO_OFF();

    Bsp_USART_Init(115200);
    Bsp_FLASH_Init();
    Bsp_Misc_Iwdg(IWDG_Prescaler_64, 0xfff);
    return;
}
#ifdef __cplusplus
}
#endif