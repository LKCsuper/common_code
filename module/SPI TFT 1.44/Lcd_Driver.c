#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "tft_spi_driver.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 清屏然后复位
 * @detail:
 * @return {*}
 * @author: lkc
 */
void Lcd_Reset(void)
{
#ifdef LCD_RST
    LCD_RST_CLR;
#endif
    delay_ms(100);
#ifdef LCD_RST
    LCD_RST_SET;
#endif
    delay_ms(50);
}

/**
 * @description: 初始化LCD
 * @detail: LCD Init For 1.44Inch LCD Panel with ST7735R.
 * @return {*}
 * @author: lkc
 */
void Lcd_Init(void)
{
    LCD_GPIO_Init(); // 初始化GPIO
    Lcd_Reset();     // 复位 before LCD Init.

    // LCD Init For 1.44Inch LCD Panel with ST7735R.
    Lcd_WriteIndex(0x11); // Sleep exit
    delay_ms(120);

    // ST7735R Frame Rate
    Lcd_WriteIndex(0xB1); // 正常模式 基本的帧率控制参数
    Lcd_WriteData(0x01);  // RTNA
    Lcd_WriteData(0x2C);  // FPA
    Lcd_WriteData(0x2D);  // BAP

    Lcd_WriteIndex(0xB2); // 空闲模式
    Lcd_WriteData(0x01);
    Lcd_WriteData(0x2C);
    Lcd_WriteData(0x2D);

    Lcd_WriteIndex(0xB3); // 局部模式 完全颜色
    Lcd_WriteData(0x01);
    Lcd_WriteData(0x2C);
    Lcd_WriteData(0x2D);
    Lcd_WriteData(0x01);
    Lcd_WriteData(0x2C);
    Lcd_WriteData(0x2D);

    Lcd_WriteIndex(0xB4); // Column inversion 使能列反转 NLA,NLB,NLC Set Inversion
    Lcd_WriteData(0x07);

    // ST7735R Power Sequence 电源序列
    Lcd_WriteIndex(0xC0); // AVDD  VRHP
    Lcd_WriteData(0xA2);  // VRHN
    Lcd_WriteData(0x02);  // mode
    Lcd_WriteData(0x84);

    Lcd_WriteIndex(0xC1); // Power Control Setting
    Lcd_WriteData(0xC5);  // : Set VGH/ VGL Voltage

    Lcd_WriteIndex(0xC2); // In Normal Mode (FullColors)
    Lcd_WriteData(0x0A);  // Operational Amplifier
    Lcd_WriteData(0x00);  // DCA: Adjust the Booster Voltage

    Lcd_WriteIndex(0xC3); // In Idle Mode (8-colors)
    Lcd_WriteData(0x8A);  // APB: Adjust the Operational Amplifier
    Lcd_WriteData(0x2A);  // DCB: Adjust the Booster Voltage

    Lcd_WriteIndex(0xC4); // In Partial Mode + Full colors
    Lcd_WriteData(0x8A);  // APC: Adjust the Operational Amplifier
    Lcd_WriteData(0xEE);  // DCC: Adjust the Booster Circuit for Idle mode

    Lcd_WriteIndex(0xC5); // VCOM Control 1
    Lcd_WriteData(0x0E);  // VCOM Voltage Control

    Lcd_WriteIndex(0x36); // MX, MY, RGB mode 配置显示方向和 RGB 模式Memory Data Access Control
    Lcd_WriteData(0xC8);  // 屏幕的显示方向为镜像水平翻转、镜像垂直翻转，并且采用 RGB 模式

    // ST7735R Gamma Sequence
    Lcd_WriteIndex(0xe0); // 配置显示的伽马曲线
    Lcd_WriteData(0x0f);
    Lcd_WriteData(0x1a);
    Lcd_WriteData(0x0f);
    Lcd_WriteData(0x18);
    Lcd_WriteData(0x2f);
    Lcd_WriteData(0x28);
    Lcd_WriteData(0x20);
    Lcd_WriteData(0x22);
    Lcd_WriteData(0x1f);
    Lcd_WriteData(0x1b);
    Lcd_WriteData(0x23);
    Lcd_WriteData(0x37);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x07);
    Lcd_WriteData(0x02);
    Lcd_WriteData(0x10);

    Lcd_WriteIndex(0xe1); // 配置显示的伽马曲线
    Lcd_WriteData(0x0f);
    Lcd_WriteData(0x1b);
    Lcd_WriteData(0x0f);
    Lcd_WriteData(0x17);
    Lcd_WriteData(0x33);
    Lcd_WriteData(0x2c);
    Lcd_WriteData(0x29);
    Lcd_WriteData(0x2e);
    Lcd_WriteData(0x30);
    Lcd_WriteData(0x30);
    Lcd_WriteData(0x39);
    Lcd_WriteData(0x3f);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x07);
    Lcd_WriteData(0x03);
    Lcd_WriteData(0x10);

    Lcd_WriteIndex(0x2a); // Column Address Set
    Lcd_WriteData(0x00);  // X Address Start:
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00); // X Address End: S≦XE≦X
    Lcd_WriteData(0x7f);

    Lcd_WriteIndex(0x2b);
    Lcd_WriteData(0x00); // Y Address Start: 0≦YS≦Y
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00); // Y Address End:S≦YE≦Y
    Lcd_WriteData(0x9f);

    Lcd_WriteIndex(0xF0); // Enable test command 测试命令
    Lcd_WriteData(0x01);
    Lcd_WriteIndex(0xF6); // Disable ram power save mode 禁用 RAM 电源节能模式。确保显示内容稳定并避免闪烁
    Lcd_WriteData(0x00);

    Lcd_WriteIndex(0x3A); // 65k mode 设置屏幕为 65k 色彩模式
    Lcd_WriteData(0x05);

    Lcd_WriteIndex(0x29); // Display on  启用显示，开始正常显示内容
}

