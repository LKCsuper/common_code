/*
 * @Description: 环境失效保护模组已经实验
 * 这个命令行最主要的作用不在于功能而是主要是最小化,节省空间实现,移植其他命令行太耗费时间
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
/* Private typedef -----------------------------------------------------------*/
#define ENV_HEAD (0X05)
#define ENV_END  (0X06)
typedef enum tagEnvAgree {
    ENV_IMPORT_PUB = 0xb0,
    ENV_IMPORT_SIG,
    ENV_GEN_RANDOM,
    ENV_BOOT,
} ENV_AGREE_E;

UCHAR gaucRandom[32] = {0};
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UCHAR gaucPrompt[10] = {0};
/* 命令 回调 help */
cmd_t gastCmd[] = {
    {"reset", "reset", 0, Shell_Reset},                          //
    {"help", "help", 0, Shell_List},                             //
    {"set_temper", "set_tmper 1 0 70", 0, Shell_SetTmper},        //
    {"set_voltage", "set_voltage 170 235", 0, Shell_SetVoltage}, //
    {"show", "show param", 0, Shell_Show},                       //
    {"de_param", "default param", 0, Shell_ResetParam},          //
    {"set_trigger", "set_trigger 1 or 0", 0, Shell_SetTrigger},
    {"log_show", "show log", 0, Shell_Log},
    {"log_init", "init log", 0, Shell_LogInit},
    {"config_set", "config set", 0, SHELL_ConfigSet},
    {"param_show", "param show", 0, SHELL_ConfigGet},
    {"tim_set", "tim set", 0, SHELL_TimSet},
    {"usr_add", "usr add", 0, SHELL_UsrAdd},
    {"usr_login", "usr_login 0", 0, SHELL_UsrLogin},
    {"usr_out", "usr out", 0, SHELL_UsrOut},
    {"gen_random", "gen random", 0, SHELL_GenRandom},
    {"pub", "show pub", 0, SHELL_ShowPub},
    {"boot", "boot", 0, SHELL_Boot},
};
UCHAR aucData[FIFO_RX_BUF_SIZE];
_fifo_t stRxFifo;
fifo_t stFifo;
fifo_t stFifo1;
UCHAR aucRx2FifoBuf[FIFO_RX_BUF_SIZE];
UCHAR aucRx1FifoBuf[FIFO_RX_BUF_SIZE];
UCHAR aucPubKey[64] = {0};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 查找字符串
 * @detail:
 * @param {UCHAR} *pucStr
 * @param {ULONG} ulStart
 * @param {ULONG} ulEnd
 * @param {UCHAR} ucCh
 * @return {*}
 * @author: lkc
 */
ULONG STR_FindCh(UCHAR *pucStr, ULONG ulStart, ULONG ulEnd, UCHAR ucCh)
{
    ULONG i = 0;
    for (i = ulStart; i < ulEnd; i++) {
        if (ucCh == pucStr[i]) {
            return i;
        }
    }
    return 0;
}

/**
 * @description: 扫描参数
 * @detail:
 * @param {UCHAR} *pucStr
 * @param {UCHAR} *pucValue
 * @param {ULONG} ulSize
 * @return {*}
 * @author: lkc
 */
BOOL SHELL_ScanParam(UCHAR *pucStr, LONG *ulValue, ULONG ulParamNum)
{
    ULONG i = 0;
    ULONG j = 0;
    ULONG ulCmdLen = strlen((const char *)pucStr);
    ULONG ulTmp = 0;
    ULONG ulIndex1 = 0;

    ulTmp = ulCmdLen - 1;
    for (j = 0; j < ulParamNum; j++) {
        for (i = ulTmp + 1; i < sizeof(aucData); i++) {
            if (32 != aucData[i]) {
                ulIndex1 = i;
                break;
            }
        }

        ulTmp = STR_FindCh(aucData, ulIndex1, sizeof(aucData), 32);
        aucData[ulTmp] = '\0';
        ulValue[j] = atoi((const char *)&aucData[ulIndex1]);
    }

    return SUCCESS;
}

