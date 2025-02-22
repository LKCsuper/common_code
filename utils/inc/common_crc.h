/*
 * @Description:
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-11-15 15:09:37
 * @LastEditors: lkc
 * @LastEditTime: 2024-07-25 11:51:09
 * @detail:
 */
#ifndef __COMMON_CRC_H__
#define __COMMON_CRC_H__
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint8_t Common_BccCal(uint8_t *string, uint16_t length);
#ifdef __cplusplus
}
#endif
#endif
