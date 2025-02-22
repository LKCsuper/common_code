/* ============================================================================
 * Copyright (c) 2010-2015.  All rights reserved.
 * SM3 Hash Cipher Algorithm: Digest length is 256-bit
 * ============================================================================
 */

#ifndef __SM3_HEADER__
#define __SM3_HEADER__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define SM3_HASH_SIZE 32
#define SM3_HASH_LEN  32

    typedef struct sm3_ctx
    {
        uint32_t h[8];
        uint8_t bb[64];
        uint64_t len;
    } sm3_ctx_t;

    typedef struct
    {
        unsigned int state[8];
        unsigned long long tc;
        unsigned int bc;
        unsigned char buffer[64];
    } SM3_CTX; // SM3算法的基本结构体

    int32_t sm3_init(struct sm3_ctx *ctx);
    int32_t sm3_update(struct sm3_ctx *ctx, const uint8_t *in, uint32_t inlen);
    int32_t sm3_final(struct sm3_ctx *ctx, uint8_t *hash);
    int32_t sm3(const uint8_t *in, uint32_t inlen, uint8_t *hash);

    // SM3算法三件套，首先调用SM3Init，然后调用SM3Update（可重复调用）逐步将消息读入，最后调用SM3Final得到SM3摘要值digest
    void SM3Init(SM3_CTX *context);
    void SM3Update(SM3_CTX *context, unsigned char *data, unsigned int len);
    void SM3Final(unsigned char digest[32], SM3_CTX *context);

    // SM3算法一步到位，输入消息和长度，得到摘要digest
    void SM3(unsigned char *data, unsigned int len, unsigned char digest[32]);

#ifdef __cplusplus
}
#endif

#endif
