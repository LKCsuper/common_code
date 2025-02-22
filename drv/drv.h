#ifndef __DRV_H__
#define __DRV_H__
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* c */
#include "stdint.h"
#include "stdio.h"

#include "common.h"

#include "drv_flash.h"
#include "drv_gpio.h"
#include "drv_misc.h"
#include "drv_uart.h"
#include "drv_spi.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
VOID DRV_Init(VOID);
#ifdef __cplusplus
}
#endif
#endif
