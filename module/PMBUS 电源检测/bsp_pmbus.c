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
#define PWR1_READ_ADDR  0XB0
#define PWR1_WRITE_ADDR 0XB1
#define PWR2_READ_ADDR  0XB2
#define PWR2_WRITE_ADDR 0XB3
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SHORT currpage;
STATIC FLOAT afVoltage[2][FILTER_COUNT];
STATIC AVE_RANGE_FILTER stVoFilter[VOLTAGE_NUM];
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
float pmbus_linear2float(uint16_t data)
{
    float val = 0;
    int8_t exponent;
    int16_t mantissa;

    exponent = data >> 11;
    // mantissa = ((int16_t)((data & 0x07ff) << 5)) >> 5;
    mantissa = data & 0x07ff;

    if (exponent > 0x0f) {
        exponent |= 0xe0;
    }
    if (mantissa > 0x03ff) {
        mantissa |= 0xf800;
    }

    val = mantissa;
    if (exponent >= 0) {
        val *= (1 << exponent);
    } else {
        val /= (1 << -exponent);
    }

    return (float)val;
}

UCHAR PMBUS_ReadByte(UCHAR ucAddr, UCHAR ucCmd)
{
    UCHAR ucPEC;
    UCHAR ucData;

    UNUSED(ucPEC);

    I2C_Start(&gstPmbusI2c);
    I2C_SendByte(&gstPmbusI2c, ucAddr | 0);
    if (I2C_WaitAck(&gstPmbusI2c)) {
        ERR("WAIT ACK Time Out\n");
        return 0;
    }

    I2C_SendByte(&gstPmbusI2c, ucCmd);
    if (I2C_WaitAck(&gstPmbusI2c)) {
        ERR("WAIT ACK Time Out\n");
        return 0;
    }

    I2C_Start(&gstPmbusI2c);

    I2C_SendByte(&gstPmbusI2c, ucAddr | 1);
    if (I2C_WaitAck(&gstPmbusI2c)) {
        ERR("WAIT ACK Time Out\n");
        return 0;
    }

    ucData = I2C_ReadByte(&gstPmbusI2c, 1);
    ucPEC = I2C_ReadByte(&gstPmbusI2c, 1);
    I2C_Stop(&gstPmbusI2c);
    return ucData;
}

void PMBUS_WriteByte(UCHAR ucAddr, UCHAR ucCmd, UCHAR ucData)
{
    I2C_Start(&gstPmbusI2c);
    I2C_SendByte(&gstPmbusI2c, ucAddr | 0);
    if (I2C_WaitAck(&gstPmbusI2c)) {
        ERR("WAIT ACK Time Out\n");
        return;
    }

    I2C_SendByte(&gstPmbusI2c, ucCmd);
    if (I2C_WaitAck(&gstPmbusI2c)) {
        ERR("WAIT ACK Time Out\n");
        return;
    }

    I2C_SendByte(&gstPmbusI2c, ucData);
    if (I2C_WaitAck(&gstPmbusI2c)) {
        ERR("WAIT ACK Time Out\n");
        return;
    }
    I2C_Stop(&gstPmbusI2c);

    return;
}

void PMBUS_ReadWord(UCHAR ucAddr, UCHAR ucCmd, USHORT *pulData)
{
    UCHAR *pucData = (UCHAR *)pulData;
    UCHAR ucPEC;
    UNUSED(ucPEC);
    int i = 0;
    I2C_Start(&gstPmbusI2c);
    I2C_SendByte(&gstPmbusI2c, ucAddr | 0);
    if (I2C_WaitAck(&gstPmbusI2c)) {
        // ERR("WAIT ACK Time Out\n");
        gpstStatusParam->ucPmbusOk = FALSE;
        return;
    }

    I2C_SendByte(&gstPmbusI2c, ucCmd);
    if (I2C_WaitAck(&gstPmbusI2c)) {
        // ERR("WAIT ACK Time Out\n");
        gpstStatusParam->ucPmbusOk = FALSE;
        return;
    }

    I2C_Start(&gstPmbusI2c);
    I2C_SendByte(&gstPmbusI2c, ucAddr | 1);
    if (I2C_WaitAck(&gstPmbusI2c)) {
        // ERR("WAIT ACK Time Out\n");
        gpstStatusParam->ucPmbusOk = FALSE;
        return;
    }

    for (i = 0; i < 2; i++) {
        pucData[i] = I2C_ReadByte(&gstPmbusI2c, 1);
    }

    ucPEC = I2C_ReadByte(&gstPmbusI2c, 1);
    I2C_Stop(&gstPmbusI2c);
    gpstStatusParam->ucPmbusOk = TRUE;
    return;
}

void PMBUS_WriteWord(UCHAR ucAddr, UCHAR ucCmd, ULONG ulData)
{
    UCHAR *pucData = (UCHAR *)ulData;
    int i = 0;
    I2C_Start(&gstPmbusI2c);
    I2C_SendByte(&gstPmbusI2c, ucAddr | 0);
    if (I2C_WaitAck(&gstPmbusI2c)) {
        ERR("WAIT ACK Time Out\n");
        return;
    }

    I2C_SendByte(&gstPmbusI2c, ucCmd);
    if (I2C_WaitAck(&gstPmbusI2c)) {
        ERR("WAIT ACK Time Out\n");
        return;
    }

    for (i = 0; i < 2; i++) {
        I2C_SendByte(&gstPmbusI2c, pucData[i]);
        if (I2C_WaitAck(&gstPmbusI2c)) {
            ERR("WAIT ACK Time Out\n");
            return;
        }
    }
    I2C_Stop(&gstPmbusI2c);

    return;
}

void PMBUS_ReadBlock(UCHAR ucAddr, UCHAR ucCmd, UCHAR *pucData, UCHAR *pucCount)
{
    int i = 0;
    UCHAR ucPEC;
    UNUSED(ucPEC);
    UCHAR ucCount;
    I2C_Start(&gstPmbusI2c);
    I2C_SendByte(&gstPmbusI2c, ucAddr | 0);
    if (ERROR == I2C_WaitAck(&gstPmbusI2c)) {
        ERR("WAIT ACK Time Out\n");
        return;
    }

    I2C_SendByte(&gstPmbusI2c, ucCmd);
    if (ERROR == I2C_WaitAck(&gstPmbusI2c)) {
        ERR("WAIT ACK Time Out\n");
        return;
    }

    I2C_Start(&gstPmbusI2c);

    I2C_SendByte(&gstPmbusI2c, ucAddr | 1);
    if (ERROR == I2C_WaitAck(&gstPmbusI2c)) {
        ERR("WAIT ACK Time Out\n");
        return;
    }

    ucCount = I2C_ReadByte(&gstPmbusI2c, 1);

    for (i = 0; i < ucCount; i++) {
        pucData[i] = I2C_ReadByte(&gstPmbusI2c, 1);
    }
    ucPEC = I2C_ReadByte(&gstPmbusI2c, 1);
    I2C_Stop(&gstPmbusI2c);

    *pucCount = ucCount;

    return;
}

void PMBUS_WriteBlock(UCHAR ucAddr, UCHAR ucCmd, UCHAR *pucData, UCHAR ucCount)
{
    int i = 0;
    I2C_Start(&gstPmbusI2c);
    I2C_SendByte(&gstPmbusI2c, ucAddr | 0);
    if (I2C_WaitAck(&gstPmbusI2c)) {
        ERR("WAIT ACK Time Out\n");
        return;
    }

    I2C_SendByte(&gstPmbusI2c, ucCmd);
    if (I2C_WaitAck(&gstPmbusI2c)) {
        ERR("WAIT ACK Time Out\n");
        return;
    }

    I2C_SendByte(&gstPmbusI2c, ucCount);
    if (I2C_WaitAck(&gstPmbusI2c)) {
        ERR("WAIT ACK Time Out\n");
        return;
    }

    for (i = 0; i < ucCount; i++) {
        I2C_SendByte(&gstPmbusI2c, pucData[i]);
        if (I2C_WaitAck(&gstPmbusI2c)) {
            ERR("WAIT ACK Time Out\n");
            return;
        }
    }
    I2C_Stop(&gstPmbusI2c);

    return;
}

UCHAR aucPmbusAddr[] = {0xB0, 0XB2};
/**
 * @description:
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID PMBUS_Init(VOID)
{
    UCHAR ucRet = SUCCESS;

    /* 初始化I2C */
    I2C_Init();

    /* 检测设备是否存在 */
    for (size_t i = 0; i < ARRARY_LENGTH(aucPmbusAddr); i++) {
        ucRet = I2C_CheckDevice(&gstPmbusI2c, aucPmbusAddr[i]);
        if (SUCCESS != ucRet) {
            ERR("NO PWR I2C DEVICE %x\n", aucPmbusAddr[i]);
            gpstStatusParam->ucPmbusOk = FALSE;
            return;
        }
    }

    stVoFilter[0].pStoreArr = afVoltage[0];
    stVoFilter[1].pStoreArr = afVoltage[1];

    for (size_t i = 0; i < VOLTAGE_NUM; i++) {
        stVoFilter[i].fRange = FILTER_RANGE;
        stVoFilter[i].ulTotalNum = FILTER_COUNT;
    }

    return;
}

