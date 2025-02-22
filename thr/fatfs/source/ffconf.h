/*---------------------------------------------------------------------------/
/  Configurations of FatFs Module
/---------------------------------------------------------------------------*/

#define FFCONF_DEF	80286	/* Revision ID */

/*---------------------------------------------------------------------------/
/ Function Configurations
/---------------------------------------------------------------------------*/

#define FF_FS_READONLY	0  // 使能或者禁止与写相关函数
/* This option switches read-only configuration. (0:Read/Write or 1:Read-only)
/  Read-only configuration removes writing API functions, f_write(), f_sync(),
/  f_unlink(), f_mkdir(), f_chmod(), f_rename(), f_truncate(), f_getfree()
/  and optional writing functions as well. */


#define FF_FS_MINIMIZE	0 // 功能裁剪
/* This option defines minimization level to remove some basic API functions.
/
/   0: Basic functions are fully enabled.
/   1: f_stat(), f_getfree(), f_unlink(), f_mkdir(), f_truncate() and f_rename()
/      are removed.
/   2: f_opendir(), f_readdir() and f_closedir() are removed in addition to 1.
/   3: f_lseek() function is removed in addition to 2. */


#define FF_USE_FIND		0 // 是能或禁用在指定目录搜索指定文件
/* This option switches filtered directory read functions, f_findfirst() and
/  f_findnext(). (0:Disable, 1:Enable 2:Enable with matching altname[] too) */


#define FF_USE_MKFS		1 // 使能或禁用f_mkfs函数
/* This option switches f_mkfs() function. (0:Disable or 1:Enable) */


#define FF_USE_FASTSEEK	0 // 使能或禁用快速搜索功能  使能后，可以加快f_lseek、f_read和f_write函数执行
/* This option switches fast seek function. (0:Disable or 1:Enable) */


#define FF_USE_EXPAND	0 // 使用或禁用 为文件分配连续数据区域
/* This option switches f_expand function. (0:Disable or 1:Enable) */


#define FF_USE_CHMOD	0 // 使能或禁用元数据控制函数
/* This option switches attribute manipulation functions, f_chmod() and f_utime().
/  (0:Disable or 1:Enable) Also FF_FS_READONLY needs to be 0 to enable this option. */


#define FF_USE_LABEL	0 // 使能或禁用卷标签API函数：f_getlabel和f_setlabel。
/* This option switches volume label functions, f_getlabel() and f_setlabel().
/  (0:Disable or 1:Enable) */


#define FF_USE_FORWARD	0 // 使能或禁用f_forward函数
/* This option switches f_forward() function. (0:Disable or 1:Enable) */


#define FF_USE_STRFUNC	0 // 使能禁止 字符串函数
#define FF_PRINT_LLI	1
#define FF_PRINT_FLOAT	1
#define FF_STRF_ENCODE	3 // 编码 3 为UTF-8
/* FF_USE_STRFUNC switches string functions, f_gets(), f_putc(), f_puts() and
/  f_printf().
/
/   0: Disable. FF_PRINT_LLI, FF_PRINT_FLOAT and FF_STRF_ENCODE have no effect.
/   1: Enable without LF-CRLF conversion.
/   2: Enable with LF-CRLF conversion.
/
/  FF_PRINT_LLI = 1 makes f_printf() support long long argument and FF_PRINT_FLOAT = 1/2
/  makes f_printf() support floating point argument. These features want C99 or later.
/  When FF_LFN_UNICODE >= 1 with LFN enabled, string functions convert the character
/  encoding in it. FF_STRF_ENCODE selects assumption of character encoding ON THE FILE
/  to be read/written via those functions.
/
/   0: ANSI/OEM in current CP
/   1: Unicode in UTF-16LE
/   2: Unicode in UTF-16BE
/   3: Unicode in UTF-8
*/


/*---------------------------------------------------------------------------/
/ Locale and Namespace Configurations
/---------------------------------------------------------------------------*/

