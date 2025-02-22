/*
 * @Description:
 * @Version: 2.0
 * @Date: 2023-06-30 21:12:08
 * @LastEditors: lkc
 * @detail:
 */
#ifndef _COMMON_GPS_H
#define _COMMON_GPS_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* 可根据实际的数据修改ID */
#define PRE_GGA   "$GNGGA" /* GPS 定位信息 */
#define PRE_GLL   "$GNGLL" /* 地理定位信息 */
#define PRE_GSA   "$GNGSA" /* 当前卫星信息 精度指针及使用卫星 */
#define PRE_GPGSV "$GPGSV" /* 可见gps卫星信息 可视卫星状态输出语句 */
#define PRE_VTG   "$GNVTG" /* 地面速度信息 */
#define PRE_RMC   "$GPRMC" /* 推荐最小定位信息 */
#define PRE_GNGSV "$GNGSV"
#define PRE_GLGSV "$GLGSV"
#define PRE_ZDA   "$GNZDA" /* 当前时间信息 */
#define PRE_GBGSV "$GBGSV" /* 可见北斗卫星信息 可视卫星状态输出语句 */
#define PRE_GNTXT "$GNTXT"
#define PRE_HEAD  "#HEAD"

/* 语句解析控制 1：使能，0：禁用 */
#define ENABLE_GGA  0
#define ENABLE_GLL  0
#define ENABLE_GSA  0
#define ENABLE_GSV  0
#define ENABLE_RMC  1
#define ENABLE_VTG  0
#define ENABLE_UTC  0
#define ENABLE_HEAD 1
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if ENABLE_RMC
/* RMC数据结构体（推荐定位信息数据格式）*/
typedef struct tagRMC {
    CHAR cUtcTim[11];  // UTC时间，hhmmss.sss格式
    UCHAR cIsPos;      // 状态，A=定位，V=未定位
    double dLat;       // 纬度ddmm.mmmm，度分格式
    CHAR cLatDir;      // 纬度N（北纬）或S（南纬）
    double dLon;       // 经度dddmm.mmmm，度分格式
    CHAR cLonDir;      // 经度E（东经）或W（西经）
    double dSpeedKn;   // 速度
    double dTrackTrue; // 方位角，度
    CHAR cDate[7];     // UTC日期，DDMMYY格式
    double dMagVar;    // 磁偏角，（000 - 180）度
    CHAR cVarDir;      // 磁偏角方向，E=东W=西
    CHAR cModeInd;     // 模式，A=自动，D=差分，E=估测，N=数据无效（3.0协议内容）

    UCHAR ucFlag; // 是否解析到标志

    uint8_t Hour;  /* UTC 时间?? hhmmss.sss */
    uint8_t Min;   /* ?? */
    uint8_t Sec;   /* ?? */
    uint16_t mSec; /* ????,注意有的不是??秒而是ss */
    uint16_t Year; /* 日期 120598 ddmmyy */
    uint8_t Month;
    uint8_t Day;
} RMC_S;
#endif

#if ENABLE_GGA
/* RMC数据结构体（推荐定位信息数据格式???*/
typedef struct tagGGA {
    UCHAR ucFlag; // 是否解析到标志

    CHAR cUtcTim[11]; // UTC时间，hhmmss.sss格式
    uint8_t Hour;     /* UTC 时间 */
    uint8_t Min;
    uint8_t Sec;

    double dLat;
    char cLatDir;
    double dLon;
    char cLonDir;

    char quality; /* 质量 */

    char staNum; /* 卫星数 */

    float hdop;       /* 水平因子 */
    float alt;        /* 高度海拔 */
    char aunits;      /* 天线高度单位 */
    float undulation; /* 大地水准面差距 */
    char uunits;      /* 大地水准面差距单位 */

    uint32_t age; /* 差分数据龄 */
    uint32_t id;  /* 差分基站id */

    uint16_t mSec; /* ????,注意有的不是??秒而是ss */
    uint16_t Year; /* 日期 120598 ddmmyy */
    uint8_t Month;
    uint8_t Day;
} GGA_S;
#endif

#if ENABLE_HEAD
/* RMC数据结构体（推荐定位信息数据格式???*/
typedef struct tagHEAD {
    UCHAR ucFlag;

    char posType[32]; /* 位置类型 NARROW_INT 窄巷固定解 最好 */
    float length;     /* 基准线长 */
    float heading;    /* 航向 */
} HEAD_S;
#endif
/* 定位模组信息结构体集??? */
typedef struct tagGpsAllInfo {
#if ENABLE_RMC /* 最小定位信息 */
    RMC_S stRmc;
#endif

#if ENABLE_GGA /* rtk 定位 */
    GGA_S stGga;
#endif

#if ENABLE_HEAD /* 定向 */
    HEAD_S stHead;
#endif
} GPS_ALL_INFO_S;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
RMC_S Common_Gps_RmcPrase(CHAR *pucData);
GGA_S Common_Gps_GgaPrase(CHAR *pucData);
HEAD_S Common_Head_GgaPrase(CHAR *pucData);
/* GPS_ALL_INFO_S Common_Gps_Prase(CHAR *pucData); */
VOID Common_Gps_Prase(CHAR *pcData, GPS_ALL_INFO_S *pstGpsInfo);
#ifdef __cplusplus
}
#endif

#endif
