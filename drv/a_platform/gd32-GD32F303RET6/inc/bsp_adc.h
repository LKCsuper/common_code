#ifndef _BSP_ADC_H
#define _BSP_ADC_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
int Bsp_Adc_Init(VOID);
float Bsp_Adc_GetVoltage(VOID);
#ifdef __cplusplus
}
#endif
#endif
