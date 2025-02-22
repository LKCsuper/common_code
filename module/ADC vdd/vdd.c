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
VOID VDD_Detect(VOID)
{
    USHORT adcValue;
    FLOAT fTmp;
    adcValue = TMP_ReadAdc(ADC_CHANNEL_VREFINT);

    fTmp = ((FLOAT)__LL_ADC_CALC_VREFANALOG_VOLTAGE(adcValue, LL_ADC_RESOLUTION_12B)) / 1000.0;

    if (TRUE == COMMON_AveRangeFilter(&stVddFilter, fTmp, &gpstStatusParam->fVdd)) {
        // TODO 滤波电压
        if (gpstStatusParam->fVdd < 3.00) {
            PRINTF("%f \n", gpstStatusParam->fVdd);
            gpstStatusParam->aucAbnormalStatus[AB_VDD] = TRUE;
        } else {
            gpstStatusParam->aucAbnormalStatus[AB_VDD] = FALSE;
        }
    }

    return;
}
#ifdef __cplusplus
}
#endif