VOID SHELL_RetJson(UCHAR isSuccess)
{
    if (SUCCESS == isSuccess) {
        printf("{\"status\": \"success\"}\n");
    } else {
        printf("{\"status\": \"err\"}\n");
    }

    return;
}

/**
 * @description: 协议处理
 * @detail:
 * @param {UCHAR} *pucData
 * @param {ULONG} ulLen
 * @return {*}
 * @author: lkc
 */
BOOL SHELL_ProAgree(UCHAR *pucData, ULONG ulLen)
{
    ULONG ulDataLen = 0;

    if ((ulLen < 5) && (128 != ulLen)) {
        SHELL_RetJson(ERROR);
        return FALSE;
    }

    if (ENV_HEAD == pucData[0]) {
        ulDataLen = pucData[1];
        if (ENV_END != pucData[ulDataLen - 1]) {
            SHELL_RetJson(ERROR);
            return false;
        }

        switch (pucData[2]) {
            case ENV_IMPORT_PUB:
                // SHELL_RetJson(SUCCESS);
                break;
            case ENV_IMPORT_SIG:
                if (0 == pucData[3]) {
                    SHELL_RetJson(ERROR);
                    return FALSE;
                }
                SHELL_RetJson(SUCCESS);
                break;
            case ENV_GEN_RANDOM:
                memcpy(gaucRandom, &pucData[3], 32);
                printf("{");
                // 输出 status 字段
                printf("\"status\": \"success\", ");
                // 输出 random 字段
                printf("\"random\": \"");
                for (size_t i = 0; i < 32; i++) {
                    printf("%02X", gaucRandom[i]);
                }
                printf("\"}");
                printf("\n");
                break;
            case ENV_BOOT: {
                SHELL_RetJson(SUCCESS);
            } break;
            default:
                SHELL_RetJson(ERROR);
                return FALSE;
        }
    } else {
        SHELL_RetJson(ERROR);
        // for (size_t i = 0; i < ulLen; i++)
        // {
        //     PRINTF("%c", pucData[i]);
        // }
    }

    return TRUE;
}

/**
 * @description: 显示公钥
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID SHELL_ShowPub(VOID)
{
    PRINTF("begin\n");

    // 打印 JSON 对象的开头
    PRINTF("{\n");
    PRINTF("  \"status\": \"success\",\n");
    // 打印 pub0
    PRINTF("  \"pub0\": \"");
    PRINT_BUF(NULL, gpstUsrParam->aucPub[0], 64);
    PRINTF("\",\n");

    // 打印 pub1
    PRINTF("  \"pub1\": \"");
    PRINT_BUF(NULL, gpstUsrParam->aucPub[1], 64);
    PRINTF("\",\n");

    // 打印 pub2
    PRINTF("  \"pub2\": \"");
    PRINT_BUF(NULL, gpstUsrParam->aucPub[2], 64);
    PRINTF("\"\n");

    // 打印 JSON 对象的结尾
    PRINTF("}\n");

    PRINTF("end\n");
    return;
}

/**
 * @description: 读取buf
 * @detail:
 * @param {UCHAR} *pucData
 * @return {*}
 * @author: lkc
 */
ULONG SHELL_GetCkBuf(UCHAR *pucData)
{
    ULONG i = 0;
    ULONG ulReadSize = 10000;

    /* 串口读取 */
    while (ulReadSize--) {
        if (TRUE == fifo_get(&stFifo1, &aucData[i])) {
            i++;
        }
    }

    return i;
}

VOID LPUART_SendData(UCHAR *data, USHORT size)
{
    for (USHORT i = 0; i < size; i++) {
        // 等待发送寄存器空
        while (!LL_LPUART_IsActiveFlag_TXE(LPUART1))
            ;
        LL_LPUART_TransmitData8(LPUART1, data[i]);
    }
    // 等待最后一个字节发送完
    while (!LL_LPUART_IsActiveFlag_TC(LPUART1))
        ;
}

VOID CK2_SEND(UCHAR ucProtocol, UCHAR *data, USHORT size)
{
    MEMSET(aucData, 0x00, sizeof(aucData));
    aucData[0] = ENV_HEAD;
    aucData[1] = 4 + size;
    aucData[2] = ucProtocol;
    if (0 != size) {
        MEMCPY(&aucData[3], data, size);
    }
    aucData[3 + size] = ENV_END;

    LPUART_SendData(aucData, 128);
}