#define FF_CODE_PAGE	932 // 规定目标系统使用的OEM代码
/* This option specifies the OEM code page to be used on the target system.
/  Incorrect code page setting can cause a file open failure.
/
/   437 - U.S.
/   720 - Arabic
/   737 - Greek
/   771 - KBL
/   775 - Baltic
/   850 - Latin 1
/   852 - Latin 2
/   855 - Cyrillic
/   857 - Turkish
/   860 - Portuguese
/   861 - Icelandic
/   862 - Hebrew
/   863 - Canadian French
/   864 - Arabic
/   865 - Nordic
/   866 - Russian
/   869 - Greek 2
/   932 - Japanese (DBCS)
/   936 - Simplified Chinese (DBCS)
/   949 - Korean (DBCS)
/   950 - Traditional Chinese (DBCS)
/     0 - Include all code pages above and configured by f_setcp()
*/


#define FF_USE_LFN		2 // 使能或禁用长文件名(LFN),禁止 静态空间 栈空间 堆空间
#define FF_MAX_LFN		255 // 最长文件名
/* The FF_USE_LFN switches the support for LFN (long file name).
/
/   0: Disable LFN. FF_MAX_LFN has no effect.
/   1: Enable LFN with static  working buffer on the BSS. Always NOT thread-safe.
/   2: Enable LFN with dynamic working buffer on the STACK.
/   3: Enable LFN with dynamic working buffer on the HEAP.
/
/  To enable the LFN, ffunicode.c needs to be added to the project. The LFN function
/  requiers certain internal working buffer occupies (FF_MAX_LFN + 1) * 2 bytes and
/  additional (FF_MAX_LFN + 44) / 15 * 32 bytes when exFAT is enabled.
/  The FF_MAX_LFN defines size of the working buffer in UTF-16 code unit and it can
/  be in range of 12 to 255. It is recommended to be set it 255 to fully support LFN
/  specification.
/  When use stack for the working buffer, take care on stack overflow. When use heap
/  memory for the working buffer, memory management functions, ff_memalloc() and
/  ff_memfree() exemplified in ffsystem.c, need to be added to the project. */


#define FF_LFN_UNICODE	2
/* This option switches the character encoding on the API when LFN is enabled.
/
/   0: ANSI/OEM in current CP (TCHAR = char)
/   1: Unicode in UTF-16 (TCHAR = WCHAR)
/   2: Unicode in UTF-8 (TCHAR = char)
/   3: Unicode in UTF-32 (TCHAR = DWORD)
/
/  Also behavior of string I/O functions will be affected by this option.
/  When LFN is not enabled, this option has no effect. */


#define FF_LFN_BUF		255
#define FF_SFN_BUF		12
/* This set of options defines size of file name members in the FILINFO structure
/  which is used to read out directory items. These values should be suffcient for
/  the file names to read. The maximum possible length of the read file name depends
/  on character encoding. When LFN is not enabled, these options have no effect. */


#define FF_FS_RPATH		0 // 配置相对路径函数
/* This option configures support for relative path.
/
/   0: Disable relative path and remove related functions.
/   1: Enable relative path. f_chdir() and f_chdrive() are available.
/   2: f_getcwd() function is available in addition to 1.
*/


/*---------------------------------------------------------------------------/
/ Drive/Volume Configurations
/---------------------------------------------------------------------------*/

#define FF_VOLUMES		1 // 可用卷数目
/* Number of volumes (logical drives) to be used. (1-10) */


