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
#include "windows.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
// 获取当前用户路径
void system_dir_root(char *path)
{
    char *pSystemPan;
    char *pSystemPath;

    pSystemPan = getenv("SYSTEMROOT");
    if (pSystemPan == NULL) {
        return;
    }

    pSystemPath = getenv("HOMEPATH");
    if (pSystemPath == NULL) {
        return;
    }
    sprintf(path, "%c:\%s", pSystemPan[0], pSystemPath);

    return;
}

// 检查文件是否存在
int system_dir_check(char *pDirPath)
{
    int rv = 0;

    if (pDirPath == NULL) {
        return -1;
    }

    rv = GetFileAttributes(pDirPath);
    if (rv == INVALID_FILE_ATTRIBUTES) {
        return -1;
    }

    if (rv & FILE_ATTRIBUTE_DIRECTORY) {
        return 0;
    }

    return -1;
}


// 创建文件夹
int system_dir_create(char *pPath)
{
    int ret = 0;
    char sysCmd[512] = {0};

    if (NULL == pPath)
    {
        return -1;
    }

    sprintf(sysCmd, "mkdir \"%s\"", pPath);

    ret = system(sysCmd);
    if (ret == 0) {
        printf("文件夹创建成功！\n");
    } else {
        printf("文件夹创建失败，错误代码：%d\n", ret);
    }

    return ret;
}

// 获取当前路径
int system_dir_cur(char *curDir, uint32_t dirNameMaxLen)
{
    if ((NULL == curDir) || (0 == dirNameMaxLen))
    {
        return -1;
    }

    if (GetCurrentDirectory(dirNameMaxLen, curDir) != 0) {
        printf("当前工作目录是：%s\n", curDir);
        return 0;
    } else {
        printf("获取当前工作目录失败，错误代码: %lu\n", GetLastError());
        return -1;
    }
}

// 删除文件夹
int system_dir_delete(char *dirPath)
{
    int ret = 0;
    char sysCmd[512] = {0};

    if (NULL == dirPath)
    {
        return -1;
    }

    // 检测文件是否存在
    // ret = system_dir_check(dirPath);

    sprintf(sysCmd, "del /f /q \"%s\\*.*\" && rmdir /s /q \"%s\"", dirPath, dirPath);
    ret = system(sysCmd);
    if (ret == 0) {
        printf("文件夹删除成功！\n");
    } else {
        printf("文件夹删除失败，错误代码：%d\n", ret);
    }

    return ret;
}

// 拷贝文件夹
int system_dir_copy(char *srcDirPath, char *dstDirPath)
{
    int ret = 0;
    char sysCmd[512] = {0};

    if ((NULL == srcDirPath) || (NULL == dstDirPath))
    {
        return -1;
    }

    // 使用xcopy
    sprintf(sysCmd, "xcopy \"%s\" \"%s\"", srcDirPath, dstDirPath);
    ret = system(sysCmd);
    if (ret == 0) {
        printf("文件夹拷贝成功！\n");
    } else {
        printf("文件夹拷贝失败，错误代码：%d\n", ret);
    }

    return ret;
}
#ifdef __cplusplus
}
#endif
