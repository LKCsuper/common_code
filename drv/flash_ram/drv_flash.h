#ifndef __DRV_FLASH_H__
#define __DRV_FLASH_H__
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
VOID DRV_FlashRead(UCHAR *pucBuf, ULONG ulSrcAddr, ULONG ulBytelen);
VOID DRV_FlashWrite(UCHAR *pucBuf, ULONG ulDstAddr, ULONG ulBytelen);
VOID DRV_FlashErasePage(ULONG ulAddress);
VOID DRV_FlashDownload(VOID);
VOID DRV_FlashEraseData(VOID);
VOID DRC_FlashTest(VOID);
VOID DRV_FLASHWriteWithErase();
#ifdef __cplusplus
}
#endif
#endif
