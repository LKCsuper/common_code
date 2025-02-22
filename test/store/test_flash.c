#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "printf_color.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 通用flash测试
 * @detail: 
 * @return {*}
 * @author: lkc
 */
VOID TEST_Flash(VOID)
{
    UCHAR i;
    UCHAR aucWriteBuf[256] = {0};
    UCHAR aucReadBuf[256] = {0};
    for (i = 0; i <= 255; i++) // 填充缓冲
    {
        aucWriteBuf[i] = i;
    }

    PRINT_HEX("Write Buf\n", aucWriteBuf, 256);

    // 字节写入方式
    // ! 写入256数据

    INFO("写结束");

    // ! 读出256数据

    // 校验数据
    for (i = 0; i < 256; i++) {
        if (aucReadBuf[i] != aucWriteBuf[i]) {
            INFO("0x%02X ", aucReadBuf[i]);
            INFO("错误:I2C EEPROM写入与读出的数据不一致");
            return 0;
        }
    }
    PRINT_HEX("Read Buf\n", aucReadBuf, 256);

    INFO("读写测试成功");
    return;
}
#ifdef __cplusplus
}
#endif