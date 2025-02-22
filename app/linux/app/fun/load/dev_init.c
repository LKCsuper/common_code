/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @detail: 
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#ifdef ZAYK_KEYU_WIN
#include <windows.h>
#endif

#include "dev_init.h"
#include "Zayk_KeyU_log.h"
/* Private define ------------------------------------------------------------*/
#ifndef GIT_VERSION
#define GIT_VERSION "v1.0.0-1234"
#endif
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SEM_HANDLE g_semHandle;
SEM_HANDLE g_lockSemHandle;
SEM_HANDLE g_logSemHandle;
ZKSKF_KEY_LOCK_INFO lockInfo;
static void *ctx_mutex = NULL;
int semHandle;
int semDevHandle;
int g_lockFlag;
int g_semExit;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#if defined ZAYK_KEYU_WIN
BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, PVOID fImpLoad)
{
    int rv = 0;
    char semName[256] = {0};
    char semNoSharedName[256] = {0};

    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            // 日志信号量
            sprintf_s(semName, sizeof(semName), "%d", ZKSKF_SEM_LOCKKEY + 2);
            rv = ZAYK_SEM_Create(semName, 1, &g_logSemHandle);
            LOGI("SDK VERSION: " GIT_VERSION);

            //创建信号量
            sprintf_s(semName, sizeof(semName), "%d", ZKSKF_SEM_LOCKKEY);
            rv = ZAYK_SEM_Create(semName, 1, &g_semHandle);
            //创建锁定设备信号量
            sprintf_s(semName, sizeof(semName), "%d", ZKSKF_SEM_LOCKKEY + 1);
            rv = ZAYK_SEM_Create(semName, 1, &g_lockSemHandle);

            // run_env_init();
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            //销毁信号量
            LOGI("DllMain End.");
            // run_env_exit();
            ZAYK_SEM_Delete(g_semHandle);
            ZAYK_SEM_Delete(g_lockSemHandle);
            ZAYK_SEM_Delete(g_logSemHandle);
            break;
    }
    memset(&lockInfo, 0x00, sizeof(lockInfo));
    return TRUE;
}
#else
int load() __attribute__((constructor));
int deload() __attribute__((destructor));

int load()
{
    int rv = 0;
    int semName = 0;

    semName = ZKSKF_SEM_LOCKKEY + 2;
    rv = ZAYK_SEM_Create(semName, 1, &g_logSemHandle);

    LOGI("SDK VERSION: " GIT_VERSION);

    //创建信号量
    semName = ZKSKF_SEM_LOCKKEY;
    rv = ZAYK_SEM_Create(semName, 1, &g_semHandle);
    //创建锁定设备信号量
    semName = ZKSKF_SEM_LOCKKEY + 1;
    rv = ZAYK_SEM_Create(semName, 1, &g_lockSemHandle);
    memset(&lockInfo, 0x00, sizeof(lockInfo));
}

int deload()
{
    int rv = 0;
	//销毁信号量
	//释放申请的资源
    ZAYK_SEM_Delete(g_semHandle);
    ZAYK_SEM_Delete(g_lockSemHandle);
    ZAYK_SEM_Delete(g_logSemHandle);
}
#endif
#ifdef __cplusplus
}
#endif
