#include "integer.h"
#include "zayk_comm.h"
#include "zayk_errcode_define.h"

#include "ff.h"

#include <stdio.h>
#include "log.h"
#include "bsp.h"

extern uint8_t temp_buff[4096];

BYTE_X work[FF_MAX_SS];
FRESULT res;
FATFS fs;
FIL file;
FILINFO fileInfo;

int fs_format(void)
{
    res = f_mkfs("", FM_ANY, 0, work, sizeof(work));
    if (res != FR_OK) {
        xprintf("fs format error [%d]\r\n", res);
    }
    else
    {
        printf("fs format success\r\n");
    }

    return 0;
}

int fs_init(void)
{
    // 需要先取消挂载
    f_mount(NULL, "", 1);

    // 这里如果不选择1 立刻挂载的话，下边的open就会出现FR_NO_FILESYSTEM问题
    res = f_mount(&fs, "", 1);

    /* 如果没有初始化文件系统 */
    if (FR_NO_FILESYSTEM == res)
    {
        xprintf("init file system [%d]\r\n", res);
        fs_format();
        
        /* 重新挂载 */
        res = f_mount(&fs, "", 1);
        if (res != FR_OK) {
            xprintf("fs mount error [%d]\r\n", res);
        }
    }
    else if (res != FR_OK)
    {
        xprintf("fs mount error [%d]\r\n", res);
    }
    else
    {
        printf("fs init success\r\n");
    }

    return 0;
}

int fs_init_ex(void)
{
    // 擦除整个文件系统分区
    // rsp_hal_spi_erase_flash(&spi_master_cap_store, FLASH_VM_USER_FILE_BASE_ADDR_IDX(0), 1*1024*1024);
    fs_format();
    fs_init();

    return 0;
}

uint32_t zayk_file_create(uint32_t cmd, void *pvpack, uint32_t vidx)
{
    // struct cmdqfunc_pack_t *ppack = pvpack;
    struct comm_file_opera_para_t *ppara;
    char ucaFileName[64];
    uint32_t uiFileNameLen;

    ppara         = (struct comm_file_opera_para_t *)pvpack;
    uiFileNameLen = ntohl(ppara->uiFileNameLen);
    memset(ucaFileName, 0, sizeof(ucaFileName));
    memcpy(ucaFileName, ppara->ucaFileName, uiFileNameLen);

    res = f_stat(ucaFileName, &fileInfo);
    if (res == FR_OK) {
        RETURN_CHECK(SDR_FILEEXISTS);
    }

    res = f_open(&file, ucaFileName, FA_READ | FA_WRITE | FA_CREATE_ALWAYS);
    if (res != FR_OK) {
        RETURN_CHECK(SDR_FILEWERR);
    }

    res = f_close(&file);
    if (res != FR_OK) {
        RETURN_CHECK(SDR_FILEWERR);
    }
    return SDR_OK;
}

uint32_t zayk_file_read(uint32_t cmd, void *pvpack, uint32_t vidx)
{
    // struct cmdqfunc_pack_t *ppack = pvpack;
    struct comm_file_opera_para_t *ppara;
    char ucaFileName[64];
    uint32_t uiFileNameLen;
    uint32_t uiOffset;
    uint32_t uiDataLen;

    ppara         = (struct comm_file_opera_para_t *)pvpack;
    uiFileNameLen = ntohl(ppara->uiFileNameLen);
    memset(ucaFileName, 0, sizeof(ucaFileName));
    memcpy(ucaFileName, ppara->ucaFileName, uiFileNameLen);
    uiOffset  = ntohl(ppara->uiOffset);
    uiDataLen = ntohl(ppara->uiDataLen);

    res = f_stat(ucaFileName, &fileInfo);
    if (res != FR_OK) {
        RETURN_CHECK(SDR_FILENOEXIST);
    }

    res = f_open(&file, ucaFileName, FA_READ);
    if (res != FR_OK) {
        RETURN_CHECK(SDR_FILEWERR);
    }

    res = f_lseek(&file, uiOffset);
    if (res != FR_OK) {
        RETURN_CHECK(SDR_FILEWERR);
    }

    UINT_X br;
    res = f_read(&file, ppara->ucaData, uiDataLen, &br);
    if (res != FR_OK) {
        RETURN_CHECK(SDR_FILEWERR);
    }

    ppara->uiDataLen = htonl(br);

    res = f_close(&file);
    if (res != FR_OK) {
        RETURN_CHECK(SDR_FILEWERR);
    }
    return SDR_OK;
}

