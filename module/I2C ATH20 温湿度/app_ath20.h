/*
 * @Description:
 * @Version: 1.0
 * @Author: lkc
 * @detail:
 */
#ifndef __ATH20_H
#define __ATH20_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ATH20_SLAVE_ADDRESS 0x38 /* I2C从机地址 */
//****************************************
// 定义 AHT20 内部地址
//****************************************
#define INIT                0xBE // 初始化
#define SoftReset           0xBA // 软复位
#define StartTest           0xAC // 开始测试
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint8_t ATH20_Init(void);
uint8_t ATH20_Read_Cal_Enable(void);  // 查询cal enable位有没有使能
void ATH20_Read_CTdata(uint32_t *ct); // 读取AHT10的温度和湿度数据
void I2C_Init(void);
#ifdef __cplusplus
}
#endif
#endif
