/*
 * @Description: Í·ÎÄ¼þ
 * @detail: 
 * @Version: 1.0
 * @Date: 2023-06-08 09:03:10
 * @LastEditTime: 2023-12-06 09:46:51
 * @LastEditors: lkc
 */
#ifndef _INCLUDES_H
#define _INCLUDES_H
/* Includes ------------------------------------------------------------------*/
#define M_BOOT                                                              (1)
/* std c*/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* std library */
#include "stm32f4xx.h"

/* common */
#include "common.h"
#include "printf_color.h"
#include "ringfifo.h"
#include "common_crc.h"

/* Shell */
#include "cmd.h"

/* conf */
#include "conf_param_iap.h"
#include "conf_param.h"

/* Drv */
#include "gpio.h"

/* bsp */
#include "bsp_usart.h"
#include "bsp_misc.h"
#include "bsp_flash.h"
#include "bsp.h"

/* ymodem */
#include "ymodem_includes.h"

/* iap */
#include "iap.h"

/* shell */
#include "shell_init.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#endif