VOID SHELL_Boot(VOID)
{
    ULONG ulCount = 0;

    PRINTF("begin\n");
    /* 串口发送 */
    CK2_SEND(ENV_BOOT, aucData, 0);
    memset(aucData, 0x00, sizeof(aucData));
    ulCount = SHELL_GetCkBuf(aucData);

    if (0 == ulCount) {
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }

    SHELL_ProAgree(aucData, ulCount);

    PRINTF("end\n");
    return;
}

/**
 * @description: 产生随机数
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID SHELL_GenRandom(VOID)
{
    ULONG ulCount = 0;
    PRINTF("begin\n");
    /* 串口发送 */
    CK2_SEND(ENV_GEN_RANDOM, aucData, 0);
    memset(aucData, 0x00, sizeof(aucData));
    ulCount = SHELL_GetCkBuf(aucData);

    if (0 == ulCount) {
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }

    // PRINT_BUF("RANDOM\n", aucData, ulCount);

    SHELL_ProAgree(aucData, ulCount);

    PRINTF("end\n");

    return;
}

/**
 * @description: 添加用户
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID SHELL_UsrAdd(VOID)
{
    ULONG ulDataLen = STRLEN((char *)aucData);
    LONG alValue[2] = {0};
    PRINTF("begin\n");
    if ((10 + 128) != ulDataLen) {
        SHELL_RetJson(ERROR);
        return;
    }

    /* 增加哪一个用户 */
    SHELL_ScanParam("usr_add", alValue, 2);

    if (alValue[0] > 3 || alValue[1] < 0) {
        /* err */
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }
    COMMON_StrToHex((char *)&aucData[10], gpstUsrParam->aucPub[alValue[0]], 64, 4, BIG_ENDI);
    /* 存储参数 */
    PARAM_UsrStore();
    SHELL_RetJson(SUCCESS);
    PRINTF("end\n");

    return;
}

/**
 * @description: 用户登录
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID SHELL_UsrLogin(VOID)
{
    UCHAR aucSignature[64];
    LONG lValue;
    ULONG ulCount = 0;
    ULONG i = 0;

    /* 登录哪个用户 */
    PRINTF("begin\n");

    /* 扫描哪一个用户 */
    SHELL_ScanParam("usr_login", &lValue, 2);

    if (lValue < 0 || lValue > 2) {
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }

    /* 获取签名 */
    COMMON_StrToHex((char *)&aucData[12], aucSignature, 64, 4, BIG_ENDI);
    /* 发送公钥 */
    CK2_SEND(ENV_IMPORT_PUB, gpstUsrParam->aucPub[lValue], 64);
    memset(aucData, 0x00, sizeof(aucData));
    ulCount = SHELL_GetCkBuf(aucData);
    if (0 == ulCount) {
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }

    // PRINT_BUF("ENV_IMPORT_PUB\n", aucData, ulCount);
    if (FALSE == SHELL_ProAgree(aucData, ulCount)) {
        PRINTF("end\n");
        return;
    }

    CK2_SEND(ENV_IMPORT_SIG, aucSignature, 64);
    memset(aucData, 0x00, sizeof(aucData));
    HAL_Delay(100);
    ulCount = SHELL_GetCkBuf(aucData);

    if (0 == ulCount) {
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }

    // PRINT_BUF("IMPORT SIG\n", aucData, ulCount);
    if (FALSE == SHELL_ProAgree(aucData, ulCount)) {
        gpstStatusParam->aucUsrLogin[lValue] = FALSE;
    } else {
        gpstStatusParam->aucUsrLogin[lValue] = TRUE;
    }

    gpstStatusParam->aucUsrTotal = 0;
    for (i = 0; i < ARRARY_LENGTH(gpstStatusParam->aucUsrLogin); i++) {
        if (TRUE == gpstStatusParam->aucUsrLogin[i]) {
            gpstStatusParam->aucUsrTotal++;
        }
    }

    PRINTF("end\n");
    return;
}

