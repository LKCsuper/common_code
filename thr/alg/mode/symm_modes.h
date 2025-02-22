#ifndef __SYMM_MODES_H__
#define __SYMM_MODES_H__

#include <stddef.h>
#include <stdint.h>

#define IS_NATIONAL_SM4_ALGO      1
#define IS_INTERNATIONAL_AES_ALGO 0

#ifdef TABLE_BITS
    #undef TABLE_BITS
#endif

#define TABLE_BITS 4

#if defined(__arch64__)
typedef long i64;
typedef unsigned long u64;
    #define U64(C) C##UL
#else
typedef long long i64;
typedef unsigned long long u64;
    #define U64(C) C##ULL
#endif

typedef unsigned char u8;
typedef unsigned int u32;

#define OPENSSL_DECLARE_IS_ENDIAN                                                                                      \
    const union {                                                                                                      \
        long one;                                                                                                      \
        char little;                                                                                                   \
    } open_ssl_is_endian = {1}
#define OPENSSL_IS_LITTLE_ENDIAN (open_ssl_is_endian.little != 0)
#define OPENSSL_BIG_ENDIAN       (open_ssl_is_endian.little == 0)

#if !defined(PEDANTIC) && !defined(OPENSSL_NO_ASM) && !defined(OPENSSL_NO_INLINE_ASM)
    #if defined(__GNUC__) && __GNUC__ >= 2
        #if defined(__x86_64) || defined(__x86_64__)
            #define BSWAP8(x)                                                                                          \
                ({                                                                                                     \
                    u64 ret_ = (x);                                                                                    \
                    asm("bswapq %0" : "+r"(ret_));                                                                     \
                    ret_;                                                                                              \
                })
            #define BSWAP4(x)                                                                                          \
                ({                                                                                                     \
                    u32 ret_ = (x);                                                                                    \
                    asm("bswapl %0" : "+r"(ret_));                                                                     \
                    ret_;                                                                                              \
                })
        #elif (defined(__i386) || defined(__i386__)) && !defined(I386_ONLY)
            #define BSWAP8(x)                                                                                          \
                ({                                                                                                     \
                    u32 lo_ = (u64)(x) >> 32, hi_ = (x);                                                               \
                    asm("bswapl %0; bswapl %1" : "+r"(hi_), "+r"(lo_));                                                \
                    (u64) hi_ << 32 | lo_;                                                                             \
                })
            #define BSWAP4(x)                                                                                          \
                ({                                                                                                     \
                    u32 ret_ = (x);                                                                                    \
                    asm("bswapl %0" : "+r"(ret_));                                                                     \
                    ret_;                                                                                              \
                })
        #elif defined(__aarch64__)
            #define BSWAP8(x)                                                                                          \
                ({                                                                                                     \
                    u64 ret_;                                                                                          \
                    asm("rev %0,%1" : "=r"(ret_) : "r"(x));                                                            \
                    ret_;                                                                                              \
                })
            #define BSWAP4(x)                                                                                          \
                ({                                                                                                     \
                    u32 ret_;                                                                                          \
                    asm("rev %w0,%w1" : "=r"(ret_) : "r"(x));                                                          \
                    ret_;                                                                                              \
                })
        #elif (defined(__arm__) || defined(__arm)) && !defined(STRICT_ALIGNMENT)
            #define BSWAP8(x)                                                                                          \
                ({                                                                                                     \
                    u32 lo_ = (u64)(x) >> 32, hi_ = (x);                                                               \
                    asm("rev %0,%0; rev %1,%1" : "+r"(hi_), "+r"(lo_));                                                \
                    (u64) hi_ << 32 | lo_;                                                                             \
                })
            #define BSWAP4(x)                                                                                          \
                ({                                                                                                     \
                    u32 ret_;                                                                                          \
                    asm("rev %0,%1" : "=r"(ret_) : "r"((u32)(x)));                                                     \
                    ret_;                                                                                              \
                })
        #endif
    #elif defined(_MSC_VER)
        #if _MSC_VER >= 1300
            #pragma intrinsic(_byteswap_uint64, _byteswap_ulong)
            #define BSWAP8(x) _byteswap_uint64((u64)(x))
            #define BSWAP4(x) _byteswap_ulong((u32)(x))
        #elif defined(_M_IX86)