/*************************************************
函数名：LCD_Set_Region
功能：设置lcd显示区域，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
void Lcd_SetRegion(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
    Lcd_WriteIndex(0x2a); // Column Address Set
    Lcd_WriteData(0x00);  // X Address Start:
    Lcd_WriteData(x_start + 2);
    Lcd_WriteData(0x00); // X Address End: S≦XE≦X
    Lcd_WriteData(x_end + 2);

    Lcd_WriteIndex(0x2b);
    Lcd_WriteData(0x00); // Y Address Start:
    Lcd_WriteData(y_start + 3);
    Lcd_WriteData(0x00); // Y Address End: S≦YE≦X
    Lcd_WriteData(y_end + 3);

    Lcd_WriteIndex(0x2c);
}

/*************************************************
函数名：LCD_Set_XY
功能：设置lcd显示起始点
入口参数：xy坐标
返回值：无
*************************************************/
void Lcd_SetXY(uint16_t x, uint16_t y)
{
    Lcd_SetRegion(x, y, x, y);
}

/*************************************************
函数名：LCD_DrawPoint
功能：画一个点
入口参数：无
返回值：无
*************************************************/
void Gui_DrawPoint(uint16_t x, uint16_t y, uint16_t Data)
{
    Lcd_SetRegion(x, y, x + 1, y + 1);
    LCD_WriteData_16Bit(Data);
}

/*****************************************
 函数功能：读TFT某一点的颜色
 出口参数：color  点颜色值
******************************************/
unsigned int Lcd_ReadPoint(uint16_t x, uint16_t y)
{
    unsigned int Data;
    Lcd_SetXY(x, y);

    // Lcd_ReadData();//丢掉无用字节
    // Data=Lcd_ReadData();
    Lcd_WriteData(Data);
    return Data;
}
/*************************************************
函数名：Lcd_Clear
功能：全屏清屏函数
入口参数：填充颜色COLOR
返回值：无
*************************************************/
void Lcd_Clear(uint16_t Color)
{
    unsigned int i, m;
    Lcd_SetRegion(0, 0, X_MAX_PIXEL - 1, Y_MAX_PIXEL - 1);
    Lcd_WriteIndex(0x2C); // Memory Write
    for (i = 0; i < X_MAX_PIXEL; i++) {
        for (m = 0; m < Y_MAX_PIXEL; m++) {
            LCD_WriteData_16Bit(Color);
        }
    }
}

#ifdef __cplusplus
}
#endif
