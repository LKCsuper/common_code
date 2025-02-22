/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @detail: 
 */
#ifndef __APP_VERSION_H__
#define __APP_VERSION_H__

#define HARDWAVE_VERSION "v1.0.0"

// NOTE 主版本、子版本、bug修复版本
#define SOFTWAVE_MAIN_VERSION (1)
#define SOFTWAVE_SUB_VERSION  (0)
#define SOFTWAVE_FIX_VERSION  (0)

#define STRINGIFY(x) #x
#define TOSTRING(x)  STRINGIFY(x)
#define SOFTWAVE_VERSION                                                                           \
    "v" TOSTRING(SOFTWAVE_MAIN_VERSION) "." TOSTRING(SOFTWAVE_SUB_VERSION) "." TOSTRING(           \
            SOFTWAVE_FIX_VERSION)

/* 设备版本 */
#define VERSION_CODE(v1, v2, v3) ((v1) * 256 * 256 + (v2) * 256 + v3)
#endif