__inline u32 _bswap4(u32 val)
{
    _asm mov eax, val _asm bswap eax
}
            #define BSWAP4(x) _bswap4(x)
        #endif
    #endif
#endif

#if defined(BSWAP4) && !defined(STRICT_ALIGNMENT)
    #define GETU32(p)    BSWAP4(*(const u32 *)(p))
    #define PUTU32(p, v) *(u32 *)(p) = BSWAP4(v)
#else
    #define GETU32(p)    ((u32)(p)[0] << 24 | (u32)(p)[1] << 16 | (u32)(p)[2] << 8 | (u32)(p)[3])
    #define PUTU32(p, v) ((p)[0] = (u8)((v) >> 24), (p)[1] = (u8)((v) >> 16), (p)[2] = (u8)((v) >> 8), (p)[3] = (u8)(v))
#endif

/* ------------------------------------------------------------------------------------------------------- */
typedef struct ccm128_context  CCM128_CONTEXT;
typedef struct gcm128_context  GCM128_CONTEXT;
typedef struct xts128_context  XTS128_CONTEXT;
typedef struct hctr128_context HCTR128_CONTEXT;

typedef int (*block128_f)(void *hdl, unsigned int uiAlgID, unsigned char *ucaKey, unsigned int keylen,
                          unsigned char *in, unsigned int in_len, unsigned char *out);

/* ---------------------------------------------- CCM GCM --------------------------------------------------------- */
struct ccm128_context {
    union {
        u64 u[2];
        u8 c[16];
    } nonce, cmac;
    u64 blocks;
    block128_f block;
    void *key;
};

typedef struct {
    u64 hi, lo;
} u128;

struct gcm128_context {
    union {
        u64 u[2];
        u32 d[4];
        u8 c[16];
        size_t t[16 / sizeof(size_t)];
    } Yi, EKi, EK0, len, Xi, H;
#if TABLE_BITS == 8
    u128 Htable[256];
#else
    u128 Htable[16];
    void (*gmult)(u64 Xi[2], const u128 Htable[16]);
    void (*ghash)(u64 Xi[2], const u128 Htable[16], const u8 *inp, size_t len);
#endif
    unsigned int mres, ares;
    block128_f block;
    void *key;
};

/* ------------------------------------------------- XTS ------------------------------------------------------ */
struct xts128_context {
    void *key1, *key2;
    u32 key1_len, key2_len;
    block128_f block1, block2;
};

/* -------------------------------------------------- HCTR ----------------------------------------------------- */
#define SYMM_SOFT_ALGO_SM4_BLOCK_SIZE 16

typedef struct hctrFieldElement_st {
	uint64_t low, high;
}hctrFieldElement;

typedef struct hctr_ctx_st {
    u8 tweak[SYMM_SOFT_ALGO_SM4_BLOCK_SIZE];
    u32 tweakLen;
    hctrFieldElement productTable[SYMM_SOFT_ALGO_SM4_BLOCK_SIZE];
}hctr_ctx;

struct hctr128_context {
    uint8_t *encKey, *hashKey;
    u32 encKeyLen, hashKeyLen;
    block128_f block_enc;
    block128_f block_dec;
};

/* --------------------------------------------------- EAX ---------------------------------------------------- */
#define SYMM_SOFT_ALGO_EAX_MODE_KEY_LEN 16
#define SYMM_SOFT_ALGO_TYPE_SM4 1
#define SYMM_SOFT_ALGO_TYPE_AES 0

typedef struct {
    uint8_t key[32];
    uint32_t key_len;
    uint32_t algo;
} eax128_cipher_t;

