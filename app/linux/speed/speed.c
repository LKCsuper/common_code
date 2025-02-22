/*
 * @Description: 测试速度
 * @Version: 1.0
 * @Author: lkc
 * @detail:
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>

#include <sys/wait.h>
#include <pthread.h>
#include <getopt.h>
#include "sys/time.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <errno.h>

/* Private define ------------------------------------------------------------*/
#define TEST_SUCCESS (0)
#define TEST_FAIL    (1)

// 断言
#define CHECK_RET(ret, str)                                                                                            \
    do {                                                                                                               \
        if (TEST_SUCCESS != ret) {                                                                                     \
            printf("Line[%d] %s error %x\n", __LINE__, str, ret);                                                      \
            getchar();                                                                                                 \
            exit(-1);                                                                                                  \
        }                                                                                                              \
    } while (0);

// 选项类型
typedef enum option_type {
    OPTION_TYPE_NONE = 0, // 无选项
    OPTION_TYPE,
    OPTION_LEN,           // 数据长度
    OPTION_PROCESS,       // 进程个数
    OPTION_THREAD,        // 线程个数
    OPTION_TIM,           // 执行时间
    OPTION_TIMS,          // 执行时间次数
    OPTION_LOOP,          // 循环次数
    OPTION_HEAD,          // 显示头
} OPTION_TYPE_E;

#define PATHNAME "."
#define PROJ_ID  0x6666
/* Private fun declare-----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

typedef struct tagUserParam {
    int size_packet;
} TEST_USER_S;

// 测试选项参数
typedef struct tagTestCaseParam {
    int pid;                      // PID
    int shmAddr;                  // 共享内存位置
    pthread_t tid;                // TID
    int num_pro_pid;              // 线程数
    int num_pro_tid;              // 进程数
    int num_loop;                 // 循环数
    int tim;                      // 执行时间
    int tims;                     // 执行时间次数
    void *(*test_func)(void *);   // 测试函数
    struct timeval starTimThread; // 线程开始时间
    char name[128];               // 算法名字
    unsigned char isBps;          // 是否统计流量

    double elapsedTim;       // 运行时间
    uint64_t runNum;         // 执行总次数
    void *user_data;         // 用户数据
    TEST_USER_S stUserParam; // 用户数据
} TEST_CASE_PARAM_ST;

// 配置数组结构体
typedef struct tagTestCase {
    char name[128];             // 测试名字
    void *(*test_func)(void *); // 测试函数
    int typeId;                 // 测试类型
    unsigned char isBps;        /* 是否统计流量 */
} TEST_CASE_ST;

enum {
    TYPE_TEST = 1,
} TYPE_ID;

/* Private macro -------------------------------------------------------------*/
void *TEST_Func(void *param);
/* Private variables ---------------------------------------------------------*/
TEST_CASE_ST gastTestCase[] = {
    {
        "TEST_DEMO",
        TEST_Func,
        TYPE_TEST,
        1,
    },
};

// 配置选项
static struct option opts[] = {
    {"type", required_argument, NULL, OPTION_TYPE},     {"process", required_argument, NULL, OPTION_PROCESS},
    {"thread", required_argument, NULL, OPTION_THREAD}, {"time", required_argument, NULL, OPTION_TIM},
    {"times", required_argument, NULL, OPTION_TIMS},    {"loop", required_argument, NULL, OPTION_LOOP},
    {"head", no_argument, NULL, OPTION_HEAD},{"len", required_argument, NULL, OPTION_LEN},
};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void *TEST_Func(void *param)
{
    int ret = 0;
    int i = 0;
    struct timeval stCurTim, stStartTim;
    int timeout;

    TEST_CASE_PARAM_ST *pstTestCaseParam = (TEST_CASE_PARAM_ST *)param;

    timeout = pstTestCaseParam->tim;
    gettimeofday(&stStartTim, NULL);
    gettimeofday(&stCurTim, NULL);

    if (timeout > 0) {
        while ((stCurTim.tv_sec + stCurTim.tv_usec / 1000000.0) -
                   (stStartTim.tv_sec + stStartTim.tv_usec / 1000000.0) <=
               pstTestCaseParam->tim) {
            // ret = 运行函数
            CHECK_RET(ret, "test");

            pstTestCaseParam->runNum++;
            gettimeofday(&stCurTim, NULL);
        }
    } else {
        for (i = 0; i < pstTestCaseParam->num_loop; i++) {
            // ret = 运行函数
            CHECK_RET(ret, "test");
        }
    }

    return NULL;
}


