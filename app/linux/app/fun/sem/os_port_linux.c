#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <string.h>

#include "os_port.h"
#include "Zayk_KeyU_log.h"

#define SPEED_TEST 1

// TODO 信号量移植
int ZAYK_SEM_Create(SEM_KEY SemaphoreName, int SemaphoreNumber, SEM_HANDLE *SemaphoreHandle)
{
    #if (SPEED_TEST)
    *SemaphoreHandle = semget(SemaphoreName, SemaphoreNumber, IPC_CREAT | 0666);
    if (*SemaphoreHandle < 0) {
        LOGE("ZAYK_SEM_Create err, errno = %d errStr = %s\n", errno, strerror(errno));

        return -1;
    }
    #endif

    // LOGI("ZAYK_SEM_Create success.\n");
    return 0;
}

int ZAYK_SEM_Delete(SEM_HANDLE SemaphoreHandle)
{
    #if (SPEED_TEST)
    int ret = 0;
    ret = semctl(SemaphoreHandle, 0, IPC_RMID, 0);
    if (ret == -1) {
        LOGE("ZAYK_SEM_Delete err, errno = %d errStr = %s\n", errno, strerror(errno));

        return -1;
    }
    #endif

    // LOGI("ZAYK_SEM_Delete success.\n");
    return 0;
}

int ZAYK_SEM_P(SEM_HANDLE SemaphoreHandle, int SemaphoreNo, int TimeOut)
{
    #if (SPEED_TEST)
    int ret = 0;
    struct sembuf op_lock[2];

    op_lock[0].sem_num = SemaphoreNo;
    op_lock[0].sem_op = 0;
    op_lock[0].sem_flg = SEM_UNDO;
    op_lock[1].sem_num = SemaphoreNo;
    op_lock[1].sem_op = 1;
    op_lock[1].sem_flg = SEM_UNDO;
    ret = semop(SemaphoreHandle, &op_lock[0], 2);
    if (ret < 0) {
        LOGE("ZAYK_SEM_P err, errno = %d errStr = %s\n", errno, strerror(errno));
        return -1;
    }
    #endif

    // LOGI("ZAYK_SEM_P success.\n");
    return 0;
}

int ZAYK_SEM_V(SEM_HANDLE SemaphoreHandle, int SemaphoreNo)
{
    #if (SPEED_TEST)
    int ret = 0;
    struct sembuf op_unlock[1];

    op_unlock[0].sem_num = SemaphoreNo;
    op_unlock[0].sem_op = -1;
    op_unlock[0].sem_flg = SEM_UNDO;

    ret = semop(SemaphoreHandle, &op_unlock[0], 1);
    if (ret < 0) {
        LOGE("ZAYK_SEM_P err, errno = %d errStr = %s\n", errno, strerror(errno));
        return -1;
    }
    #endif

    // LOGI("ZAYK_SEM_V success.\n");
    return 0;
}
