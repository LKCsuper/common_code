#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <pthread.h>

#include "../../os_port.h"

#include "ZaykSkf_api.h"
#include "ZaykSkfDebug.h"

int ZAYK_SEM_Create(SEM_KEY SemaphoreName, int SemaphoreNumber, SEM_HANDLE *SemaphoreHandle)
{
    pthread_mutex_t *lpThreadMutex = NULL;

    lpThreadMutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    if (lpThreadMutex == NULL) {
        LOGE("ZAYK_SEM_Create malloc err.\n");
        return -1;
    }

    pthread_mutex_init(lpThreadMutex, NULL);

    *SemaphoreHandle = (void *)lpThreadMutex;

    return 0;
}

int ZAYK_SEM_Delete(SEM_HANDLE SemaphoreHandle)
{
    pthread_mutex_t *lpThreadMutex = NULL;

    if ((void *)SemaphoreHandle == NULL) {
        return 0;
    }

    lpThreadMutex = (pthread_mutex_t *)SemaphoreHandle;

    pthread_mutex_destroy(lpThreadMutex);
    if (lpThreadMutex != NULL) {
        free(lpThreadMutex);
    }

    return 0;
}

int ZAYK_SEM_P(SEM_HANDLE SemaphoreHandle, int SemaphoreNo, int TimeOut)
{
    pthread_mutex_t *lpThreadMutex = NULL;

    lpThreadMutex = (pthread_mutex_t *)SemaphoreHandle;

    pthread_mutex_lock(lpThreadMutex);

    return 0;
}

int ZAYK_SEM_V(SEM_HANDLE SemaphoreHandle, int SemaphoreNo)
{
    pthread_mutex_t *lpThreadMutex = NULL;

    lpThreadMutex = (pthread_mutex_t *)SemaphoreHandle;

    pthread_mutex_unlock(lpThreadMutex);

    return 0;
}
