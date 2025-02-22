/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @detail: 
 */
#ifndef __APP_PMBUS_H__
#define __APP_PMBUS_H__
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
FLOAT PMBUS_GetVoltage(UCHAR ucPowerIndex);
VOID PMBUS_GetModel(UCHAR ucPowerIndex, UCHAR *pucModel, UCHAR *pucCount);
VOID PMBUS_Detect(VOID);
#ifdef __cplusplus
}
#endif
#endif
