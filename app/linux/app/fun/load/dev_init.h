#ifndef __DEV_INIT_H__
#define __DEV_INIT_H__
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "os_port.h"
/* Private define ------------------------------------------------------------*/
#define ZKSKF_SEM_LOCKKEY  55155
#define ZKSKF_SEM_NOSHARED 55255
#define ZKSKF_SEM_TIMEOUT  60 * 1000
#define COMM_LOCKKEY 15730
/* Private typedef -----------------------------------------------------------*/
// 设备信息结构
typedef struct _ZKSKF_KEY_LOCK_INFO_ {
    uint32_t lockFlag;  // 设备锁定标识，0-未锁定，1-锁定
    void * handle; // 设备句柄
#ifdef ZAYK_KEYU_WIN
    long startTime; // 锁定起始时间
#else
    struct timeval startTime; // 锁定起始时间
#endif
    uint32_t lockTime; // 锁定超时时间，单位为毫秒，为0XFFFFFFFF时无限等待
} ZKSKF_KEY_LOCK_INFO;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern  SEM_HANDLE g_semHandle;
extern SEM_HANDLE g_lockSemHandle;
extern SEM_HANDLE g_logSemHandle;
extern ZKSKF_KEY_LOCK_INFO lockInfo;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif