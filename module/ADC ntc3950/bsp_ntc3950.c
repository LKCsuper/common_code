/*
 * @Description:
 * @Version: 1.0
 * @Author: lkc
 * @detail:
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private define ------------------------------------------------------------*/
#define TABLE_SIZE         (181)
#define ADC1_CONVDATA_SIZE (8)
#define FILTER_TMPER_COUNT (10)
#define TMPER_FILTER_RANGE (3.0)
#define VDD_FILTER_RANGE   (0.4)
#define FILTER_VDD_COUNT   (3)
#define TMPER_DEV          (3)
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
STATIC FLOAT afTmper[TMPER_DEV][FILTER_TMPER_COUNT];
STATIC FLOAT afVdd[FILTER_TMPER_COUNT];
STATIC AVE_RANGE_FILTER stTemperFilter[3];
STATIC AVE_RANGE_FILTER stVddFilter;
// NTC-10K-3950-B值
//-55~125°C对应的电阻阻值表(单位: Ω)
// 阻值~温度换算: 数组下标索引减去55即为温度值
const ULONG g_rvTable[TABLE_SIZE] = {
    1034600, 959006, 889452, 825419, 766434,                                         //-55~-51
    712066,  661926, 615656, 572934, 533466, 496983, 463240, 432015, 403104, 376320, //-50~-41
    351495,  328472, 307110, 287279, 268859, 251741, 235826, 221021, 207242, 194412, //-40~-31
    182460,  171320, 160932, 151241, 142196, 133750, 125859, 118485, 111589, 105139, //-30~-21
    99102,   93450,  88156,  83195,  78544,  74183,  70091,  66250,  66643,  59255,  //-20~-11
    56071,   53078,  50263,  47614,  45121,  42774,  40563,  38480,  36517,  34665,  //-10~-1
    32919,                                                                           // 0
    31270,   29715,  28246,  26858,  25547,  24307,  23135,  22026,  20977,  19987,  // 1~10
    19044,   18154,  17310,  16510,  15752,  15034,  14352,  13705,  13090,  12507,  // 11~20
    11953,   11427,  10927,  10452,  10000,  9570,   9161,   8771,   8401,   8048,   // 21~30
    7712,    7391,   7086,   6795,   6518,   6254,   6001,   5761,   5531,   5311,   // 31~40
    5102,    4902,   4710,   4528,   4353,   4186,   4026,   3874,   3728,   3588,   // 41~50
    3454,    3326,   3203,   3085,   2973,   2865,   2761,   2662,   2567,   2476,   // 51~60
    2388,    2304,   2223,   2146,   2072,   2000,   1932,   1866,   1803,   1742,   // 61~70
    1684,    1627,   1573,   1521,   1471,   1423,   1377,   1332,   1289,   1248,   // 71~80
    1208,    1170,   1133,   1097,   1063,   1030,   998,    968,    938,    909,    // 81~90
    882,     855,    829,    805,    781,    758,    735,    714,    693,    673,    // 91~100
    653,     635,    616,    599,    582,    565,    550,    534,    519,    505,    // 101~110
    491,     478,    465,    452,    440,    428,    416,    405,    395,    384,    // 111~120
    374,     364,    355,    345,    337                                             // 121~125
};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 获取ntc温度传感器
 * @detail:
 * @param {ULONG} adcValue
 * @return {*}
 * @author: lkc
 */
FLOAT TEM_NtcGet(ULONG adcValue)
{
    USHORT i;
    float rValue;
    FLOAT temperature;
    FLOAT ulDiff = 0;

    rValue = 4095000.0;
    rValue = rValue / adcValue - 1000;
    adcValue = 10000000.0 / rValue;
    for (i = 0; i < TABLE_SIZE; i++) {
        if (adcValue > g_rvTable[i]) {
            ulDiff = g_rvTable[i - 1] - g_rvTable[i];
            ulDiff = ((FLOAT)adcValue - g_rvTable[i]) / ulDiff;
            temperature = ((FLOAT)i - 55) - ulDiff;
            break;
        }
    }

    return temperature;
}

/**
 * @description: 读取adc
 * @detail:
 * @param {uint32_t} Channel
 * @return {*}
 * @author: lkc
 */
uint16_t TMP_ReadAdc(uint32_t Channel)
{
    ADC_ChannelConfTypeDef adcConf;
    uint16_t adcData = 0;

    /* wait the the Vrefint used by adc is set */
    while (__HAL_PWR_GET_FLAG(PWR_FLAG_VREFINTRDY) == RESET) {
    };

    // 启动CLK时钟
    __HAL_RCC_ADC1_CLK_ENABLE();

    /*calibrate ADC if any calibraiton hardware*/
    HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED);

    /* Deselects all channels*/
    adcConf.Channel = ADC_CHANNEL_MASK;
    adcConf.Rank = ADC_RANK_NONE;
    HAL_ADC_ConfigChannel(&hadc, &adcConf);

    /* configure adc channel */
    adcConf.Channel = Channel;
    adcConf.Rank = ADC_RANK_CHANNEL_NUMBER;
    HAL_ADC_ConfigChannel(&hadc, &adcConf);

    /* Start the conversion process */
    HAL_ADC_Start(&hadc);

    /* Wait for the end of conversion */
    HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);

    /* Get the converted value of regular channel */
    adcData += HAL_ADC_GetValue(&hadc);
    __HAL_ADC_DISABLE(&hadc);

    __HAL_RCC_ADC1_CLK_DISABLE();

    return adcData;
}

