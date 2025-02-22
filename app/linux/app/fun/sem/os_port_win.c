#include <windows.h>
#include <winbase.h>

#include <string.h>
#include <memory.h>
#include <stdio.h>

#include "Zayk_KeyU_log.h"
#include "os_port.h"

int ZAYK_SEM_Create(SEM_KEY SemaphoreName, int SemaphoreNumber, SEM_HANDLE *SemaphoreHandle)
{
    DWORD dwLastError = 0;

    *SemaphoreHandle = CreateSemaphore(NULL, 1, 1, SemaphoreName);
    dwLastError = GetLastError();
    if (*SemaphoreHandle == 0) {
        LOGE("CreateSemaphore dwLastError : %d", dwLastError);
        return -1;
    }

    return 0;
}

int ZAYK_SEM_Delete(SEM_HANDLE SemaphoreHandle)
{
    int ret = 0;
    DWORD dwLastError = 0;
    ret = CloseHandle((HANDLE)SemaphoreHandle);
    dwLastError = GetLastError();
    if (ret == 0) {
        LOGE("ZAYK_SEM_Delete err, errno: %d", dwLastError);
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
        LOGE("ZAYK_SEM_P wait_timeout ...");
		return -1;
	} else if (WAIT_OBJECT_0 == dwSemaphoreRet) {
		return 0;
	} else {
		dwLastError = GetLastError();
        LOGE("ZAYK_SEM_P err, errno: %d", dwLastError);
        return -1;
	}

    return 0;
}

int ZAYK_SEM_V(SEM_HANDLE SemaphoreHandle, int SemaphoreNo)
{
    int ret = 0;
    DWORD dwLastError=0;

    ret = ReleaseSemaphore((HANDLE)SemaphoreHandle, 1, NULL);
    if (ret == 0) {
        dwLastError = GetLastError();
        LOGE("ZAYK_SEM_V ReleaseSemaphore err, errno: %d", dwLastError);
        return -1;
    }
    
    return 0;
}