/**
 * @description: 使用说明
 * @detail:
 * @return {*}
 * @author: lkc
 */
static void usage(void)
{
    printf("usage:\n");
    printf("  --type  \t: 速度类型\n");
    printf("  --process  \t: 进程个数\n");
    printf("  --thread  \t: 线程个数\n");
    printf("  --time  \t: 测试时间\n");
    printf("  --times  \t: 测试次数\n");
    printf("  --loop  \t: 测试循环\n");
    printf("  --head  \t: 表头\n");
    printf("  --注意: \t: process与thread不能同时选择  time与loop不能同时选择\n");
}

/**
 * @description: 获取cpu信息
 * @detail:
 * @return {*}
 * @author: lkc
 */
void CPU_GetInfo(void)
{
    FILE *fp;
    char buf[4096];
    fp = popen("lscpu", "r");

    printf("|");
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        if (0 == memcmp("Architecture", buf, strlen("Architecture"))) {
            buf[strlen(buf) - 1] = 0;
            char *pFind = strstr(buf, "：");
            if (NULL == pFind) {
                pFind = strstr(buf, ":");
            }
            if (NULL != pFind) {
                *(pFind) = 0;
            }
            *pFind = 0;
            printf("%s:", buf);
            pFind++;
            while (isspace(*(pFind++))) {
            }
            pFind--;
            printf("%s|", pFind);
        } else if (0 == memcmp("型号名称", buf, strlen("型号名称"))) {
            buf[strlen(buf) - 1] = 0;
            char *pFind = strstr(buf, "：");
            if (NULL == pFind) {
                pFind = strstr(buf, ":");
            }
            if (NULL != pFind) {
                *(pFind) = 0;
            }
            printf("%s:", buf);
            pFind++;
            pFind++;
            pFind++;
            while (isspace(*(pFind++))) {
            }
            pFind--;
            printf("%s|", pFind);
        } else if (0 == memcmp("Model name", buf, strlen("Model name"))) {
            buf[strlen(buf) - 1] = 0;
            char *pFind = strstr(buf, ":");
            if (NULL == pFind) {
                pFind = strstr(buf, ":");
            }
            if (NULL != pFind) {
                *(pFind) = 0;
            }
            printf("%s:", buf);
            pFind++;
            pFind++;
            pFind++;
            while (isspace(*(pFind++))) {
            }
            pFind--;
            printf("%s|", pFind);
        } else if (0 == memcmp("座", buf, strlen("座"))) {
            buf[strlen(buf) - 1] = 0;
            char *pFind = strstr(buf, "：");
            if (NULL == pFind) {
                pFind = strstr(buf, ":");
            }
            if (NULL != pFind) {
                *(pFind) = 0;
            }
            printf("%s:", buf);
            pFind++;
            pFind++;
            pFind++;
            while (isspace(*(pFind++))) {
            }
            pFind--;
            printf("%s|", pFind);
        } else if (0 == memcmp("Socket(s)", buf, strlen("Socket(s)"))) {
            buf[strlen(buf) - 1] = 0;
            char *pFind = strstr(buf, ":");
            if (NULL == pFind) {
                pFind = strstr(buf, ":");
            }
            if (NULL != pFind) {
                *(pFind) = 0;
            }
            printf("%s:", buf);
            pFind++;
            pFind++;
            pFind++;
            while (isspace(*(pFind++))) {
            }
            pFind--;
            printf("%s|", pFind);
        } else if (0 == memcmp("BogoMIPS", buf, strlen("BogoMIPS"))) {
            buf[strlen(buf) - 1] = 0;
            char *pFind = strstr(buf, "：");
            if (NULL == pFind) {
                pFind = strstr(buf, ":");
            }
            if (NULL != pFind) {
                *(pFind) = 0;
            }
            printf("%s:", buf);
            pFind++;
            pFind++;
            pFind++;
            while (isspace(*(pFind++))) {
            }
            pFind--;
            printf("%s|", pFind);
        }
        memset(buf, 0x00, sizeof(buf));
        fflush(stdout);
    }
    pclose(fp);

    // 操作系统
    fp = popen("cat /etc/system-release", "r");
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        buf[strlen(buf) - 1] = 0;
        printf("%s ", buf);
    }
    pclose(fp);

    // 内核版本
    fp = popen("uname -r", "r");
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        buf[strlen(buf) - 1] = 0;
        printf("%s|", buf);
    }
    pclose(fp);

    printf("\n");
    return;
}

