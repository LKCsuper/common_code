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
#define ATH20_SLAVE_ADDRESS 0x38 /* I2C�ӻ���ַ */
//****************************************
// ���� AHT20 �ڲ���ַ
//****************************************
#define INIT                0xBE // ��ʼ��
#define SoftReset           0xBA // ��λ
#define StartTest           0xAC // ��ʼ����
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint8_t ATH20_Init(void);
uint8_t ATH20_Read_Cal_Enable(void);  // ��ѯcal enableλ��û��ʹ��
void ATH20_Read_CTdata(uint32_t *ct); // ��ȡAHT10���¶Ⱥ�ʪ������
void I2C_Init(void);
#ifdef __cplusplus
}
#endif
#endif
