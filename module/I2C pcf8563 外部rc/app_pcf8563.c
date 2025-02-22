/*
 * PCF8563.c
 *
 *	The MIT License.
 *  Created on: 5.09.2019
 *      Author: Mateusz Salamon
 *		Contact: mateusz@msalamon.pl
 *
 *      Website: https://msalamon.pl/dalsze-zmagania-z-rtc-ds1307-i-pcf8563-na-stm32/
 *      GitHub: https://github.com/lamik/PCF8563_RTC_STM32_HAL
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
I2C_TYPE *hi2c_pcf8563;
uint8_t Pcf8563Buffer[7];
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void PCF8563_TEST1Enable(uint8_t Enable)
{
    WriteBitToControlRegister(hi2c_pcf8563, PCF8563_REG_CONTROL_STATUS1, PCF8563_CONTROL1_TEST1, Enable);
}

void PCF8563_STOPEnable(uint8_t Enable)
{
    WriteBitToControlRegister(hi2c_pcf8563, PCF8563_REG_CONTROL_STATUS1, PCF8563_CONTROL1_STOP, Enable);
}

void PCF8563_TESTCEnable(uint8_t Enable)
{
    WriteBitToControlRegister(hi2c_pcf8563, PCF8563_REG_CONTROL_STATUS1, PCF8563_CONTROL1_TESTC, Enable);
}

void PCF8563_InterruptEnable(uint8_t Enable)
{
    WriteBitToControlRegister(hi2c_pcf8563, PCF8563_REG_CONTROL_STATUS2, PCF8563_CONTROL2_TI_TP, Enable);
}

void PCF8563_AlarmFlagEnable(uint8_t Enable)
{
    WriteBitToControlRegister(hi2c_pcf8563, PCF8563_REG_CONTROL_STATUS2, PCF8563_CONTROL2_AF, Enable);
}

void PCF8563_TimerFlagEnable(uint8_t Enable)
{
    WriteBitToControlRegister(hi2c_pcf8563, PCF8563_REG_CONTROL_STATUS2, PCF8563_CONTROL2_TF, Enable);
}

void PCF8563_AlarmInterruptEnable(uint8_t Enable)
{
    WriteBitToControlRegister(hi2c_pcf8563, PCF8563_REG_CONTROL_STATUS2, PCF8563_CONTROL2_AIE, Enable);
}

void PCF8563_TimerInterruptEnable(uint8_t Enable)
{
    WriteBitToControlRegister(hi2c_pcf8563, PCF8563_REG_CONTROL_STATUS2, PCF8563_CONTROL2_TIE, Enable);
}

void PCF8563_ClkoutFrequency(CLKOUT_Freq Frequency)
{
    uint8_t tmp;

    if (Frequency > 3) {
        Frequency = (CLKOUT_Freq)3;
    }

    SOFT_I2C_Mem_Read(hi2c_pcf8563, PCF8563_ADDRESS, PCF8563_REG_CLKOUT, 1, &tmp, 1, PCF8563_I2C_TIMEOUT);
    tmp &= ~(3 << PCF8563_CLKOUT_CONTROL_FD0);
    tmp |= (Frequency << PCF8563_CLKOUT_CONTROL_FD0);
    SOFT_I2C_Mem_Write(hi2c_pcf8563, PCF8563_ADDRESS, PCF8563_REG_CLKOUT, 1, &tmp, 1, PCF8563_I2C_TIMEOUT);
}

uint8_t bcd2dec(uint8_t BCD)
{
    return (((BCD & 0xF0) >> 4) * 10) + (BCD & 0xF);
}

uint8_t dec2bcd(uint8_t DEC)
{
    return ((DEC / 10) << 4) + (DEC % 10);
}

int dayofweek(int Day, int Month, int Year)
{
    int Y, C, M, N, D;
    M = 1 + (9 + Month) % 12;
    Y = Year - (M > 10);
    C = Y / 100;
    D = Y % 100;
    N = ((13 * M - 1) / 5 + D + D / 4 + 6 * C + Day + 5) % 7;
    return (7 + N) % 7;
}

void PCF8563_SetDateTime(rtc_calendar_t *DateTime)
{
    uint8_t tmp[7];

    if (DateTime->second > 59) {
        DateTime->second = 59;
    }
    if (DateTime->minute > 59) {
        DateTime->minute = 59;
    }
    if (DateTime->hour > 23) {
        DateTime->hour = 23;
    }
    if (DateTime->day > 31) {
        DateTime->day = 31;
    }
    if (DateTime->month > 12) {
        DateTime->month = 12;
    }
    if (DateTime->year > 2099) {
        DateTime->year = 2099;
    }

    tmp[0] = dec2bcd((DateTime->second) & 0x7F);
    tmp[1] = dec2bcd(DateTime->minute);
    tmp[2] = dec2bcd(DateTime->hour);
    tmp[3] = dec2bcd(DateTime->day);
    tmp[4] = dayofweek(DateTime->day, DateTime->month, DateTime->year);
    tmp[5] = dec2bcd(DateTime->month);
    tmp[6] = dec2bcd(DateTime->year - 2000);

    SOFT_I2C_Mem_Write(hi2c_pcf8563, PCF8563_ADDRESS, PCF8563_REG_TIME, 1, tmp, 7, PCF8563_I2C_TIMEOUT);
}

void PCF8563_CalculateDateTime(rtc_calendar_t *DateTime)
{
    DateTime->second = bcd2dec((Pcf8563Buffer[0]) & 0x7F);
    DateTime->minute = bcd2dec(Pcf8563Buffer[1]);
    DateTime->hour = bcd2dec(Pcf8563Buffer[2]);
    DateTime->day = bcd2dec(Pcf8563Buffer[3]);
    DateTime->week = bcd2dec(Pcf8563Buffer[4] + 1); // too keep weekdays in 1-7 format
    DateTime->month = bcd2dec(Pcf8563Buffer[5] & 0x1F);
    DateTime->year = 2000 + bcd2dec(Pcf8563Buffer[6]);
}

#ifdef PCF8563_USE_DMA
void PCF8563_ReceiveDateTimeDMA(void)
{
    HAL_I2C_Mem_Read_DMA(hi2c_pcf8563, PCF8563_ADDRESS, PCF8563_REG_TIME, 1, Pcf8563Buffer, 7);
}
#else
void PCF8563_GetDateTime(rtc_calendar_t *DateTime)
{
    SOFT_I2C_Mem_Read(hi2c_pcf8563, PCF8563_ADDRESS, PCF8563_REG_TIME, 1, Pcf8563Buffer, 7, PCF8563_I2C_TIMEOUT);

    PCF8563_CalculateDateTime(DateTime);
}
#endif

void PCF8563_Init(I2C_TYPE *i2c)
{
    rtc_calendar_t stTim;
    UCHAR value = 0x00;
    hi2c_pcf8563 = (I2C_TYPE *)i2c;
    I2C2_Init();
    SOFT_I2C_Mem_Write(hi2c_pcf8563, PCF8563_ADDRESS, PCF8563_REG_CONTROL_STATUS1, 1, &value, 1, PCF8563_I2C_TIMEOUT);
    SOFT_I2C_Mem_Write(hi2c_pcf8563, PCF8563_ADDRESS, PCF8563_REG_CONTROL_STATUS2, 1, &value, 1, PCF8563_I2C_TIMEOUT);
    SOFT_I2C_Mem_Write(hi2c_pcf8563, PCF8563_ADDRESS, PCF8563_REG_CLKOUT, 1, &value, 1, PCF8563_I2C_TIMEOUT);

    PCF8563_GetDateTime(&stTim);
    gpstStatusParam->ulTimStamp = Common_ConvertTimstamp(stTim, 1);
    
    return;
}
#ifdef __cplusplus
}
#endif