/**
 * @description: 共享内存通用操作
 * @detail:
 * @param {int} size
 * @param {int} flags
 * @return {*}
 * @author: lkc
 */
static int SHM_Comm(int size, int flags)
{
    key_t key = ftok(PATHNAME, PROJ_ID);
    if (key < 0) {
        perror("ftok");
        return -1;
    }
    int shmid = 0;
    if ((shmid = shmget(key, size, flags)) < 0) {
        perror("shmget");
        return -2;
    }
    return shmid;
}

/**
 * @description: 销毁共享内存
 * @detail:
 * @param {int} shmid
 * @return {*}
 * @author: lkc
 */
static int SHM_Destory(int shmid)
{
    if (shmctl(shmid, IPC_RMID, NULL) < 0) {
        perror("shmctl");
        return -1;
    }
    return 0;
}

/**
 * @description: 进程测试
 * @detail:
 * @param {int} num_proc
 * @param {int} num_loop
 * @param {int} size_packet
 * @param {void} *
 * @param {void} *arg
 * @return {*}
 * @author: lkc
 */
void run_process(TEST_CASE_PARAM_ST *pstTestCaseParam)
{
    struct timeval start_time, end_time;
    struct timeval start_time_thread;
    int i = 0;
    int shmid;
    pid_t pid;

    TEST_CASE_PARAM_ST *pstParam = pstTestCaseParam;

    srand((unsigned int)time(NULL));
    gettimeofday(&start_time_thread, NULL); // 获取开始时间

    pstParam->starTimThread = start_time_thread; // 线程开始时间

    // TODO 预处理

    gettimeofday(&start_time, NULL); // 获取开始时间

    /* 创建共享内存 */
    shmid = SHM_Comm(4096, IPC_CREAT | 0666);
    /* 判断是否创建成功 */
    if (shmid < 0) {
        if (errno == EEXIST) {
            shmid = SHM_Comm(4096, IPC_CREAT);
        } else {
            perror("shmget err\n");
            exit(-1);
        }
    }

    for (i = 0; i < pstParam->num_pro_tid; i++) {
        pstParam->shmAddr = i; // 共享内存，内存地址
        pid = fork();          // 创建子进程
        if (pid < 0) {
            printf("Error: fork failed.\n");
            exit(1);
        } else if (pid == 0) {
            pstParam->pid = getpid();
            pstParam->test_func(pstParam); // 执行测试函数
            // 打或者创建共享内存
            int shmid = SHM_Comm(4096, IPC_CREAT);
            /* 共享内存连接到当前进程空间 */
            char *addr = shmat(shmid, NULL, 0);
            uint64_t *pAddr64 = (uint64_t *)addr;
            pAddr64[pstParam->shmAddr] = pstParam->runNum; // 每次次数+1
            // 断开进程和内存的链接
            shmdt(addr);
            exit(0);
        }
    }

    // 等待所有子进程结束
    int status;
    for (i = 0; i < pstParam->num_pro_tid; i++) {
        wait(&status);
    }

    // 共享内存链接到当前进程的地址空间
    char *addr = shmat(shmid, NULL, 0);
    uint64_t *pAddr64 = (uint64_t *)addr;
    for (i = 0; i < pstParam->num_pro_tid; i++) {
        pstParam->runNum += pAddr64[i];
    }

    // 销毁共享内存
    shmdt(addr);
    SHM_Destory(shmid);

    gettimeofday(&end_time, NULL);

    double elapsed_time = end_time.tv_sec - start_time.tv_sec + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

    // 获取执行时间以及执行次数
    pstParam->elapsedTim = elapsed_time;

    // TODO 释放内存

    return;
}