/**
 * @description: 用户登出
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID SHELL_UsrOut(VOID)
{
    LONG lValue = 5;
    ULONG i = 0;
    SHELL_ScanParam("usr_out", &lValue, 1);
    PRINTF("begin\n");
    if (lValue < 0 || lValue > 2) {
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }
    SHELL_RetJson(SUCCESS);
    gpstStatusParam->aucUsrLogin[lValue] = FALSE;
    PRINTF("end\n");

    gpstStatusParam->aucUsrTotal = 0;
    for (i = 0; i < ARRARY_LENGTH(gpstStatusParam->aucUsrLogin); i++) {
        if (TRUE == gpstStatusParam->aucUsrLogin[i]) {
            gpstStatusParam->aucUsrTotal++;
        }
    }

    return;
}

/**
 * @description:
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID SHELL_TimSet(VOID)
{
    LONG lValue[6];
    rtc_calendar_t stTim;

    PRINTF("begin\n");

    if (0 == gpstStatusParam->aucUsrTotal) {
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }

    SHELL_ScanParam("tim_set", lValue, ARRARY_LENGTH(lValue));
    stTim.year = lValue[0];
    stTim.month = (uint8_t)lValue[1];
    stTim.day = (uint8_t)lValue[2];
    stTim.hour = (uint8_t)lValue[3];
    stTim.minute = (uint8_t)lValue[4];
    stTim.second = (uint8_t)lValue[5];
    gpstStatusParam->ulTimStamp = Common_ConvertTimstamp(stTim, 1);
    PCF8563_SetDateTime(&stTim);
    SHELL_RetJson(SUCCESS);
    PRINTF("end\n");

    return;
}

/**
 * @description: 配置设置
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID SHELL_ConfigSet(VOID)
{
    LONG ulValue[2] = {0};

    PRINTF("begin\n");

    if (0 == gpstStatusParam->aucUsrTotal) {
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }

    SHELL_ScanParam("config_set", ulValue, 2);
    if (ulValue[0] < AB_NUM && (0 == ulValue[1] || 1 == ulValue[1])) {
        gpstStoreParam->aucFunEn[ulValue[0]] = ulValue[1];
        SHELL_RetJson(SUCCESS);
    } else {
        SHELL_RetJson(ERROR);
    }
    PRINTF("end\n");

    return;
}

/**
 * @description: 打印json
 * @detail:
 * @return {*}
 * @author: lkc
 */
void print_json_object(unsigned long ulUsrLogin, unsigned long ulConfig, float voltageUp, float voltageLow,
                       float temperUp[3], float temperLow[3], unsigned char ucTrigger, float tmp[3], float voltage[2],
                       float vdd, unsigned long ulTimStamp, unsigned char ucPmbusOk, unsigned long ulAbLog,
                       unsigned long ulPwrLog, unsigned char status)
{
    PRINTF("begin\n");
    // 打印 JSON 结构
    PRINTF("{\n");

    // 输出 status 字段
    if (status == SUCCESS) {
        PRINTF("  \"status\": \"success\",\n");
    } else {
        PRINTF("  \"status\": \"error\",\n");
    }

    PRINTF("  \"CONFIG\": \"0x%lx\",\n", ulConfig);
    PRINTF("  \"USR\": \"0x%lx\",\n", ulUsrLogin);
    PRINTF("  \"VOLTAGE_UP\": %.3f,\n", voltageUp);
    PRINTF("  \"VOLTAGE_LOW\": %.3f,\n", voltageLow);

    // 打印温度上限和下限
    for (int i = 0; i < 3; i++) {
        PRINTF("  \"TEMPER%d_UP\": %.3f,\n", i, temperUp[i]);
        PRINTF("  \"TEMPER%d_LOW\": %.3f,\n", i, temperLow[i]);
    }

    // 打印状态温度
    for (int i = 0; i < 3; i++) {
        PRINTF("  \"TEMPER%d\": %.3f,\n", i, tmp[i]);
    }

    // 打印电压
    for (int i = 0; i < 2; i++) {
        PRINTF("  \"VOLTAGE%d\": %.3f,\n", i, voltage[i]);
    }

    // 打印其他值
    PRINTF("  \"VDD\": %0.3f,\n", vdd);
    PRINTF("  \"RTC\": %lu,\n", ulTimStamp);
    PRINTF("  \"PMBUS\": %d,\n", ucPmbusOk);
    PRINTF("  \"AB\": \"0x%lx\",\n", ulAbLog);
    PRINTF("  \"PWAB\": \"0x%lx\"\n", ulPwrLog);
    PRINTF("}\n");
    PRINTF("end\n");
}