void i2c_write_byte(uint8_t address, uint8_t data)
{
    I2C_Start(&gstPmbusI2c);
    I2C_SendByte(&gstPmbusI2c, data);
    I2C_Stop(&gstPmbusI2c);

    return;
}

void i2c_write_byte_data(uint8_t address, uint8_t reg, uint8_t value)
{
    I2C_Start(&gstPmbusI2c);
    I2C_SendByte(&gstPmbusI2c, reg);
    I2C_SendByte(&gstPmbusI2c, value);
    I2C_Stop(&gstPmbusI2c);

    return;
}


void i2c_write_word_data(uint8_t address, uint8_t reg, uint16_t value)
{
    I2C_Start(&gstPmbusI2c);
    I2C_SendByte(&gstPmbusI2c, reg);
    I2C_SendByte(&gstPmbusI2c, 0xFF & value);
    I2C_SendByte(&gstPmbusI2c, value >> 8);
    I2C_Stop(&gstPmbusI2c);

    return;
}

int16_t i2c_read_byte_data(uint8_t address, uint8_t reg)
{
    int16_t value;
    I2C_Start(&gstPmbusI2c);
    I2C_SendByte(&gstPmbusI2c, (address) | 0);
    I2C_WaitAck(&gstPmbusI2c);
    I2C_SendByte(&gstPmbusI2c, reg);
    I2C_WaitAck(&gstPmbusI2c);
    I2C_Start(&gstPmbusI2c);
    I2C_SendByte(&gstPmbusI2c, (address) | 1);
    I2C_WaitAck(&gstPmbusI2c);
    value = I2C_ReadByte(&gstPmbusI2c, 0);
    I2C_Stop(&gstPmbusI2c);

    return value;
}

int32_t i2c_read_word_data(uint8_t address, uint8_t reg)
{

    int temp = -1;
    int32_t value = -1;

    I2C_Start(&gstPmbusI2c);
    I2C_SendByte(&gstPmbusI2c, reg);
    I2C_Start(&gstPmbusI2c);

    value = I2C_ReadByte(&gstPmbusI2c, 1);
    temp = I2C_ReadByte(&gstPmbusI2c, 0);
    I2C_Stop(&gstPmbusI2c);
    if (temp == -1 || value == -1) {
        return -1;
    }
    value |= temp << 8;

    return value;
}

void read_block(uint8_t address, uint8_t reg, int bytes, uint8_t *buffer)
{
    int i = 0;
    I2C_Start(&gstPmbusI2c);
    I2C_SendByte(&gstPmbusI2c, reg);
    I2C_Start(&gstPmbusI2c);

    for (i = 0; i < bytes; i++) {
        buffer[i] = I2C_ReadByte(&gstPmbusI2c, i < bytes - 1);
    }

    return;
}

int i2c_read_string(uint8_t address, uint8_t reg, int bytes, uint8_t *buffer)
{
    int len = 0;

    len = (int)i2c_read_byte_data(address, reg);
    PRINTF_GREEN("len %d \n", len);

    if (0 == len) {
        return len;
    }

    if ((len) && (len < bytes)) {
        read_block(address, reg, len, buffer);
    }
    PRINT_HEX("hex ", buffer, 21);
    return len;
}

void PMBUS_SendByte(UCHAR ucAddr, UCHAR ucData)
{
    I2C_Start(&gstPmbusI2c);
    I2C_SendByte(&gstPmbusI2c, ucAddr | 0);
    if (I2C_WaitAck(&gstPmbusI2c)) {
        ERR("WAIT ACK Time Out\n");
        return;
    }

    I2C_SendByte(&gstPmbusI2c, ucData);
    if (I2C_WaitAck(&gstPmbusI2c)) {
        ERR("WAIT ACK Time Out\n");
        return;
    }
    I2C_Stop(&gstPmbusI2c);

    return;
}

#define PMBUS_PAGE 0x00
int pmbus_set_page(uint8_t page)
{
    int rv = SUCCESS;
    uint8_t newpage;

    if (page != currpage) {
        PMBUS_WriteByte(I2C_WRITE_ADDR, PMBUS_PAGE, page);

        newpage = PMBUS_ReadByte(I2C_READ_ADDR, PMBUS_PAGE);
        if (newpage != page) {
            return FALSE;
        } else {
            currpage = page;
        }
    }
    return rv;
}

/**
 * @description: 获取电压
 * @detail:
 * @param {UCHAR} ucPowerIndex
 * @return {*}
 * @author: lkc
 */
