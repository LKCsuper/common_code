/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-10-30 13:29:55
 * @LastEditors: lkc
 * @LastEditTime: 2023-12-19 08:56:51
 * @detail: 
 */
#ifndef __BSP_PWR_H__
#define __BSP_PWR_H__
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
void PWR_Extern(PWR_STATE_E enPwr);
void PWR_Module(PWR_STATE_E enPwr);
void PWR_RTK(PWR_STATE_E enPwr);
#ifdef __cplusplus
}
#endif
#endif