/**
 * @description: 配置获取
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID SHELL_ConfigGet(VOID)
{
    ULONG ulConfig = 0;
    ULONG ulUsrLogin = 0;
    rtc_calendar_t stTim;
    for (size_t i = 0; i < sizeof(gpstStoreParam->aucFunEn); i++) {
        SET_BIT(ulConfig, gpstStoreParam->aucFunEn[i] << i);
    }

    for (size_t i = 0; i < sizeof(gpstStatusParam->aucUsrLogin); i++) {
        SET_BIT(ulUsrLogin, gpstStatusParam->aucUsrLogin[i] << i);
    }

    if (0 == gpstStatusParam->aucUsrTotal) {
        PRINTF("begin\n");
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }


    PCF8563_GetDateTime(&stTim);
    gpstStatusParam->ulTimStamp = Common_ConvertTimstamp(stTim, 1);
    print_json_object(ulUsrLogin, ulConfig, gpstStoreParam->fVoltageUp, gpstStoreParam->fVoltageLow,
                      gpstStoreParam->fTmperUp, gpstStoreParam->fTmperLow, gpstStoreParam->ucTrigger,
                      gpstStatusParam->fTmp, gpstStatusParam->fVoltage, gpstStatusParam->fVdd,
                      gpstStatusParam->ulTimStamp, gpstStatusParam->ucPmbusOk, gpstStatusParam->ulAbLog,
                      gpstStatusParam->ulPwrLog, SUCCESS);

    return;
}

/**
 * @description:
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Shell_LogInit(VOID)
{
    PRINTF("begin\n");

    if (0 == gpstStatusParam->aucUsrTotal) {
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }

    LOG_ReInit();
    SHELL_RetJson(SUCCESS);
    PRINTF("end\n");
    return;
}

VOID Shell_Log(VOID)
{
    PRINTF("begin\n");

    if (0 == gpstStatusParam->aucUsrTotal) {
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }

    LOG_Show();
    PRINTF("end\n");
    return;
}

/**
 * @description: 显示状态
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Shell_Show(VOID)
{
    PRINTF("begin\n");
    if (0 == gpstStatusParam->aucUsrTotal) {
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }

    APP_ShowParam();
    return;
}

/**
 * @description: 显示状态
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Shell_ResetParam(VOID)
{
    PRINTF("begin\n");

    if (0 == gpstStatusParam->aucUsrTotal) {
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }

    SHELL_RetJson(SUCCESS);
    PRINTF("end\n");
    gpstStoreParam->ulFlag = 0x00;
    PARAM_Store();
    PARAM_StoreInit();
    return;
}

/**
 * @description: 设置电压
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Shell_SetTrigger(VOID)
{
    LONG ulTrigger = 0;
    PRINTF("begin\n");

    if (0 == gpstStatusParam->aucUsrTotal) {
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }

    SHELL_ScanParam("set_trigger", &ulTrigger, 1);
    if ((0 == ulTrigger) || (1 == ulTrigger)) {
        gpstStoreParam->ucTrigger = ulTrigger;
        PARAM_Store();
        SHELL_RetJson(SUCCESS);
    } else {
        SHELL_RetJson(ERROR);
    }
    PRINTF("end\n");
    return;
}

/**
 * @description: 设置电压
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Shell_SetVoltage(VOID)
{
    LONG lIndex1 = 0;
    LONG lVoltageLow = 0;
    LONG lVoltageHigh = 0;
    LONG alValue[2];
    PRINTF("begin\n");

    if (0 == gpstStatusParam->aucUsrTotal) {
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }

    SHELL_ScanParam("set_voltage", alValue, 2);
    lVoltageLow = alValue[0];
    lVoltageHigh = alValue[1];

    if (lIndex1 < 0 || lIndex1 > 1) {
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }
    gpstStoreParam->fVoltageLow = lVoltageLow;
    gpstStoreParam->fVoltageUp = lVoltageHigh;
    PARAM_Store();
    SHELL_RetJson(SUCCESS);
    PRINTF("end\n");

    return;
}

/**
 * @description: 设置温度
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Shell_SetTmper(VOID)
{
    LONG lTmperIndex = 0;
    LONG alValue[3];
    LONG lTmperLow;
    LONG lTmperHigh;

    PRINTF("begin\n");

    if (0 == gpstStatusParam->aucUsrTotal) {
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }

    SHELL_ScanParam("set_temper", alValue, 3);
    lTmperIndex = alValue[0];
    lTmperLow = alValue[1];
    lTmperHigh = alValue[2];

    if ((lTmperIndex >= 0) && lTmperIndex <= PARAM_TMPER_NUM) {
        gpstStoreParam->fTmperLow[lTmperIndex] = lTmperLow;
        gpstStoreParam->fTmperUp[lTmperIndex] = lTmperHigh;
        PARAM_Store();
    } else {
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }
    SHELL_RetJson(SUCCESS);
    PRINTF("end\n");

    return;
}

/**
 * @description: 切换image
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Shell_Reset(VOID)
{
    PRINTF("begin\n");

    if (0 == gpstStatusParam->aucUsrTotal) {
        SHELL_RetJson(ERROR);
        PRINTF("end\n");
        return;
    }

    SHELL_RetJson(SUCCESS);
    PRINTF("end\n");
    LL_mDelay(100); // 保证打印完成
    HAL_NVIC_SystemReset();
    return;
}

/**
 * @description: 列出命令信息
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Shell_List(VOID)
{
    for (int i = 0; i < sizeof(gastCmd) / sizeof(cmd_t); i++) {
        PRINTF("%s -->%s\n", gastCmd[i].cmd, gastCmd[i].cmd_mess);
    }

    return;
}

/**
 * @description: 命令初始化
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID CMD_Init(VOID)
{
    /* 初始化fifo */
    fifo_init(&stFifo, aucRx2FifoBuf, FIFO_RX_BUF_SIZE, "rx2 fifo");
    fifo_init(&stFifo1, aucRx1FifoBuf, FIFO_RX1_BUF_SIZE, "rx1 fifo");

    /* 终端提示用户 */
    memcpy(gaucPrompt, PROMPT_USER, sizeof(PROMPT_USER));
    /* 打印默认到终端 */
    PRINTF("%s", gaucPrompt);
}

