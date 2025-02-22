#ifndef __APP_RLE_H__
#define __APP_RLE_H__
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
int RLE_Decode(const unsigned char *inbuf, int inSize, unsigned char *outbuf, int onuBufSize, int *outsize);
int RLE_GetAddrData(uint32_t read_size, uint32_t need_size, unsigned char *out);
#ifdef __cplusplus
}
#endif
#endif
