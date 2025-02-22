/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @detail: 
 */
#ifndef __TFT_SPI_H__
#define __TFT_SPI_H__
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
void LCD_GPIO_Init(void);
void SPI_WriteData(uint8_t Data);
void Lcd_WriteIndex(uint8_t Index);
void Lcd_WriteData(uint8_t Data);
void LCD_WriteData_16Bit(uint16_t Data);
void Lcd_WriteReg(uint8_t Index, uint8_t Data);
#ifdef __cplusplus
}
#endif
#endif