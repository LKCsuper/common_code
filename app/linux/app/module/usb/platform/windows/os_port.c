#include <windows.h>
#include <winbase.h>

#include <string.h>
#include <memory.h>
#include <stdio.h>

#include "../../os_port.h"

#include "ZaykSkf_api.h"
#include "ZaykSkfDebug.h"

int ZAYK_SEM_Create(SEM_KEY SemaphoreName, int SemaphoreNumber, SEM_HANDLE *SemaphoreHandle)
{
    DWORD dwLastError = 0;
    // HANDLE sem_1;
    // SECURITY_ATTRIBUTES sa;
    // SECURITY_ATTRIBUTES sd;

    // if (0 == InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION)) {
    //     dwLastError = GetLastError();
    //     LOGE("ZAYK_SEM_Create InitializeSecurityDescriptor err, errno: %d\n", dwLastError);
    //     return -1;
    // }
    // if (0 == SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE)) {
    //     dwLastError = GetLastError();
    //     LOGE("ZAYK_SEM_Create SetSecurityDescriptorDacl err, errno: %d\n", dwLastError);
    //     return -1;
    // }
    // sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    // sa.bInheritHandle = TRUE;
    // sa.lpSecurityDescriptor = &sd;

    *SemaphoreHandle = CreateSemaphore(NULL, 1, 1, SemaphoreName);
    dwLastError = GetLastError();
    // if (ERROR_ALREADY_EXISTS == dwLastError) {
        
    // } else {
    //     if (NULL != hsem) {
    //         goto __finish;
    //     } else {
    //         LOGE("ZAYK_SEM_Create err, errno: %d\n", dwLastError);
    //         return -1;
    //     }
    // }
    LOGE("ZAYK_SEM_Create CreateSemaphore dwLastError : %d\n", dwLastError);
    if (*SemaphoreHandle == 0) {
        LOGE("ZAYK_SEM_Create CreateSemaphore dwLastError : %d\n", dwLastError);
        return -1;
    }

    return 0;
}

int ZAYK_SEM_Delete(SEM_HANDLE SemaphoreHandle)
{
    // DWORD dwLastError = 0;
    // if (NULL != (HANDLE)SemaphoreHandle) {
    //     if (CloseHandle((HANDLE)SemaphoreHandle) == 0) {
    //         dwLastError = GetLastError();
    //         LOGE("ZAYK_SEM_Delete err, errno: %d\n", dwLastError);
    //         return -1;
    //     }
    // } else {
    //     return -1;
    // }
    int ret = 0;
    DWORD dwLastError = 0;
    ret = CloseHandle((HANDLE)SemaphoreHandle);
    dwLastError = GetLastError();
    if (ret == 0) {
        LOGE("ZAYK_SEM_Delete err, errno: %d\n", dwLastError);
        return -1;
    }

    return 0;
}

int ZAYK_SEM_P(SEM_HANDLE SemaphoreHandle, int SemaphoreNo, int TimeOut)
{
    DWORD dwSemaphoreRet=0;
	DWORD dwLastError=0;

    dwSemaphoreRet = WaitForSingleObject((HANDLE)SemaphoreHandle, TimeOut);
    if (WAIT_TIMEOUT == dwSemaphoreRet)  {
		// ReleaseSemaphore((HANDLE)SemaphoreHandle, 1, NULL);
        LOGE("ZAYK_SEM_P wait_timeout ... \n");
		return -1;
	} else if (WAIT_OBJECT_0 == dwSemaphoreRet) {
		return 0;
	} else {
		dwLastError = GetLastError();
        LOGE("ZAYK_SEM_P err, errno: %d\n", dwLastError);
        return -1;
	}

    return 0;
}

int ZAYK_SEM_V(SEM_HANDLE SemaphoreHandle, int SemaphoreNo)
{
    // DWORD dwLastError=0;
    // if (NULL != (HANDLE)SemaphoreHandle) {
    //     if (!ReleaseSemaphore((HANDLE)SemaphoreHandle, 1, NULL)) {
    //         dwLastError = GetLastError();
    //         LOGE("ZAYK_SEM_V err, errno: %d\n", dwLastError);
    //         return -1;
    //     }
    // } else {
    //     return -1;
    // }
    int ret = 0;
    DWORD dwLastError=0;

    ret = ReleaseSemaphore((HANDLE)SemaphoreHandle, 1, NULL);
    if (ret == 0) {
        dwLastError = GetLastError();
        LOGE("ZAYK_SEM_V ReleaseSemaphore err, errno: %d\n", dwLastError);
        return -1;
    }
    
    return 0;
}