typedef union {
    uint64_t q[2];
    uint32_t w[4];
    uint8_t b[16];
} eax128_block_t;

typedef struct {
    void *cipher_ctx;
    eax128_block_t mac;
    eax128_block_t block;
    unsigned int bytepos;
} eax128_omac_t;

typedef struct {
    void *cipher_ctx;
    eax128_block_t nonce;
    eax128_block_t xorbuf;
    unsigned int blocknum;
} eax128_ctr_t;

typedef struct {
    eax128_omac_t domac;
    eax128_omac_t homac;
    eax128_ctr_t ctr;
} eax128_t;

/* ------------------------------------------------------------------------------------------------------- */
extern int CRYPTO_ctr128_encrypt(void *hdl, unsigned int uiAlgID, const unsigned char *pucaKey, unsigned int keylen,
                                 unsigned char counter[16], const unsigned char *in, size_t len, unsigned char *out,
                                 block128_f block);
extern int CRYPTO_ctr_encrypt(void *hdl, unsigned int uiAlgID, const unsigned char *pucaKey, unsigned int keylen,
                              unsigned char *counter, unsigned int counter_len, const unsigned char *in, size_t len,
                              unsigned char *out, block128_f block);

extern int CRYPTO_cbc_encrypt(void *hdl, unsigned int uiAlgID, const unsigned char *pucaKey, unsigned int keyLen,
                              unsigned char *pucIv, unsigned int uiIvLen, const unsigned char *in, size_t len,
                              unsigned char *out, block128_f block);
extern int CRYPTO_cbc_decrypt(void *hdl, unsigned int uiAlgID, const unsigned char *pucaKey, unsigned int keyLen,
                              unsigned char *pucIv, unsigned int uiIvLen, const unsigned char *in, size_t len,
                              unsigned char *out, block128_f block);

extern int CRYPTO_cfb_encrypt(void *hdl, unsigned int uiAlgID, const unsigned char *pucaKey, unsigned int keyLen,
                              unsigned char *pucIv, unsigned int uiIvLen, const unsigned char *in, size_t len,
                              unsigned char *out, block128_f block);
extern int CRYPTO_cfb_decrypt(void *hdl, unsigned int uiAlgID, const unsigned char *pucaKey, unsigned int keyLen,
                              unsigned char *pucIv, unsigned int uiIvLen, const unsigned char *in, size_t len,
                              unsigned char *out, block128_f block);

extern int CRYPTO_ofb_encrypt(void *hdl, unsigned int uiAlgID, const unsigned char *pucaKey, unsigned int keyLen,
                              unsigned char *pucIv, unsigned int uiIvLen, const unsigned char *in, size_t len,
                              unsigned char *out, block128_f block);

extern void CRYPTO_ccm128_init(CCM128_CONTEXT *ctx, unsigned int M, unsigned int L, void *key, block128_f block);
extern int CRYPTO_ccm128_setiv(CCM128_CONTEXT *ctx, const unsigned char *nonce, size_t nlen, size_t mlen);
extern void CRYPTO_ccm128_aad(void *hdl, unsigned int uiAlgID, CCM128_CONTEXT *ctx, const unsigned char *aad,
                              size_t alen);
extern int CRYPTO_ccm128_encrypt(void *hdl, unsigned int uiAlgID, CCM128_CONTEXT *ctx, const unsigned char *inp,
                                 unsigned char *out, size_t len);
extern int CRYPTO_ccm128_decrypt(void *hdl, unsigned int uiAlgID, CCM128_CONTEXT *ctx, const unsigned char *inp,
                                 unsigned char *out, size_t len);
extern size_t CRYPTO_ccm128_tag(CCM128_CONTEXT *ctx, unsigned char *tag, size_t len);

extern void CRYPTO_gcm128_init(void *hdl, unsigned int uiAlgID, GCM128_CONTEXT *ctx, void *key, block128_f block);
extern void CRYPTO_gcm128_setiv(void *hdl, unsigned int uiAlgID, GCM128_CONTEXT *ctx, const unsigned char *iv,
                                size_t len);
