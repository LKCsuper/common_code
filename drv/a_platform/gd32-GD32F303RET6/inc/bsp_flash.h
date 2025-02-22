#ifndef _BSP_FLASH_H
#define _BSP_FLASH_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
VOID Bsp_Flash_Erase(ULONG ulPageAddr);
VOID Bsp_Flash_WriteBuf(ULONG *ulBuf, ULONG ulBufLen, ULONG ulAdr);
ULONG Bsp_Flash_Read32(ULONG ulAddr);
#ifdef __cplusplus
}
#endif
#endif
