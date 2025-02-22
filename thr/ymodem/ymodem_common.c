/**
 ******************************************************************************
 * @file    ymodem_common.c
 * @author
 * @version V1.0.0
 * @date    2022/8/06
 * @brief   ymodem 通用
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "ymodem_includes.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  整数转换字符串
 * @param  str: The string
 * @param  intnum: The integer to be converted
 * @retval None
 */
void Int2Str(uint8_t *str, int32_t intnum)
{
    uint32_t i, Div = 1000000000, j = 0, Status = 0;

    for (i = 0; i < 10; i++) {
        str[j++] = (intnum / Div) + 48;

        intnum = intnum % Div;
        Div /= 10;
        if ((str[j - 1] == '0') & (Status == 0)) {
            j = 0;
        } else {
            Status++;
        }
    }

    return;
}

/**
 * @brief  str转换整数
 * @param  inputstr: The string to be converted
 * @param  intnum: The integer value
 * @retval 1: Correct
 *         0: Error
 */
uint32_t Str2Int(uint8_t *inputstr, int32_t *intnum)
{
    uint32_t i = 0, res = 0;
    uint32_t val = 0;

    if (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X')) {
        if (inputstr[2] == '\0') {
            return 0;
        }
        for (i = 2; i < 11; i++) {
            if (inputstr[i] == '\0') {
                *intnum = val;
                /* return 1; */
                res = 1;
                break;
            }
            if (ISVALIDHEX(inputstr[i])) {
                val = (val << 4) + CONVERTHEX(inputstr[i]);
            } else {
                /* Return 0, Invalid input */
                res = 0;
                break;
            }
        }
        /* Over 8 digit hex --invalid */
        if (i >= 11) {
            res = 0;
        }
    } else /* max 10-digit decimal input */
    {
        for (i = 0; i < 11; i++) {
            if (inputstr[i] == '\0') {
                *intnum = val;
                /* return 1 */
                res = 1;
                break;
            } else if ((inputstr[i] == 'k' || inputstr[i] == 'K') && (i > 0)) {
                val = val << 10;
                *intnum = val;
                res = 1;
                break;
            } else if ((inputstr[i] == 'm' || inputstr[i] == 'M') && (i > 0)) {
                val = val << 20;
                *intnum = val;
                res = 1;
                break;
            } else if (ISVALIDDEC(inputstr[i])) {
                val = val * 10 + CONVERTDEC(inputstr[i]);
            } else {
                /* return 0, Invalid input */
                res = 0;
                break;
            }
        }
        /* Over 10 digit decimal --invalid */
        if (i >= 11) {
            res = 0;
        }
    }

    return res;
}

/**
 * @brief  Get an integer from the HyperTerminal
 * @param  num: The integer
 * @retval 1: Correct
 *         0: Error
 */
uint32_t GetIntegerInput(int32_t *num)
{
    uint8_t inputstr[16];

    while (1) {
        GetInputString(inputstr);
        if (inputstr[0] == '\0') {
            continue;
        }
        if ((inputstr[0] == 'a' || inputstr[0] == 'A') && inputstr[1] == '\0') {
            SerialPutString("User Cancelled \r\n");
            return 0;
        }

        if (Str2Int(inputstr, num) == 0) {
            SerialPutString("Error, Input again: \r\n");
        } else {
            return 1;
        }
    }
}

/**
 * @brief  Get a key from the HyperTerminal
 * @param  None
 * @retval The Key Pressed
 */
uint8_t GetKey(void)
{
    uint8_t key = 0;

    /* Waiting for user input */
    while (1) {
        if (Ymodem_GetChar((uint8_t *)&key)) {
            break;
        }
    }
    return key;
}

/**
 * @brief  发音字符
 * @param  s: The string to be printed
 * @retval None
 */
void Serial_PutString(uint8_t *s)
{
    while (*s != '\0') {
        Ymodem_PutChar(*s);
        s++;
    }
}

/**
 * @brief  获取字符
 * @param  buffP: The input string
 * @retval None
 */
void GetInputString(uint8_t *buffP)
{
    uint32_t bytes_read = 0;
    uint8_t c = 0;
    do {
        c = GetKey();
        /* 如果回车结束\r\n */
        if (c == '\r') {
            break;
        }
        if (c == '\b') /* Backspace */
        {
            if (bytes_read > 0) {
                SerialPutString("\b \b");
                bytes_read--;
            }
            continue;
        }
        /* 输入终端字符太长 */
        if (bytes_read >= CMD_STRING_SIZE) {
            SerialPutString("Command string size overflow\r\n");
            bytes_read = 0;
            continue;
        }
        if (c >= 0x20 && c <= 0x7E) {
            buffP[bytes_read++] = c;
            Ymodem_PutChar(c);
        }
    } while (1);
    SerialPutString(("\n\r"));
    buffP[bytes_read] = '\0';
}

/**
 * @brief  Update CRC16 for input byte
 * @param  CRC input value
 * @param  input byte
 * @retval None
 */
uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte)
{
    uint32_t crc = crcIn;
    uint32_t in = byte | 0x100;

    do {
        crc <<= 1;
        in <<= 1;
        if (in & 0x100) {
            ++crc;
        }
        if (crc & 0x10000) {
            crc ^= 0x1021;
        }
    }

    while (!(in & 0x10000));

    return crc & 0xffffu;
}


/**
 * @brief  Cal CRC16 for YModem Packet
 * @param  data
 * @param  length
 * @retval None
 */
uint16_t Cal_CRC16(const uint8_t *data, uint32_t size)
{
    uint32_t crc = 0;
    const uint8_t *dataEnd = data + size;

    while (data < dataEnd) {
        crc = UpdateCRC16(crc, *data++);
    }

    crc = UpdateCRC16(crc, 0);
    crc = UpdateCRC16(crc, 0);

    return crc & 0xffffu;
}

/**
 * @brief  Cal Check sum for YModem Packet
 * @param  data
 * @param  length
 * @retval None
 */
uint8_t CalChecksum(const uint8_t *data, uint32_t size)
{
    uint32_t sum = 0;
    const uint8_t *dataEnd = data + size;

    while (data < dataEnd) {
        sum += *data++;
    }

    return (sum & 0xffu);
}

#ifdef __cplusplus
};
#endif
