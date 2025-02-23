//
//  sm9_utils.c
//  SM9_FREE
//
#include "sm9_utils.h"
#include "sm3.h"
#include <stdlib.h>
#include <string.h>

const unsigned char hfc[2] = {0x01, 0x02};
sm9_ctx *sm9Ctx;

miracl *GenMiracl(int secLevel)
{
    if (!sm9init) {
        return NULL;
    }

#ifdef MR_OS_THREADS
    miracl *mr_mip = get_mip();
#endif
    mr_mip = mirsys(128, 0);
    mr_mip->TWIST = TWIST;

    ecurve_init(_MIPP_ sm9a, sm9b, sm9q, MR_PROJECTIVE);

    return mr_mip;
}

void CloseMiracl(miracl *mr_mip)
{
    if (mr_mip) {
        mirexit(_MIPPO_);
    }
}

void CloseMirCtx(mir_ctx *mirCtx)
{
    mirkill(mirCtx->mcs_sm9t);
    zzn2_kill(_MIPP_ & (mirCtx->mcs_sm9X));
    epoint_free(mirCtx->mcs_p1G1);
    epoint_free(mirCtx->mcs_ppG1);
    epoint_free(mirCtx->mcs_keG1);
    epoint_free(mirCtx->mcs_cpG1);
    ecn2_kill(_MIPP_ & (mirCtx->mcs_p2G2));
    ecn2_kill(_MIPP_ & (mirCtx->mcs_ppG2));
    zzn12_kill(_MIPP_ & (mirCtx->mcs_gGt));
    zzn12_kill(_MIPP_ & (mirCtx->mcs_eGt));
    zzn12_kill(_MIPP_ & (mirCtx->mcs_kGt));
    zzn12_kill(_MIPP_ & (mirCtx->mcs_cGt));

    if (mirCtx) {
        mr_free(mirCtx);
    }
}

void CloseSm9Ctx(sm9_ctx *sm9Ctx)
{
    if (sm9Ctx) {
        mr_free(sm9Ctx);
    }
}

sm9_ctx *sm9sys()
{
    sm9Ctx = (sm9_ctx *)calloc(1, sizeof(sm9_ctx));

    return sm9Ctx;
}

int ceilfunc(double v)
{
    int vi;
    vi = (int)v;
    if ((v - vi) > 0) {
        vi += 1;
    }

    return vi;
}

int floorfunc(double v)
{
    int vi;
    vi = (int)v;

    return vi;
}

int inverseUcBuf(unsigned char *H, int HLen)
{
    unsigned char tmpChar;
    int i, loop;

    loop = HLen / 2;

    for (i = 0; i < loop; i++) {
        tmpChar = H[i];
        H[i] = H[HLen - 1 - i];
        H[HLen - 1 - i] = tmpChar;
    }

    return 0;
}

int integer_to_bytes(int bitLen, unsigned char *xbuf, int xbufLen)
{
    unsigned char intBuf[4], *ptmpBuf;

    ptmpBuf = (unsigned char *)&bitLen;

    memcpy(intBuf, ptmpBuf, sizeof(int));
    memcpy(xbuf, intBuf, xbufLen > sizeof(int) ? sizeof(int) : xbufLen);
#ifdef MR_LITTLE_ENDIAN
    inverseUcBuf(xbuf, xbufLen);
#endif

#ifdef MR_BIG_ENDIAN
#endif

    return 0;
}

// h function. h=1: h1; h=2:h2
big Hfun(_MIPD_ char *zbuf, int zbufLen, int secLevel, int h)
{
    int ct = 0x00000001;
    int v = SM3_HASH_LEN * 8;
    int hlentmp = ceilfunc((double)secLevel * 5 / 4) * 8;
    int hlentmp2;
    int loopCount = ceilfunc((double)hlentmp / v);
    int i, HaLen, HaFLen;
    char kbuf[50];
    big t, N;
    SM3_CTX sm3;
    unsigned char s[SM3_HASH_LEN], ctBuf[4];

    t = mirvar(_MIPP_ 0);
    N = mirvar(_MIPP_ 0);

    HaLen = 0;
    for (i = 0; i < loopCount; i++) {
        SM3Init(&sm3);
        SM3Update(&sm3, (unsigned char *)hfc + (h - 1), 1);
        SM3Update(&sm3, (unsigned char *)zbuf, zbufLen);
        integer_to_bytes(ct, ctBuf, sizeof(ctBuf));
        SM3Update(&sm3, ctBuf, sizeof(ct));
        SM3Final(s, &sm3);
        if (i == loopCount - 1) {
            if (loopCount * v == hlentmp) {
                memcpy(kbuf + HaLen, s, SM3_HASH_LEN);
                HaLen += SM3_HASH_LEN;
            } else {
                hlentmp2 = floorfunc((double)hlentmp / v);

                HaFLen = (hlentmp - v * (int)hlentmp2) / 8;
                memcpy(kbuf + HaLen, s, HaFLen);
                HaLen += HaFLen;
            }
        } else {
            memcpy(kbuf + HaLen, s, SM3_HASH_LEN);
            HaLen += SM3_HASH_LEN;
        }
        ct++;
    }

    bytes_to_big(_MIPP_ HaLen, (char *)kbuf, t);
    decr(_MIPP_ sm9n, 1, N);
    divide(_MIPP_ t, N, N);
    incr(_MIPP_ t, 1, t);
    mirkill(N);

    return t;
}

int KDF(char *zbuf, int zbufLen, int klen, char *kbuf)
{
    int ct = 0x00000001;
    int v = SM3_HASH_LEN * 8;
    int hlentmp = ceilfunc((double)klen / v);
    int loopCount = (int)hlentmp;
    int i, HaLen, HaFLen;
    SM3_CTX sm3;
    unsigned char s[SM3_HASH_LEN], ctBuf[4];

    HaLen = 0;
    for (i = 0; i < loopCount; i++) {
        SM3Init(&sm3);

        SM3Update(&sm3, (unsigned char *)zbuf, zbufLen);

        integer_to_bytes(ct, ctBuf, sizeof(ctBuf));

        SM3Update(&sm3, ctBuf, sizeof(ct));

        SM3Final(s, &sm3);
        if (i == loopCount - 1) {
            if (loopCount * v == klen) {
                memcpy(kbuf + HaLen, s, SM3_HASH_LEN);
                HaLen += SM3_HASH_LEN;
            } else {
                hlentmp = floorfunc((double)klen / v);

                HaFLen = (klen - v * (int)hlentmp) / 8;
                memcpy(kbuf + HaLen, s, HaFLen);
                HaLen += HaFLen;
            }
        } else {
            memcpy(kbuf + HaLen, s, SM3_HASH_LEN);
            HaLen += SM3_HASH_LEN;
        }
        ct++;
    }

    return 0;
}

int MAC(unsigned char *key, unsigned int keylen, unsigned char *mes, unsigned int meslen, unsigned char *mac)
{
    SM3_CTX sm3;

    SM3Init(&sm3);
    SM3Update(&sm3, mes, meslen);
    SM3Update(&sm3, key, keylen);
    SM3Final(mac, &sm3);

    return 0;
}

int xorAlgor(unsigned char *bufIn, int ilen, unsigned char *bufKey, unsigned char *bufOut)
{
    int i;
    for (i = 0; i < ilen; i++) {
        bufOut[i] = bufIn[i] ^ bufKey[i];
    }
    return 0;
}
