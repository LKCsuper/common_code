#ifndef __SOFTALG_SHA512_H__
#define __SOFTALG_SHA512_H__

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(__i386) || defined(__i386__) || defined(_M_IX86) || defined(__x86_64) || defined(_M_AMD64) ||              \
  defined(_M_X64) || defined(__s390__) || defined(__s390x__) || defined(__aarch64__) || defined(SHA512_ASM)
    #define SHA512_BLOCK_CAN_MANAGE_UNALIGNED_DATA
#endif

#if (defined(_WIN32) || defined(_WIN64)) && !defined(__MINGW32__)
    #define U64(C) C##UI64
#elif defined(__arch64__)
    #define U64(C) C##UL
#else
    #define U64(C) C##ULL
#endif

#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__)
    #define DECLARE_IS_ENDIAN const int ossl_is_little_endian = __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define IS_LITTLE_ENDIAN  (ossl_is_little_endian)
    #define IS_BIG_ENDIAN     (!ossl_is_little_endian)
    #if defined(L_ENDIAN) && (__BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__)
        #error "L_ENDIAN defined on a big endian machine"
    #endif
    #if defined(B_ENDIAN) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
        #error "B_ENDIAN defined on a little endian machine"
    #endif
    #if !defined(L_ENDIAN) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
        #define L_ENDIAN
    #endif
    #if !defined(B_ENDIAN) && (__BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__)
        #define B_ENDIAN
    #endif
#else
    #define DECLARE_IS_ENDIAN                                                                                          \
        const union                                                                                                    \
        {                                                                                                              \
            long one;                                                                                                  \
            char little;                                                                                               \
        } ossl_is_endian = { 1 }

    #define IS_LITTLE_ENDIAN (ossl_is_endian.little != 0)
    #define IS_BIG_ENDIAN    (ossl_is_endian.little == 0)
#endif

#define SHA_LONG   unsigned int
#define SHA_LONG64 unsigned long long

#define SHA_LBLOCK 16
#define SHA_CBLOCK (SHA_LBLOCK * 4)

#define SHA512_DIGEST_LENGTH 64

#define SHA512_CBLOCK (SHA_LBLOCK * 8)

    typedef struct SHA512state_st
    {
        SHA_LONG64 h[8];
        SHA_LONG64 Nl, Nh;
        union
        {
            SHA_LONG64    d[SHA_LBLOCK];
            unsigned char p[SHA512_CBLOCK];
        } u;
        unsigned int num, md_len;
    } SHA512_CTX;

    int            SHA512_Init(SHA512_CTX *c);
    int            SHA512_Update(SHA512_CTX *c, const void *data, size_t len);
    int            SHA512_Final(unsigned char *md, SHA512_CTX *c);
    unsigned char *SHA512(const unsigned char *d, size_t n, unsigned char *md);
    void           SHA512_Transform(SHA512_CTX *c, const unsigned char *data);

#ifdef __cplusplus
}
#endif

#endif
