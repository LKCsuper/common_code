
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "ff.h"
#include "common.h"
#include "ff_port.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
BYTE work[FF_MAX_SS];
FATFS fs;
FIL file;
FILINFO fileInfo;

BYTE work[FF_MAX_SS];
fs_disk_t fs_disk = 
{
	.sector_count = 256,
	.sector_size = 256,
	.sector_start_addr = 0
};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 设置flahsh参数
 * @detail: 
 * @param {fs_disk_t} stFsConfig
 * @return {*}
 * @author: lkc
 */
VOID FS_SetConfig(fs_disk_t stFsConfig)
{
    fs_disk.sector_count = stFsConfig.sector_count;
    fs_disk.sector_size = stFsConfig.sector_size;
    fs_disk.sector_start_addr = stFsConfig.sector_start_addr;
}

/**
 * @description: 系统格式化
 * @detail:
 * @return {*}
 * @author: lkc
 */
LONG FS_format(VOID)
{
    FRESULT res;

    res = f_mkfs("", FM_ANY, 0, work, sizeof(work));
    if (res != FR_OK) {
        xprintf("fs format error [%d]\r\n", res);
    } else {
        printf("fs format success\r\n");
    }

    return 0;
}

/**
 * @description: 初始化
 * @detail:
 * @return {*}
 * @author: lkc
 */
LONG FS_Init(VOID)
{
    FRESULT res;

    // 需要先取消挂载
    f_mount(NULL, "", 1);

    // 这里如果不选择1 立刻挂载的话，下边的open就会出现FR_NO_FILESYSTEM问题
    res = f_mount(&fs, "", 1);

    /* 如果没有初始化文件系统 */
    if (FR_NO_FILESYSTEM == res) {
        xprintf("init file system [%d]\r\n", res);
        fs_format();

        /* 重新挂载 */
        res = f_mount(&fs, "", 1);
        if (res != FR_OK) {
            xprintf("fs mount error [%d]\r\n", res);
        }
    } else if (res != FR_OK) {
        xprintf("fs mount error [%d]\r\n", res);
    } else {
        printf("fs init success\r\n");
    }

    return 0;
}

/**
 * @description: 重新初始化
 * @detail:
 * @return {*}
 * @author: lkc
 */
LONG Fs_ReInit(VOID)
{
    fs_format();
    fs_init();

    return 0;
}

/**
 * @description: 创建文件
 * @detail:
 * @param {UCHAR} *pucFileName
 * @return {*}
 * @author: lkc
 */
ULONG FS_Create(UCHAR *pucFileName)
{
    FRESULT res;

    res = f_stat(pucFileName, &fileInfo);
    if (res == FR_OK) {
        // CHECK_RETURN(SDR_FILEEXISTS);
    }

    res = f_open(&file, pucFileName, FA_READ | FA_WRITE | FA_CREATE_ALWAYS);
    if (res != FR_OK) {
        // CHECK_RETURN(SDR_FILEWERR);
    }

    res = f_close(&file);
    if (res != FR_OK) {
        // CHECK_RETURN(SDR_FILEWERR);
    }
    return SUCCESS;
}

/**
 * @description: 读取文件
 * @detail:
 * @param {UCHAR} *pucFileName
 * @param {ULONG} ulOffset
 * @param {UCHAR} *pucData
 * @param {ULONG} ulDatalen
 * @return {*}
 * @author: lkc
 */
ULONG FS_Read(UCHAR *pucFileName, ULONG ulOffset, UCHAR *pucData, ULONG *pulDatalen)
{
    FRESULT res;
    UINT br;

    res = f_stat(pucFileName, &fileInfo);
    if (res != FR_OK) {
        // CHECK_RETURN(SDR_FILENOEXIST);
    }

    res = f_open(&file, pucFileName, FA_READ);
    if (res != FR_OK) {
        // CHECK_RETURN(SDR_FILEWERR);
    }

    res = f_lseek(&file, ulOffset);
    if (res != FR_OK) {
        // CHECK_RETURN(SDR_FILEWERR);
    }

    res = f_read(&file, pucData, *pulDatalen, &br);
    if (res != FR_OK) {
        // CHECK_RETURN(SDR_FILEWERR);
    }

    /* 代表文件已经读取完成 */
    if (0 == br) {
    }

    *pulDatalen = htonl(br);

    res = f_close(&file);
    if (res != FR_OK) {
        // CHECK_RETURN(SDR_FILEWERR);
    }
    return SUCCESS;
}