/**
 * @description: 线程测试
 * @detail:
 * @param {int} num_thread
 * @param {int} num_loop
 * @param {int} size_packet
 * @param {void} *
 * @param {void} *arg
 * @return {*}
 * @author: lkc
 */
void run_thread(TEST_CASE_PARAM_ST *pstTestCaseParam)
{
    struct timeval start_time, end_time;
    struct timeval start_time_thread;
    int i = 0;

    TEST_CASE_PARAM_ST *pstParam;

    srand((unsigned int)time(NULL));
    gettimeofday(&start_time_thread, NULL);

    // 申请多个线程变量
    pstParam = malloc((sizeof(TEST_CASE_PARAM_ST) * pstTestCaseParam->num_pro_pid));
    if (NULL == pstParam) {
        printf("malloc err.\n");
        exit(-1);
    }

    // 拷贝副本
    for (i = 0; i < pstTestCaseParam->num_pro_pid; i++) {
        memset(&pstParam[i], 0, sizeof(TEST_CASE_PARAM_ST));
        memcpy(&pstParam[i], pstTestCaseParam, sizeof(TEST_CASE_PARAM_ST));
        pstParam[i].starTimThread = start_time_thread;
    }

    gettimeofday(&start_time, NULL); // 获取多线程开始时间
    for (i = 0; i < pstParam->num_pro_pid; i++) {
        pthread_create(&pstParam[i].tid, NULL, pstParam->test_func, &pstParam[i]);
    }

    for (i = 0; i < pstParam->num_pro_pid; i++) {
        pthread_join(pstParam[i].tid, NULL);
    }
    gettimeofday(&end_time, NULL); // 获取多线程结束时间

    // 计算时间间隔
    pstTestCaseParam->elapsedTim =
        end_time.tv_sec - start_time.tv_sec + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

    for (i = 0; i < pstTestCaseParam->num_pro_pid; i++) {
        pstTestCaseParam->runNum += pstParam[i].runNum;
    }

    // TODO 多线程 释放操作
    for (i = 0; i < pstTestCaseParam->num_pro_pid; i++) {
    }

    return;
}

void speed_cmd(int argc, char **argv, TEST_CASE_PARAM_ST *pstTestCaseParam)
{
    int typeSupportFlag = 0;
    int opt_index = 0;
    int index = 0;
    int i = 0;
    int c = 0;
    char cHeadFlag = 0;

    memset(pstTestCaseParam, 0x00, sizeof(TEST_CASE_PARAM_ST));
    pstTestCaseParam->tims = 1; // 默认执行次数1
    while (1) {
        c = getopt_long(argc, argv, "", opts, &opt_index);

        if (-1 == c) {
            break;
        }
        index = optind - 1;
        switch (c) {
            case OPTION_TYPE:
                for (i = 0; i < sizeof(gastTestCase) / sizeof(TEST_CASE_ST); i++) {
                    if (memcmp(gastTestCase[i].name, argv[index], strlen(gastTestCase[i].name)) == 0) {
                        pstTestCaseParam->test_func = gastTestCase[i].test_func;
                        pstTestCaseParam->isBps = gastTestCase[i].isBps;
                        /* 这里可以传递 需要运算的参数 pstUserParam. = gastTestCase.*/
                        memcpy(pstTestCaseParam->name, gastTestCase[i].name, strlen(gastTestCase[i].name));

                        typeSupportFlag = 1;
                        break;
                    }
                }
                break;
            case OPTION_PROCESS:
                pstTestCaseParam->num_pro_tid = atoi(argv[index]);
                break;
            case OPTION_THREAD:
                pstTestCaseParam->num_pro_pid = atoi(argv[index]);
                break;
            case OPTION_TIM:
                pstTestCaseParam->tim = atoi(argv[index]);
                break;
            case OPTION_TIMS:
                pstTestCaseParam->tims = atoi(argv[index]);
                break;
            case OPTION_LOOP:
                pstTestCaseParam->num_loop = atoi(argv[index]);
                break;
            case OPTION_HEAD:
                cHeadFlag = 1;
                break;
            case OPTION_LEN:
                pstTestCaseParam->stUserParam.size_packet = atoi(argv[index]);
                break;
            default:
                break;
        }
    }

    if (!typeSupportFlag) {
        exit(-1);
    }

    if ((0 != pstTestCaseParam->num_loop) && (0 != pstTestCaseParam->tim)) {
        printf("Loop and Tim can not be used at the same time\n");
        usage();
        exit(-1);
    }

    if ((0 != pstTestCaseParam->num_pro_pid) && (0 != pstTestCaseParam->num_pro_tid)) {
        printf("Process and Thread can not be used at the same time\n");
        usage();
        exit(-1);
    }

    // 判断是否打印表头
    if (1 == cHeadFlag) {
        // 系统信息
        CPU_GetInfo();
        printf("|算法模式        进程 线程 循环 包长 时间 次数\t");
        for (int i = 0; i < pstTestCaseParam->tims; i++) {
            printf("|第%d次指标(tps Mbps)", i);
        }
        printf("|平均指标(tps Mbps)|数值波动(tps Mbps)|\n");
    }


    return;
}

