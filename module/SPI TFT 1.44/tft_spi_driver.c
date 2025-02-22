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
#include "Lcd_Driver.h"
#include "tft_spi_driver.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 液晶IO初始化配置
 * @detail:
 * @return {*}
 * @author: lkc
 */
void LCD_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

#ifdef LCD_RST
    GPIO_InitStructure.GPIO_Pin = LCD_LED | LCD_RS | LCD_CS | LCD_SCL | LCD_SDA | LCD_RST;
#else
    GPIO_InitStructure.GPIO_Pin = LCD_LED | LCD_RS | LCD_CS | LCD_SCL | LCD_SDA;
#endif

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    return;
}

/**
 * @description: 向SPI总线传输一个8位数据
 * @detail:
 * @param {uint8_t} Data
 * @return {*}
 * @author: lkc
 */
void SPI_WriteData(uint8_t Data)
{
    unsigned char i = 0;
    for (i = 8; i > 0; i--) {
        if (Data & 0x80) {
            LCD_SDA_SET; // 输出数据
        } else {
            LCD_SDA_CLR;
        }

        LCD_SCL_CLR;
        LCD_SCL_SET;
        Data <<= 1;
    }
}

/**
 * @description: 向液晶屏写一个8位指令
 * @detail:
 * @param {uint8_t} Index
 * @return {*}
 * @author: lkc
 */
void Lcd_WriteIndex(uint8_t Index)
{
    // SPI 写命令时序开始
    LCD_CS_CLR;
    LCD_RS_CLR;
    SPI_WriteData(Index);
    LCD_CS_SET;
}

/**
 * @description: 向液晶屏写一个8位数据
 * @detail:
 * @param {uint8_t} Data
 * @return {*}
 * @author: lkc
 */
void Lcd_WriteData(uint8_t Data)
{
    LCD_CS_CLR;
    LCD_RS_SET;
    SPI_WriteData(Data);
    LCD_CS_SET;
}

/**
 * @description: 向液晶屏写一个16位数据
 * @detail:
 * @param {uint16_t} Data
 * @return {*}
 * @author: lkc
 */
void LCD_WriteData_16Bit(uint16_t Data)
{
    LCD_CS_CLR;
    LCD_RS_SET;
    SPI_WriteData(Data >> 8); // 写入高8位数据
    SPI_WriteData(Data);      // 写入低8位数据
    LCD_CS_SET;
}

/**
 * @description: 写寄存器
 * @detail:
 * @param {uint8_t} Index
 * @param {uint8_t} Data
 * @return {*}
 * @author: lkc
 */
void Lcd_WriteReg(uint8_t Index, uint8_t Data)
{
    Lcd_WriteIndex(Index);
    Lcd_WriteData(Data);
}
#ifdef __cplusplus
}
#endif