/**
 * @description: 温度初始化
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID TEM_Init(VOID)
{
    ULONG i = 0;
    stTemperFilter[0].pStoreArr = afTmper[0];
    stTemperFilter[1].pStoreArr = afTmper[1];
    stTemperFilter[2].pStoreArr = afTmper[2];
    for (i = 0; i < 3; i++) {
        stTemperFilter[i].fRange = TMPER_FILTER_RANGE;
        stTemperFilter[i].ulTotalNum = FILTER_TMPER_COUNT;
    }

    stVddFilter.pStoreArr = afVdd;
    stVddFilter.fRange = VDD_FILTER_RANGE;
    stVddFilter.ulTotalNum = FILTER_VDD_COUNT;
    return;
}


/**
 * @description: 温度检测
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID TEM_Detect(VOID)
{
    USHORT adcValue[3];
    FLOAT fTmp[3];

    if ((TRUE == gpstStatusParam->ucPwrBattery)) { // 如果当前是电池供电,直接返回
        gpstStatusParam->aucAbnormalStatus[AB_TMP1] = FALSE;
        gpstStatusParam->aucAbnormalStatus[AB_TMP2] = FALSE;
        gpstStatusParam->aucAbnormalStatus[AB_TMP3] = FALSE;
        return;
    }

    adcValue[0] = TMP_ReadAdc(ADC_CHANNEL_0);
    adcValue[1] = TMP_ReadAdc(ADC_CHANNEL_1);
    adcValue[2] = TMP_ReadAdc(ADC_CHANNEL_8);

    for (size_t i = 0; i < 3; i++) {
        fTmp[i] = TEM_NtcGet(adcValue[i]);
    }
    // ! 使能后必须插温度传感器,或者通过判断小于4080生效
#if DETECT_TMPER
    if (TRUE == COMMON_AveRangeFilter(&stTemperFilter[0], fTmp[0], &gpstStatusParam->fTmp[0])) {
        if ((gpstStatusParam->fTmp[0] >= gpstStoreParam->fTmperUp[0] ||
             gpstStatusParam->fTmp[0] <= gpstStoreParam->fTmperLow[0]) &&
            (1 == gpstStoreParam->aucFunEn[AB_TMP1]) && (FALSE == gpstStatusParam->ucPwrBattery)) {
            gpstStatusParam->aucAbnormalStatus[AB_TMP1] = TRUE;
            gpstStatusParam->aucPwrAbnormalStatus[AB_TMP1] = TRUE;
        } else {
            gpstStatusParam->aucAbnormalStatus[AB_TMP1] = FALSE;
        }
    }

    if (TRUE == COMMON_AveRangeFilter(&stTemperFilter[1], fTmp[1], &gpstStatusParam->fTmp[1])) {
        if ((gpstStatusParam->fTmp[1] >= gpstStoreParam->fTmperUp[1] ||
             gpstStatusParam->fTmp[1] <= gpstStoreParam->fTmperLow[1]) &&
            (1 == gpstStoreParam->aucFunEn[AB_TMP2]) && (FALSE == gpstStatusParam->ucPwrBattery)) {
            gpstStatusParam->aucAbnormalStatus[AB_TMP2] = TRUE;
            gpstStatusParam->aucPwrAbnormalStatus[AB_TMP2] = TRUE;
        } else {
            gpstStatusParam->aucAbnormalStatus[AB_TMP2] = FALSE;
        }
    }

    if (TRUE == COMMON_AveRangeFilter(&stTemperFilter[2], fTmp[2], &gpstStatusParam->fTmp[2])) {
        if ((gpstStatusParam->fTmp[2] >= gpstStoreParam->fTmperUp[2] ||
             gpstStatusParam->fTmp[2] <= gpstStoreParam->fTmperLow[2]) &&
            (1 == gpstStoreParam->aucFunEn[AB_TMP3]) && (FALSE == gpstStatusParam->ucPwrBattery)) {
            gpstStatusParam->aucAbnormalStatus[AB_TMP3] = TRUE;
            gpstStatusParam->aucPwrAbnormalStatus[AB_TMP3] = TRUE;
        } else {
            gpstStatusParam->aucAbnormalStatus[AB_TMP3] = FALSE;
        }
#endif
    }
    return;
}

#ifdef __cplusplus
}
#endif
