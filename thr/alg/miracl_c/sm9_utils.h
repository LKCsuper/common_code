//
//  sm9_utils.h
//  SM9_FREE
//

#ifndef sm9_utils_h
#define sm9_utils_h

#include "miracl.h"
#include "smzzn12.h"

#define sm9t mirCtx->mcs_sm9t
#define sm9X mirCtx->mcs_sm9X
#define p1G1 mirCtx->mcs_p1G1
#define ppG1 mirCtx->mcs_ppG1
#define keG1 mirCtx->mcs_keG1
#define cpG1 mirCtx->mcs_cpG1
#define p2G2 mirCtx->mcs_p2G2
#define ppG2 mirCtx->mcs_ppG2
#define gGt  mirCtx->mcs_gGt
#define eGt  mirCtx->mcs_eGt
#define kGt  mirCtx->mcs_kGt
#define cGt  mirCtx->mcs_cGt

typedef epoint ecn;         // edit by andrew song
extern BOOL sm9init;        // sm9算法库状态
extern BOOL sm9sign;        // sm9签名模块状态
extern BOOL sm9encrypt;     // sm9加解密模块状态
extern BOOL sm9keyexchange; // sm9密钥协商模块状态
extern BOOL sm9encap;       // sm9封装解封模块状态

extern int sm9len; // sm9算法的安全参数，标准中是32（bytes）

extern big sm9q; // sm9椭圆曲线基域素数
extern big sm9a; // sm9椭圆曲线函数参数a
extern big sm9b; // sm9椭圆曲线函数参数b
extern big sm9n; // sm9基域上椭圆曲线的阶

extern big sm9t_Ex;  // sm9椭圆曲线复乘参数t
extern zzn2 sm9X_Ex; // sm9椭圆曲线pairing预计算参数

extern ecn* p1G1_Ex; // sm9椭圆曲线G1群生成元
extern ecn* ppG1_Ex; // sm9算法加密主公钥
extern ecn* keG1_Ex; // sm9算法密钥协商主公钥
extern ecn* cpG1_Ex; // sm9算法封装主公钥

extern ecn2 p2G2_Ex; // sm9椭圆曲线G2群生成元
extern ecn2 ppG2_Ex; // sm9算法签名主公钥

extern zzn12 gGt_Ex; // sm9算法签名模块预计算结果
extern zzn12 eGt_Ex; // sm9算法加解密模块预计算结果
extern zzn12 kGt_Ex; // sm9算法密钥协商模块预计算结果
extern zzn12 cGt_Ex; // sm9算法封装解封模块预计算结果

extern BOOL TWIST; // sm9算法TWIST类型，标准为MR_SEXTIC_M
const extern unsigned char hid[3]; // sm9算法标识，hid[0]表示签名算法，hid[1]表示密钥协商算法，hid[2]表示加解密算法

typedef struct sm9_ctx_st
{
    big sm9t_Ex;
    zzn2 sm9X_Ex;

    ecn* p1G1_Ex;
    ecn* ppG1_Ex;
    ecn* keG1_Ex;
    ecn* cpG1_Ex;
    // ecn p1G1_Ex;
    // ecn ppG1_Ex;
    // ecn keG1_Ex;
    // ecn cpG1_Ex;

    ecn2 p2G2_Ex;
    ecn2 ppG2_Ex;

    zzn12 gGt_Ex;
    zzn12 eGt_Ex;
    zzn12 kGt_Ex;
    zzn12 cGt_Ex;
}sm9_ctx;
extern sm9_ctx *sm9Ctx;

miracl *GenMiracl(int secLevel);  // 初始化一个mircal库
void CloseMiracl(miracl *mr_mip); // 关闭一个mircal库
void CloseMirCtx(mir_ctx *mirCtx);
void CloseSm9Ctx(sm9_ctx *sm9Ctx);
sm9_ctx *sm9sys(void);

// sm9算法中辅助算法H1、H2
//@zbuf 输入字符串
//@zbuflen 字符串长度
//@secLevel 安全参数
//@h 函数选择参数，当h=1时为函数H1，当h=2时为函数H2
// 输出一个大整数
big Hfun(_MIPD_ char *zbuf, int zbufLen, int secLevel, int h);

// sm9算法中kdf算法
//@zbuf 输入字符串
//@zbulen 字符串长度
//@klen 预期输出长度
//@kbuf 输出字符串
// 输出 0
int KDF(char *zbuf, int zbufLen, int klen, char *kbuf);

// 基于SM3的MAC算法
//@key 输入密钥
//@keylen 密钥长度
//@msg   消息
//@msglen 消息长度
//@mac 输出的mac值
// 输出 0
int MAC(unsigned char *key, unsigned int keylen, unsigned char *msg, unsigned int msglen, unsigned char *mac);

// XOR加密函数
//@bufIn 输入字符串
//@ilen  输入字符串长度
//@bufKey XOR加密密钥
//@bufOut 加密结果
// 输出 0
int xorAlgor(unsigned char *bufIn, int ilen, unsigned char *bufKey, unsigned char *bufOut);

BOOL ecap(_MIPD_ ecn2 *P, ecn *Q, big x, zzn2 *X, zzn12 *r);
void set_frobenius_constant(_MIPD_ zzn2 *x);
#endif /* sm9_parameters_h */
