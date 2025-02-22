#ifndef __FF_PORT_H__
#define __FF_PORT_H__
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
typedef struct
{
	uint32_t sector_count;
	uint32_t sector_size;
	uint32_t sector_start_addr;
} fs_disk_t;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 设置flahsh参数
 * @detail: 
 * @param {fs_disk_t} stFsConfig
 * @return {*}
 * @author: lkc
 */
VOID FS_SetConfig(fs_disk_t stFsConfig);
/**
 * @description: 系统格式化
 * @detail:
 * @return {*}
 * @author: lkc
 */
LONG FS_format(VOID);

/**
 * @description: 初始化
 * @detail:
 * @return {*}
 * @author: lkc
 */
LONG FS_Init(VOID);

/**
 * @description: 重新初始化
 * @detail:
 * @return {*}
 * @author: lkc
 */
LONG Fs_ReInit(VOID);

/**
 * @description: 创建文件
 * @detail:
 * @param {UCHAR} *pucFileName
 * @return {*}
 * @author: lkc
 */
ULONG FS_Create(UCHAR *pucFileName);

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
ULONG FS_Read(UCHAR *pucFileName, ULONG ulOffset, UCHAR *pucData, ULONG *pulDatalen);

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
ULONG FS_Write(UCHAR *pucFileName, ULONG ulOffset, UCHAR *pucData, ULONG *pulDatalen);

/**
 * @description: 删除文件
 * @detail:
 * @param {UCHAR} *pucFileName
 * @return {*}
 * @author: lkc
 */
ULONG FS_FileDelete(UCHAR *pucFileName);

/**
 * @description: 枚举文件
 * @detail:
 * @param {UCHAR} *pucFlieList
 * @param {ULONG} *pulListlen
 * @return {*}
 * @author: lkc
 */
ULONG FS_FileEnum(UCHAR *pucFlieList, ULONG *pulListlen);

/**
 * @description: 获取文件长度
 * @detail:
 * @param {CHAR} *ucaFileName
 * @param {ULONG} *pFileLen
 * @return {*}
 * @author: lkc
 */
ULONG FS_FileLengthGet(CHAR *ucaFileName, ULONG *pFileLen);
#ifdef __cplusplus
}
#endif
#endif