extern int CRYPTO_gcm128_aad(GCM128_CONTEXT *ctx, const unsigned char *aad, size_t len);
extern int CRYPTO_gcm128_encrypt(void *hdl, unsigned int uiAlgID, GCM128_CONTEXT *ctx, const unsigned char *in,
                                 unsigned char *out, size_t len);
extern int CRYPTO_gcm128_decrypt(void *hdl, unsigned int uiAlgID, GCM128_CONTEXT *ctx, const unsigned char *in,
                                 unsigned char *out, size_t len);
extern int CRYPTO_gcm128_finish(GCM128_CONTEXT *ctx, const unsigned char *tag, size_t len);
extern void CRYPTO_gcm128_tag(GCM128_CONTEXT *ctx, unsigned char *tag, size_t len);

extern int CRYPTO_xts128_encrypt(void *hdl, unsigned int uiAlgID, const XTS128_CONTEXT *ctx, const unsigned char iv[16],
                                 const unsigned char *inp, unsigned char *out, size_t len, int enc);
extern int CRYPTO_xts128_gb_encrypt(void *hdl, unsigned int uiAlgID, const XTS128_CONTEXT *ctx,
                                    const unsigned char iv[16], const unsigned char *inp, unsigned char *out,
                                    size_t len, int enc);
extern int CRYPTO_xts_encrypt(void *hdl, unsigned int uiAlgID, const unsigned char iv[16], const unsigned char *puckey,
                              unsigned int uikeylen, const unsigned char *tweakkey, unsigned int tweakkeylen,
                              const unsigned char *inp, unsigned char *out, size_t len, int enc, unsigned int isnation,
                              block128_f block1, block128_f block2);

extern int CRYPTO_bc_encrypt(void *hdl, unsigned int uiAlgID, const unsigned char *pucaKey, unsigned int keyLen,
                             unsigned char *pucIv, unsigned int uiIvLen, const unsigned char *in, size_t len,
                             unsigned char *out, block128_f block);
extern int CRYPTO_bc_decrypt(void *hdl, unsigned int uiAlgID, const unsigned char *pucaKey, unsigned int keyLen,
                             unsigned char *pucIv, unsigned int uiIvLen, const unsigned char *in, size_t len,
                             unsigned char *out, block128_f block);

extern int CRYPTO_ofbnlf_encrypt(void *hdl, unsigned int uiAlgID, const unsigned char *pucaKey, unsigned int keyLen,
                                 unsigned char *pucIv, unsigned int uiIvLen, const unsigned char *in, size_t len,
                                 unsigned char *out, block128_f block1, block128_f block2);

extern int CRYPTO_hctr_init(HCTR128_CONTEXT *ctx, uint8_t *tweak, uint32_t tweakLen);
extern int CRYPTO_hctr_encrypt(void *hdl, uint32_t uiAlgID, HCTR128_CONTEXT *ctx, uint8_t *in, uint32_t inLen,
                           uint8_t *out);
extern int CRYPTO_hctr_decrypt(void *hdl, uint32_t uiAlgID, HCTR128_CONTEXT *ctx, uint8_t *in, uint32_t inLen,
                           uint8_t *out);

extern int CRYPTO_eax_encrypt(unsigned int alg_type, const unsigned char *iv, unsigned int iv_len, unsigned char *puckey, 
                       unsigned int ke_len, unsigned char *header, unsigned int header_len, unsigned char *in, 
                       unsigned char *out, size_t len, unsigned char *puctag, unsigned int *tag_len);

extern int CRYPTO_eax_decrypt(unsigned int alg_type, const unsigned char *iv, unsigned int iv_len, unsigned char *puckey, 
                       unsigned int ke_len, unsigned char *header, unsigned int header_len, unsigned char *in, 
                       unsigned char *out, size_t len, unsigned char *puctag, unsigned int tag_len);
/* ------------------------------------------------------------------------------------------------------- */

#endif
