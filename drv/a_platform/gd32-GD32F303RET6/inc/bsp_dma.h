/*
 * @Description: 
 * @Version: 2.0
 * @Date: 2023-06-30 20:14:19
 * @LastEditors: lkc
 * @detail: 
 */
#ifndef _BSP_DMA_H
#define _BSP_DMA_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define GPS_DMA                                                         DMA0
#define GPS_DMA_CH                                                      DMA_CH5   
#define GPS_DMA_CLK                                                     RCU_DMA0 

#define L716_DMA                                                        DMA0
#define L716_DMA_CH                                                     DMA_CH2   
#define L716_DMA_CLK                                                    RCU_DMA0 

/* 核心板dma传输 */
#define MB_DMA                                                          DMA1
#define MB_DMA_CH                                                       DMA_CH2   
#define MB_DMA_CLK                                                      RCU_DMA1
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
VOID Bsp_Dma_GpsInit(VOID);
int Bsp_Dma_Init(VOID);
#ifdef __cplusplus
}
#endif
#endif