#define FF_STR_VOLUME_ID	0 // 是能禁用字符卷标识
#define FF_VOLUME_STRS		"RAM","NAND","CF","SD","SD2","USB","USB2","USB3" // 定义每一个逻辑驱动器ID。ID数目不能少于_VOLUMES。驱动器ID有效字符只能为A-Z和0-9。比如：
/* FF_STR_VOLUME_ID switches support for volume ID in arbitrary strings.
/  When FF_STR_VOLUME_ID is set to 1 or 2, arbitrary strings can be used as drive
/  number in the path name. FF_VOLUME_STRS defines the volume ID strings for each
/  logical drives. Number of items must not be less than FF_VOLUMES. Valid
/  characters for the volume ID strings are A-Z, a-z and 0-9, however, they are
/  compared in case-insensitive. If FF_STR_VOLUME_ID >= 1 and FF_VOLUME_STRS is
/  not defined, a user defined volume string table is needed as:
/
/  const char* VolumeStr[FF_VOLUMES] = {"ram","flash","sd","usb",...
*/


#define FF_MULTI_PARTITION	0 // 使能或禁止多分区函数
/* This option switches support for multiple volumes on the physical drive.
/  By default (0), each logical drive number is bound to the same physical drive
/  number and only an FAT volume found on the physical drive will be mounted.
/  When this function is enabled (1), each logical drive number can be bound to
/  arbitrary physical drive and partition listed in the VolToPart[]. Also f_fdisk()
/  function will be available. */


#define FF_MIN_SS		512 // _MIN_SS、_MAX_SS 扇区大小
#define FF_MAX_SS		512
/* This set of options configures the range of sector size to be supported. (512,
/  1024, 2048 or 4096) Always set both 512 for most systems, generic memory card and
/  harddisk, but a larger value may be required for on-board flash memory and some
/  type of optical media. When FF_MAX_SS is larger than FF_MIN_SS, FatFs is configured
/  for variable sector size mode and disk_ioctl() function needs to implement
/  GET_SECTOR_SIZE command. */


#define FF_LBA64		0
/* This option switches support for 64-bit LBA. (0:Disable or 1:Enable)
/  To enable the 64-bit LBA, also exFAT needs to be enabled. (FF_FS_EXFAT == 1) */


#define FF_MIN_GPT		0x10000000
/* Minimum number of sectors to switch GPT as partitioning format in f_mkfs and
/  f_fdisk function. 0x100000000 max. This option has no effect when FF_LBA64 == 0. */


#define FF_USE_TRIM		0 // 使能或禁用空闲簇计数和最后分配的簇计数。
/* This option switches support for ATA-TRIM. (0:Disable or 1:Enable)
/  To enable Trim function, also CTRL_TRIM command should be implemented to the
/  disk_ioctl() function. */



/*---------------------------------------------------------------------------/
/ System Configurations
/---------------------------------------------------------------------------*/

#define FF_FS_TINY		0 
/*
配置FatFs为正常模式或者微型(TINY)模式。配置为微型模式后，对内存需求变小，
文件对象数据结构FIL会减少_MAX_SS字节。程序复用FATFS数据结构中的缓冲区代替FIL数据结构中去除掉的缓冲区
*/
/* This option switches tiny buffer configuration. (0:Normal or 1:Tiny)
/  At the tiny configuration, size of file object (FIL) is shrinked FF_MAX_SS bytes.
/  Instead of private sector buffer eliminated from the file object, common sector
/  buffer in the filesystem object (FATFS) is used for the file data transfer. */


#define FF_FS_EXFAT		1
/*
使能或禁用exFAT文件系统。要使能exFAT文件系统，必须使能长文件名功能并且配置_LFN_UNICODE = 1。
如果要使用全功能版的exFAT，推荐_MAX_LFN = 255。注意，使能exFAT意味着不再兼容C89，因为要用到64位整形数。
*/
/* This option switches support for exFAT filesystem. (0:Disable or 1:Enable)
/  To enable exFAT, also LFN needs to be enabled. (FF_USE_LFN >= 1)
/  Note that enabling exFAT discards ANSI C (C89) compatibility. */

