/*
 * @Description: 
 * @Version: 2.0
 * @Date: 2023-07-17 11:33:18
 * @LastEditors: lkc
 * @detail: 
 */
#ifndef _COMMON_MODULE_H
#define _COMMON_MODULE_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
typedef int (*init_fn_t)(void);
/* Private typedef -----------------------------------------------------------*/
// #ifdef _MSC_VER /* we do not support MS VC++ compiler */
//     #define MODULE_EXPORT(fn, level)
// #else
#if MODULE_DEBUG_INIT
		struct rt_module_desc
		{
				const char* fn_name;
				const init_fn_t fn;
		};
		#define MODULE_EXPORT(fn, level)                                                       \
				const char __rti_##fn##_name[] = #fn;                                            \
				RT_USED const struct rt_module_desc __rt_module_desc_##fn SECTION(".rti_module_fn." level) = \
				{ __rti_##fn##_name, fn};
#else
		#define MODULE_EXPORT(fn, level)                                                       \
				RT_USED const init_fn_t __rt_module_##fn SECTION(".rti_module_fn." level) = fn
#endif
//#endif
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void MODULE_Components_Board_Init(void);
#ifdef __cplusplus
}
#endif
#endif