FLOAT PMBUS_GetVoltage(UCHAR ucPowerIndex)
{
    UCHAR ucAddr = 0;
    USHORT sWord;
    if (0 == ucPowerIndex) {
        ucAddr = PWR1_READ_ADDR;
    } else if (1 == ucPowerIndex) {
        ucAddr = PWR2_READ_ADDR;
    }

    /* 读取电压尝试 */
    PMBUS_ReadWord(ucAddr, 0x88, &sWord);

    return pmbus_linear2float(sWord);
}

/**
 * @description: 获取型号
 * @detail:
 * @param {UCHAR} ucPowerIndex
 * @param {UCHAR} *pucModel
 * @param {UCHAR} *pucCount
 * @return {*}
 * @author: lkc
 */
VOID PMBUS_GetModel(UCHAR ucPowerIndex, UCHAR *pucModel, UCHAR *pucCount)
{
    UCHAR ucAddr = 0;
    if (0 == ucPowerIndex) {
        ucAddr = PWR1_READ_ADDR;
    } else if (1 == ucPowerIndex) {
        ucAddr = PWR2_READ_ADDR;
    }

    /* 读取电压尝试 */
    PMBUS_ReadBlock(ucAddr, 0x99, pucModel, pucCount);

    return;
}

/**
 * @description: PMBUS 电源检测
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID PMBUS_Detect(VOID)
{
#if DETECT_VOLTAGE
    FLOAT fSumVoltage[2] = {0};

    if ((TRUE == gpstStatusParam->ucPwrBattery)) { // 如果当前是电池供电 直接返回
        gpstStatusParam->aucAbnormalStatus[AB_VOLTAGE1] = FALSE;
        gpstStatusParam->aucAbnormalStatus[AB_VOLTAGE2] = FALSE;
        return;
    }

    // 获取电压
    fSumVoltage[0] = PMBUS_GetVoltage(0);
    fSumVoltage[1] = PMBUS_GetVoltage(1);

    // pmbus设备是否连接
    if (FALSE == gpstStatusParam->ucPmbusOk) {
        gpstStatusParam->aucAbnormalStatus[AB_VOLTAGE1] = FALSE;
        gpstStatusParam->aucAbnormalStatus[AB_VOLTAGE2] = FALSE;
#if PWR_RECOVER
        gpstStatusParam->aucPwrAbnormalStatus[AB_VOLTAGE1] = FALSE;
        gpstStatusParam->aucPwrAbnormalStatus[AB_VOLTAGE2] = FALSE;
#endif

        gpstStatusParam->fVoltage[0] = 0;
        gpstStatusParam->fVoltage[1] = 0;
        return;
    }

    if (TRUE == COMMON_AveRangeFilter(&stVoFilter[0], fSumVoltage[0], &gpstStatusParam->fVoltage[0])) {
        if ((gpstStatusParam->fVoltage[0] >= gpstStoreParam->fVoltageUp ||
             gpstStatusParam->fVoltage[0] <= gpstStoreParam->fVoltageLow) &&
            gpstStatusParam->fVoltage[0] != 0) {
            gpstStatusParam->aucAbnormalStatus[AB_VOLTAGE1] = TRUE;
            gpstStatusParam->aucPwrAbnormalStatus[AB_VOLTAGE1] = TRUE;
        } else {
            gpstStatusParam->aucAbnormalStatus[AB_VOLTAGE1] = FALSE;
#if PWR_RECOVER
            gpstStatusParam->aucPwrAbnormalStatus[AB_VOLTAGE1] = FALSE;
#endif
        }
    }

    if (TRUE == COMMON_AveRangeFilter(&stVoFilter[1], fSumVoltage[1], &gpstStatusParam->fVoltage[1])) {
        if ((gpstStatusParam->fVoltage[1] >= gpstStoreParam->fVoltageUp ||
             gpstStatusParam->fVoltage[1] <= gpstStoreParam->fVoltageLow) &&
            gpstStatusParam->fVoltage[1] != 0) {
            gpstStatusParam->aucAbnormalStatus[AB_VOLTAGE2] = TRUE;
            gpstStatusParam->aucPwrAbnormalStatus[AB_VOLTAGE2] = TRUE;
        } else {
            gpstStatusParam->aucAbnormalStatus[AB_VOLTAGE2] = FALSE;
#if PWR_RECOVER
            gpstStatusParam->aucPwrAbnormalStatus[AB_VOLTAGE2] = FALSE;
#endif
        }
    }
#endif
    return;
}
#ifdef __cplusplus
}
#endif
