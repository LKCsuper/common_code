/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-11-10 12:01:47
 * @LastEditors: lkc
 * @LastEditTime: 2023-11-17 09:35:10
 * @detail: 
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: �豸����
 * @detail: 
 * @return {*}
 * @author: lkc
 */
void BSP_MISC_Reset(void)
{
    ERR("Reset\r\n");
    OS_DELAY(100);

    /* �ر������ж� */
    __set_FAULTMASK(1);
    /* ������� */
    NVIC_SystemReset();
}

/**
 * @description: ��ȡоƬid
 * @detail: 
 * @return {*}
 * @author: lkc
 */
void BSP_MISC_GetChipID(void)
{  
    uint32_t temp_id;
    
    gpstParamState->stChipId.origin_ID[0] = *(__I uint32_t *)(0x1FFF7A10 + 0x00);
    gpstParamState->stChipId.origin_ID[1] = *(__I uint32_t *)(0x1FFF7A10 + 0x04);
    gpstParamState->stChipId.origin_ID[2] = *(__I uint32_t *)(0x1FFF7A10 + 0x08); 

    /* �������һ�����㣬��12�ֽڴ����2�ֽ� ����оƬsimpe_ID��ͬ�ĸ����� 1/65535 */  
    temp_id = gpstParamState->stChipId.origin_ID[0] + gpstParamState->stChipId.origin_ID[1] + gpstParamState->stChipId.origin_ID[2];
    
    temp_id = (temp_id>>16) + (temp_id &0x0000FFFF);
    
    gpstParamState->stChipId.simple_ID = (uint16_t)temp_id;  
}
#ifdef __cplusplus
}
#endif