/**
 * @description: 写文件
 * @detail:
 * @param {UCHAR} *pucFileName
 * @param {ULONG} ulOffset
 * @param {UCHAR} *pucData
 * @param {ULONG} ulDataLen
 * @return {*}
 * @author: lkc
 */
ULONG FS_Write(UCHAR *pucFileName, ULONG ulOffset, UCHAR *pucData, ULONG *pulDatalen)
{
    FRESULT res;

    res = f_stat(pucFileName, &fileInfo);
    if (res != FR_OK) {
        // CHECK_RETURN(SDR_FILENOEXIST);
        // 获取设备信息
    }

    res = f_open(&file, pucFileName, FA_WRITE);
    if (res != FR_OK) {
        // CHECK_RETURN(SDR_FILEWERR);
        // 打开失败
    }

    res = f_lseek(&file, ulOffset);
    if (res != FR_OK) {
        // CHECK_RETURN(SDR_FILEWERR);
        // 查找偏移
    }

    UINT_X br;
    res = f_write(&file, pucData, pulDatalen, &br);
    if (res != FR_OK) {
        // CHECK_RETURN(SDR_FILEWERR);
        // 写失败
    }

    /* 代表文件已经写满 */
    if (0 == br) {
    }

    *pulDatalen = htonl(br);

    res = f_close(&file);
    if (res != FR_OK) {
        // CHECK_RETURN(SDR_FILEWERR);
    }
    return SUCCESS;
}

/**
 * @description: 删除文件
 * @detail:
 * @param {UCHAR} *pucFileName
 * @return {*}
 * @author: lkc
 */
ULONG FS_FileDelete(UCHAR *pucFileName)
{
    FRESULT res;

    res = f_stat(pucFileName, &fileInfo);
    if (res != FR_OK) {
        // CHECK_RETURN(SDR_FILENOEXIST);
        // 文件是否存在
    }

    res = f_unlink(pucFileName);
    if (res != FR_OK) {
        // CHECK_RETURN(SDR_FILEWERR);
        // 取消链接失败
    }
    return SUCCESS;
}

/**
 * @description: 枚举文件
 * @detail:
 * @param {UCHAR} *pucFlieList
 * @param {ULONG} *pulListlen
 * @return {*}
 * @author: lkc
 */
ULONG FS_FileEnum(UCHAR *pucFlieList, ULONG *pulListlen)
{
    FRESULT res;
    DIR dir;
    LONG i = 0;

    res = f_opendir(&dir, "/");
    if (res != FR_OK) {
        // CHECK_RETURN(SDR_FILEWERR);
    }

    while (1) {
        res = f_readdir(&dir, &fileInfo);
        if (res != FR_OK) {
            // CHECK_RETURN(SDR_FILEWERR);
        } else if (fileInfo.fname[0] == 0) {
            pucFlieList[i++] = 0;
            uiFileListLen = i;
            break;
        } else {
            if (fileInfo.fattrib & AM_DIR) {

            } else {
                strcpy((CHAR *)pucFlieList + i, fileInfo.fname);
                i += strlen(fileInfo.fname);
                pucFlieList[i++] = 0;
            }
        }
    }

    res = f_closedir(&dir);
    if (res != FR_OK) {
        // CHECK_RETURN(SDR_FILEWERR);
    }

    *pucFlieList = htonl(uiFileListLen);
    return SUCCESS;
}

/**
 * @description: 获取文件长度
 * @detail:
 * @param {CHAR} *ucaFileName
 * @param {ULONG} *pFileLen
 * @return {*}
 * @author: lkc
 */
ULONG FS_FileLengthGet(CHAR *ucaFileName, ULONG *pFileLen)
{
    FRESULT res;
    LONG uiFileSize = 0;

    res = f_stat(ucaFileName, &fileInfo);
    if (res != FR_OK) {
        // CHECK_RETURN(SDR_FILENOEXIST);
        //  文件不存在
    }

    res = f_open(&file, ucaFileName, FA_READ);
    if (res != FR_OK) {
        // CHECK_RETURN(SDR_FILEWERR);
        // 打开错误
    }

    uiFileSize = f_size(&file);

    res = f_close(&file);
    if (res != FR_OK) {
        // CHECK_RETURN(SDR_FILEWERR);
        // 关闭错误
    }

    *pFileLen = htonl(uiFileSize);
    return SUCCESS;
}

#ifdef __cplusplus
}
#endif