/**
 * @description: 命令处理
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Cmd_Pro(VOID)
{
    UCHAR ucData;
    STATIC ULONG ulDataLen = 0;

    if (fifo_get(&stFifo, &ucData)) {
        if ((ASCALL_BACK_SPACE == ucData)) {
            if (ulDataLen > 0) {
                ulDataLen--;
                aucData[ulDataLen] = '\0';
                PRINTF(OUTPUT_BACK_SPACE);
            }
            return;
        }

        /* 如果有回车，上层回车是\r */
        if ('\r' == ucData) {
            /* 处理命令 */
            if (0 != ulDataLen) {
                ULONG i = 0;
                char machFlag = 0;
                for (i = 0; i < ARRARY_LENGTH(gastCmd); i++) {
                    if (0 == strncmp((const char *)aucData, gastCmd[i].cmd, strlen(gastCmd[i].cmd))) {
                        PRINTF("\n");
                        gastCmd[i].handler();
                        machFlag = 1;
                        break;
                    }
                }
                if (0 == machFlag && 0 != aucData[0]) {
                    PRINTF("\nerr cmd \n");
                }
            }
            memset(aucData, 0, sizeof(aucData));
            ulDataLen = 0;
            PRINTF("\r\n%s", gaucPrompt);
            return;
        }

        /* 回显,并且保存 */
        PRINTF("%c", ucData);
        aucData[ulDataLen] = ucData;
        ulDataLen++;

        return;
    }
}
#ifdef __cplusplus
}
#endif