/*
使能或禁用时间戳函数。使能时间戳函数需要硬件RTC，并且需要提供底层函数get_fattime。
如果系统没有硬件RTC或者不需要时间戳功能，设置_FS_NORTC为1禁用时间戳函数。
此时，如果FatFs修改任何文件，使用固定的时间戳，固定时间戳由宏_NORTC_MON、_NORTC_MDAY和_NORTC_YEAR定义。
*/
#define FF_FS_NORTC		0
/* 如果系统没有RTC，这些宏用来定义固定时间戳。只读或者_FS_NORTC=0时，这些宏无意义 */
#define FF_NORTC_MON	1
#define FF_NORTC_MDAY	1
#define FF_NORTC_YEAR	2022
/* The option FF_FS_NORTC switches timestamp feature. If the system does not have
/  an RTC or valid timestamp is not needed, set FF_FS_NORTC = 1 to disable the
/  timestamp feature. Every object modified by FatFs will have a fixed timestamp
/  defined by FF_NORTC_MON, FF_NORTC_MDAY and FF_NORTC_YEAR in local time.
/  To enable timestamp function (FF_FS_NORTC = 0), get_fattime() function need to be
/  added to the project to read current time form real-time clock. FF_NORTC_MON,
/  FF_NORTC_MDAY and FF_NORTC_YEAR have no effect.
/  These options have no effect in read-only configuration (FF_FS_READONLY = 1). */


#define FF_FS_NOFSINFO	0
/* If you need to know correct free space on the FAT32 volume, set bit 0 of this
/  option, and f_getfree() function at the first time after volume mount will force
/  a full FAT scan. Bit 1 controls the use of last allocated cluster number.
/
/  bit0=0: Use free cluster count in the FSINFO if available.
/  bit0=1: Do not trust free cluster count in the FSINFO.
/  bit1=0: Use last allocated cluster number in the FSINFO if available.
/  bit1=1: Do not trust last allocated cluster number in the FSINFO.
*/


/* 使能或禁用文件锁功能。控制重复打开文件和非法打开文件对象。注意：文件锁功能不具有可重入性。只读模式下，这个宏必须为0。 */
#define FF_FS_LOCK		0
/* The option FF_FS_LOCK switches file lock function to control duplicated file open
/  and illegal operation to open objects. This option must be 0 when FF_FS_READONLY
/  is 1.
/
/  0:  Disable file lock function. To avoid volume corruption, application program
/      should avoid illegal open, remove and rename to the open objects.
/  >0: Enable file lock function. The value defines how many files/sub-directories
/      can be opened simultaneously under file lock control. Note that the file
/      lock control is independent of re-entrancy. */

/* 使能或禁用FatFs模块的可重入特性。注意，访问不同卷上的文件/目录总是可重入的，
无论是否使能本参数，卷控制函数f_mount、f_mkfs和f_fdisk总是不可重入的。
要使能可重入特性，用户必须提供同步处理，要向工程中添加ff_req_grant、ff_rel_grant、ff_del_syncobj和ff_cre_syncobj函数。
可以在文件option/syscall.c中找到示例。
*/
#define FF_FS_REENTRANT	0
/* 设置超时时间，单位为系统时钟滴答周期，当宏_FS_REENTRANT=0时，本设置无效。 */
#define FF_FS_TIMEOUT	1000
/* The option FF_FS_REENTRANT switches the re-entrancy (thread safe) of the FatFs
/  module itself. Note that regardless of this option, file access to different
/  volume is always re-entrant and volume control functions, f_mount(), f_mkfs()
/  and f_fdisk() function, are always not re-entrant. Only file/directory access
/  to the same volume is under control of this featuer.
/
/   0: Disable re-entrancy. FF_FS_TIMEOUT have no effect.
/   1: Enable re-entrancy. Also user provided synchronization handlers,
/      ff_mutex_create(), ff_mutex_delete(), ff_mutex_take() and ff_mutex_give()
/      function, must be added to the project. Samples are available in ffsystem.c.
/
/  The FF_FS_TIMEOUT defines timeout period in unit of O/S time tick.
*/



/*--- End of configuration options ---*/