/**
 * @description: 速度测试
 * @detail:
 * @param {int} argc
 * @param {char} *
 * @return {*}
 * @author: lkc
 */
int main(int argc, char **argv)
{
    int i = 0;
    double elapsed_time;
    uint64_t count;
    uint64_t bits;
    double tps;
    double bps;
    double averBps = 0.00, averTps = 0.00;
    double *paBps, *paTps;
    double varBps = 0.00, varTps = 0.00;
    TEST_CASE_PARAM_ST stTestCaseParam;
    TEST_CASE_PARAM_ST *pstTestCaseParam = NULL;
    TEST_USER_S *pstUserParam = NULL;

    pstTestCaseParam = &stTestCaseParam;
    pstUserParam = &stTestCaseParam.stUserParam;

    // 判断参数合法性
    if (argc <= 1) {
        usage();
        exit(-1); // 进程退出
    }

    speed_cmd(argc, argv, pstTestCaseParam);
    
    if (0 != pstTestCaseParam->num_pro_pid) {
        pstTestCaseParam->num_pro_tid = 1;
        printf("|%-15s %02d   %02d   %02d   %-4d %02d   %02d\t", pstTestCaseParam->name, pstTestCaseParam->num_pro_tid,
               pstTestCaseParam->num_pro_pid, pstTestCaseParam->num_loop, pstUserParam->size_packet,
               pstTestCaseParam->tim, pstTestCaseParam->tims);
        fflush(stdout);
        paBps = malloc(sizeof(double) * pstTestCaseParam->tims);
        paTps = malloc(sizeof(double) * pstTestCaseParam->tims);
        for (i = 0; i < pstTestCaseParam->tims; i++) {
            run_thread(pstTestCaseParam);
            pstUserParam = &stTestCaseParam.stUserParam;
            pstTestCaseParam = &stTestCaseParam;

            count = pstTestCaseParam->runNum;
            elapsed_time = pstTestCaseParam->elapsedTim;

            if (0 != pstTestCaseParam->num_loop) {
                count = pstTestCaseParam->num_pro_pid * pstTestCaseParam->num_loop; // 执行多少次
            }
            bits = count * pstUserParam->size_packet * 8; // 执行多少位
            tps = count / elapsed_time;                   // 执行多少次/秒
            bps = bits / elapsed_time;                    // 执行多少位/秒

            paBps[i] = bps;
            paTps[i] = tps;

            printf("|%-10d ", (int)tps);
            if (1 == pstTestCaseParam->isBps) {
                printf("%-8.3f", bps / 1024 / 1024);
            }
            fflush(stdout);
            pstTestCaseParam->runNum = 0;
            pstTestCaseParam->elapsedTim = 0.00;
        }
        /* 求平均值 */
        averBps = 0.0f;
        averTps = 0.0f;
        for (i = 0; i < pstTestCaseParam->tims; i++) {
            averBps += paBps[i];
            averTps += paTps[i];
        }
        averBps = averBps / (double)pstTestCaseParam->tims;
        averTps = averTps / (double)pstTestCaseParam->tims;

        /* 求方差 */
        varBps = 0.0;
        varTps = 0.0;
        for (i = 0; i < pstTestCaseParam->tims; i++) {
            varBps +=
                (paBps[i] / 1024 / 1024 - averBps / 1024 / 1024) * (paBps[i] / 1024 / 1024 - averBps / 1024 / 1024);
            varTps += (paTps[i] - averTps) * (paTps[i] - averTps);
        }
        varBps = sqrt(varBps / (double)pstTestCaseParam->tims);
        varTps = sqrt(varTps / (double)pstTestCaseParam->tims);

        printf("|%-10d%%", (int)averTps);
        if (1 == pstTestCaseParam->isBps) {
            printf("%-8.3f", averBps / 1024 / 1024);
        }

        printf("|%.3f%%   ", (averTps - varTps) / averTps * 100.0);
        if (1 == pstTestCaseParam->isBps) {
            printf("%.3f%%|", (averBps / 1024 / 1024 - varBps) / (averBps / 1024 / 1024) * 100.0);
        }
        printf("\n");
    } else if (0 != stTestCaseParam.num_pro_tid) {
        pstTestCaseParam->num_pro_pid = 1;
        printf("|%-15s %02d   %02d   %02d   %-4d %02d   %02d\t", pstTestCaseParam->name, pstTestCaseParam->num_pro_tid,
               pstTestCaseParam->num_pro_pid, pstTestCaseParam->num_loop, pstUserParam->size_packet,
               pstTestCaseParam->tim, pstTestCaseParam->tims);
        fflush(stdout);
        paBps = malloc(sizeof(double) * stTestCaseParam.tims);
        paTps = malloc(sizeof(double) * stTestCaseParam.tims);
        for (i = 0; i < stTestCaseParam.tims; i++) {
            run_process(&stTestCaseParam);
            pstUserParam = &stTestCaseParam.stUserParam;
            pstTestCaseParam = &stTestCaseParam;

            count = pstTestCaseParam->runNum;
            elapsed_time = pstTestCaseParam->elapsedTim;

            if (0 != pstTestCaseParam->num_loop) {
                count = pstTestCaseParam->num_pro_pid * pstTestCaseParam->num_loop; // 执行多少次
            }
            bits = count * pstUserParam->size_packet * 8; // 执行多少位
            tps = count / elapsed_time;                   // 执行多少次/秒
            bps = bits / elapsed_time;                    // 执行多少位/秒

            paBps[i] = bps;
            paTps[i] = tps;

            printf("|%-9d ", (int)tps);
            if (1 == pstTestCaseParam->isBps) {
                printf("%-9.3f", bps / 1024 / 1024);
            }
            fflush(stdout);
            pstTestCaseParam->runNum = 0;
            pstTestCaseParam->elapsedTim = 0.00;
        }
        /* 求平均值 */
        averBps = 0.0f;
        averTps = 0.0f;
        for (i = 0; i < pstTestCaseParam->tims; i++) {
            averBps += paBps[i];
            averTps += paTps[i];
        }
        averBps = averBps / (double)pstTestCaseParam->tims;
        averTps = averTps / (double)pstTestCaseParam->tims;

        /* 求方差 */
        varBps = 0.0;
        varTps = 0.0;
        for (i = 0; i < pstTestCaseParam->tims; i++) {
            varBps +=
                (paBps[i] / 1024 / 1024 - averBps / 1024 / 1024) * (paBps[i] / 1024 / 1024 - averBps / 1024 / 1024);
            varTps += (paTps[i] - averTps) * (paTps[i] - averTps);
        }
        varBps = sqrt(varBps / (double)pstTestCaseParam->tims);
        varTps = sqrt(varTps / (double)pstTestCaseParam->tims);

        printf("|%-9d%%", (int)averTps);
        if (1 == pstTestCaseParam->isBps) {
            printf("%-9.3f", averBps / 1024 / 1024);
        }

        printf("|%.3f%%   ", (averTps - varTps) / averTps * 100.0);
        if (1 == pstTestCaseParam->isBps) {
            printf("%.3f%%|", (averBps / 1024 / 1024 - varBps) / (averBps / 1024 / 1024) * 100.0);
        }
        printf("\n");
    }

    free(paBps);
    free(paTps);
    return 1;
}
#ifdef __cplusplus
}
#endif