uint32_t zayk_file_write(uint32_t cmd, void *pvpack, uint32_t vidx)
{
    // struct cmdqfunc_pack_t *ppack = pvpack;
    struct comm_file_opera_para_t *ppara;
    char ucaFileName[64];
    uint32_t uiFileNameLen;
    uint32_t uiOffset;
    uint32_t uiDataLen;

    ppara         = (struct comm_file_opera_para_t *)pvpack;
    uiFileNameLen = ntohl(ppara->uiFileNameLen);
    memset(ucaFileName, 0, sizeof(ucaFileName));
    memcpy(ucaFileName, ppara->ucaFileName, uiFileNameLen);
    uiOffset  = ntohl(ppara->uiOffset);
    uiDataLen = ntohl(ppara->uiDataLen);

    res = f_stat(ucaFileName, &fileInfo);
    if (res != FR_OK) {
        RETURN_CHECK(SDR_FILENOEXIST);
    }

    res = f_open(&file, ucaFileName, FA_WRITE);
    if (res != FR_OK) {
        RETURN_CHECK(SDR_FILEWERR);
    }

    res = f_lseek(&file, uiOffset);
    if (res != FR_OK) {
        RETURN_CHECK(SDR_FILEWERR);
    }

    UINT_X br;
    res = f_write(&file, ppara->ucaData, uiDataLen, &br);
    if (res != FR_OK) {
        RETURN_CHECK(SDR_FILEWERR);
    }

    ppara->uiDataLen = htonl(br);

    res = f_close(&file);
    if (res != FR_OK) {
        RETURN_CHECK(SDR_FILEWERR);
    }
    return SDR_OK;
}

uint32_t zyak_file_delete(uint32_t cmd, void *pvpack, uint32_t vidx)
{
    // struct cmdqfunc_pack_t *ppack = pvpack;
    struct comm_file_opera_para_t *ppara;
    char ucaFileName[64];
    uint32_t uiFileNameLen;

    ppara         = (struct comm_file_opera_para_t *)pvpack;
    uiFileNameLen = ntohl(ppara->uiFileNameLen);
    memset(ucaFileName, 0, sizeof(ucaFileName));
    memcpy(ucaFileName, ppara->ucaFileName, uiFileNameLen);

    res = f_stat(ucaFileName, &fileInfo);
    if (res != FR_OK) {
        RETURN_CHECK(SDR_FILENOEXIST);
    }

    res = f_unlink(ucaFileName);
    if (res != FR_OK) {
        RETURN_CHECK(SDR_FILEWERR);
    }
    return SDR_OK;
}

uint32_t zayk_file_enum(uint32_t cmd, void *pvpack, uint32_t vidx)
{
    // struct cmdqfunc_pack_t *ppack = pvpack;
    struct comm_file_opera_para_t *ppara;
    uint32_t uiFileListLen;

    ppara = (struct comm_file_opera_para_t *)pvpack;

    DIR dir;
    int i = 0;
    res   = f_opendir(&dir, "/");
    if (res != FR_OK) {
        RETURN_CHECK(SDR_FILEWERR);
    }

    while (1) {
        res = f_readdir(&dir, &fileInfo);
        if (res != FR_OK) {
            RETURN_CHECK(SDR_FILEWERR);
        } else if (fileInfo.fname[0] == 0) {
            temp_buff[i++] = 0;
            uiFileListLen    = i;
            break;
        } else {
            if (fileInfo.fattrib & AM_DIR) {

            } else {
                strcpy((char *)temp_buff + i, fileInfo.fname);
                i += strlen(fileInfo.fname);
                temp_buff[i++] = 0;
            }
        }
    }

    res = f_closedir(&dir);
    if (res != FR_OK) {
        RETURN_CHECK(SDR_FILEWERR);
    }

    memcpy(ppara->ucaData, temp_buff, uiFileListLen);
    ppara->uiDataLen = htonl(uiFileListLen);
    return SDR_OK;
}

uint32_t zayk_file_length_get(uint32_t cmd, void *pvpack, uint32_t vidx)
{
    // struct cmdqfunc_pack_t *ppack = pvpack;
    struct comm_file_opera_para_t *ppara;
    char ucaFileName[64];
    uint32_t uiFileNameLen;

    ppara         = (struct comm_file_opera_para_t *)pvpack;
    uiFileNameLen = ntohl(ppara->uiFileNameLen);
    memset(ucaFileName, 0, sizeof(ucaFileName));
    memcpy(ucaFileName, ppara->ucaFileName, uiFileNameLen);

    res = f_stat(ucaFileName, &fileInfo);
    if (res != FR_OK) {
        RETURN_CHECK(SDR_FILENOEXIST);
    }

    res = f_open(&file, ucaFileName, FA_READ);
    if (res != FR_OK) {
        RETURN_CHECK(SDR_FILEWERR);
    }

    int uiFileSize = 0;
    uiFileSize     = f_size(&file);

    res = f_close(&file);
    if (res != FR_OK) {
        RETURN_CHECK(SDR_FILEWERR);
    }

    ppara->uiFileSize = htonl(uiFileSize);
    return SDR_OK;
}
