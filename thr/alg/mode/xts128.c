/*
 * Copyright 2011-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <string.h>
#include <stdio.h>
#include "symm_modes.h"

int CRYPTO_xts128_encrypt(void *hdl, unsigned int uiAlgID, const XTS128_CONTEXT *ctx, const unsigned char iv[16],
                          const unsigned char *inp, unsigned char *out, size_t len, int enc)
{
    const union {
        long one;
        char little;
    } is_endian = {1};
    union {
        u64 u[2];
        u32 d[4];
        u8 c[16];
    } tweak, scratch;
    unsigned int i;

    if (len < 16) {
        return -1;
    }

    memcpy(tweak.c, iv, 16);

    (*ctx->block2)(hdl, uiAlgID, ctx->key2, ctx->key2_len, tweak.c, 16, tweak.c);

    if (!enc && (len % 16)) {
        len -= 16;
    }

    while (len >= 16) {
#if defined(STRICT_ALIGNMENT)
        memcpy(scratch.c, inp, 16);
        scratch.u[0] ^= tweak.u[0];
        scratch.u[1] ^= tweak.u[1];
#else
        scratch.u[0] = ((u64 *)inp)[0] ^ tweak.u[0];
        scratch.u[1] = ((u64 *)inp)[1] ^ tweak.u[1];
#endif
        (*ctx->block1)(hdl, uiAlgID, ctx->key1, ctx->key1_len, scratch.c, 16, scratch.c);
#if defined(STRICT_ALIGNMENT)
        scratch.u[0] ^= tweak.u[0];
        scratch.u[1] ^= tweak.u[1];
        memcpy(out, scratch.c, 16);
#else
        ((u64 *)out)[0] = scratch.u[0] ^= tweak.u[0];
        ((u64 *)out)[1] = scratch.u[1] ^= tweak.u[1];
#endif
        inp += 16;
        out += 16;
        len -= 16;

        if (len == 0) {
            return 0;
        }

        if (is_endian.little) {
            unsigned int carry, res;

            res = 0x87 & (((int)tweak.d[3]) >> 31);
            carry = (unsigned int)(tweak.u[0] >> 63);
            tweak.u[0] = (tweak.u[0] << 1) ^ res;
            tweak.u[1] = (tweak.u[1] << 1) | carry;
        } else {
            size_t c;

            for (c = 0, i = 0; i < 16; ++i) {
                /*
                 * + substitutes for |, because c is 1 bit
                 */
                c += ((size_t)tweak.c[i]) << 1;
                tweak.c[i] = (u8)c;
                c = c >> 8;
            }
            tweak.c[0] ^= (u8)(0x87 & (0 - c));
        }
    }
    if (enc) {
        for (i = 0; i < len; ++i) {
            u8 c = inp[i];
            out[i] = scratch.c[i];
            scratch.c[i] = c;
        }
        scratch.u[0] ^= tweak.u[0];
        scratch.u[1] ^= tweak.u[1];
        (*ctx->block1)(hdl, uiAlgID, ctx->key1, ctx->key1_len, scratch.c, 16, scratch.c);
        scratch.u[0] ^= tweak.u[0];
        scratch.u[1] ^= tweak.u[1];
        memcpy(out - 16, scratch.c, 16);
    } else {
        union {
            u64 u[2];
            u8 c[16];
        } tweak1;

        if (is_endian.little) {
            unsigned int carry, res;

            res = 0x87 & (((int)tweak.d[3]) >> 31);
            carry = (unsigned int)(tweak.u[0] >> 63);
            tweak1.u[0] = (tweak.u[0] << 1) ^ res;
            tweak1.u[1] = (tweak.u[1] << 1) | carry;
        } else {
            size_t c;

            for (c = 0, i = 0; i < 16; ++i) {
                /*
                 * + substitutes for |, because c is 1 bit
                 */
                c += ((size_t)tweak.c[i]) << 1;
                tweak1.c[i] = (u8)c;
                c = c >> 8;
            }
            tweak1.c[0] ^= (u8)(0x87 & (0 - c));
        }
#if defined(STRICT_ALIGNMENT)
        memcpy(scratch.c, inp, 16);
        scratch.u[0] ^= tweak1.u[0];
        scratch.u[1] ^= tweak1.u[1];
#else
        scratch.u[0] = ((u64 *)inp)[0] ^ tweak1.u[0];
        scratch.u[1] = ((u64 *)inp)[1] ^ tweak1.u[1];
#endif
        (*ctx->block1)(hdl, uiAlgID, ctx->key1, ctx->key1_len, scratch.c, 16, scratch.c);
        scratch.u[0] ^= tweak1.u[0];
        scratch.u[1] ^= tweak1.u[1];

        for (i = 0; i < len; ++i) {
            u8 c = inp[16 + i];
            out[16 + i] = scratch.c[i];
            scratch.c[i] = c;
        }
        scratch.u[0] ^= tweak.u[0];
        scratch.u[1] ^= tweak.u[1];
        (*ctx->block1)(hdl, uiAlgID, ctx->key1, ctx->key1_len, scratch.c, 16, scratch.c);
#if defined(STRICT_ALIGNMENT)
        scratch.u[0] ^= tweak.u[0];
        scratch.u[1] ^= tweak.u[1];
        memcpy(out, scratch.c, 16);
#else
        ((u64 *)out)[0] = scratch.u[0] ^ tweak.u[0];
        ((u64 *)out)[1] = scratch.u[1] ^ tweak.u[1];
#endif
    }

    return 0;
}

int CRYPTO_xts128_gb_encrypt(void *hdl, unsigned int uiAlgID, const XTS128_CONTEXT *ctx, const unsigned char iv[16],
                             const unsigned char *inp, unsigned char *out, size_t len, int enc)
{
    OPENSSL_DECLARE_IS_ENDIAN;
    union {
        u64 u[2];
        u32 d[4];
        u8 c[16];
    } tweak, scratch;
    unsigned int i;

    if (len < 16) {
        return -1;
    }

    memcpy(tweak.c, iv, 16);

    (*ctx->block2)(hdl, uiAlgID, ctx->key2, ctx->key2_len, tweak.c, 16, tweak.c);

    if (!enc && (len % 16)) {
        len -= 16;
    }

    while (len >= 16) {
#if defined(STRICT_ALIGNMENT)
        memcpy(scratch.c, inp, 16);
        scratch.u[0] ^= tweak.u[0];
        scratch.u[1] ^= tweak.u[1];
#else
        scratch.u[0] = ((u64 *)inp)[0] ^ tweak.u[0];
        scratch.u[1] = ((u64 *)inp)[1] ^ tweak.u[1];
#endif
        (*ctx->block1)(hdl, uiAlgID, ctx->key1, ctx->key1_len, scratch.c, 16, scratch.c);
#if defined(STRICT_ALIGNMENT)
        scratch.u[0] ^= tweak.u[0];
        scratch.u[1] ^= tweak.u[1];
        memcpy(out, scratch.c, 16);
#else
        ((u64 *)out)[0] = scratch.u[0] ^= tweak.u[0];
        ((u64 *)out)[1] = scratch.u[1] ^= tweak.u[1];
#endif
        inp += 16;
        out += 16;
        len -= 16;

        if (len == 0) {
            return 0;
        }

        if (OPENSSL_IS_LITTLE_ENDIAN) {
            u8 res;
            u64 hi, lo;
#ifdef BSWAP8
            hi = BSWAP8(tweak.u[0]);
            lo = BSWAP8(tweak.u[1]);
#else
            u8 *p = tweak.c;

            hi = (u64)GETU32(p) << 32 | GETU32(p + 4);
            lo = (u64)GETU32(p + 8) << 32 | GETU32(p + 12);
#endif
            res = (u8)lo & 1;
            tweak.u[0] = (lo >> 1) | (hi << 63);
            tweak.u[1] = hi >> 1;
            if (res) {
                tweak.c[15] ^= 0xe1;
            }
#ifdef BSWAP8
            hi = BSWAP8(tweak.u[0]);
            lo = BSWAP8(tweak.u[1]);
#else
            p = tweak.c;

            hi = (u64)GETU32(p) << 32 | GETU32(p + 4);
            lo = (u64)GETU32(p + 8) << 32 | GETU32(p + 12);
#endif
            tweak.u[0] = lo;
            tweak.u[1] = hi;
        } else {
            u8 carry, res;
            carry = 0;
            for (i = 0; i < 16; ++i) {
                res = (tweak.c[i] << 7) & 0x80;
                tweak.c[i] = ((tweak.c[i] >> 1) + carry) & 0xff;
                carry = res;
            }
            if (res) {
                tweak.c[0] ^= 0xe1;
            }
        }
    }
    if (enc) {
        for (i = 0; i < len; ++i) {
            u8 c = inp[i];
            out[i] = scratch.c[i];
            scratch.c[i] = c;
        }
        scratch.u[0] ^= tweak.u[0];
        scratch.u[1] ^= tweak.u[1];
        (*ctx->block1)(hdl, uiAlgID, ctx->key1, ctx->key1_len, scratch.c, 16, scratch.c);
        scratch.u[0] ^= tweak.u[0];
        scratch.u[1] ^= tweak.u[1];
        memcpy(out - 16, scratch.c, 16);
    } else {
        union {
            u64 u[2];
            u8 c[16];
        } tweak1;

        if (OPENSSL_IS_LITTLE_ENDIAN) {
            u8 res;
            u64 hi, lo;
#ifdef BSWAP8
            hi = BSWAP8(tweak.u[0]);
            lo = BSWAP8(tweak.u[1]);
#else
            u8 *p = tweak.c;

            hi = (u64)GETU32(p) << 32 | GETU32(p + 4);
            lo = (u64)GETU32(p + 8) << 32 | GETU32(p + 12);
#endif
            res = (u8)lo & 1;
            tweak1.u[0] = (lo >> 1) | (hi << 63);
            tweak1.u[1] = hi >> 1;
            if (res) {
                tweak1.c[15] ^= 0xe1;
            }
#ifdef BSWAP8
            hi = BSWAP8(tweak1.u[0]);
            lo = BSWAP8(tweak1.u[1]);
#else
            p = tweak1.c;

            hi = (u64)GETU32(p) << 32 | GETU32(p + 4);
            lo = (u64)GETU32(p + 8) << 32 | GETU32(p + 12);
#endif
            tweak1.u[0] = lo;
            tweak1.u[1] = hi;
        } else {
            u8 carry, res;
            carry = 0;
            for (i = 0; i < 16; ++i) {
                res = (tweak.c[i] << 7) & 0x80;
                tweak1.c[i] = ((tweak.c[i] >> 1) + carry) & 0xff;
                carry = res;
            }
            if (res) {
                tweak1.c[0] ^= 0xe1;
            }
        }
#if defined(STRICT_ALIGNMENT)
        memcpy(scratch.c, inp, 16);
        scratch.u[0] ^= tweak1.u[0];
        scratch.u[1] ^= tweak1.u[1];
#else
        scratch.u[0] = ((u64 *)inp)[0] ^ tweak1.u[0];
        scratch.u[1] = ((u64 *)inp)[1] ^ tweak1.u[1];
#endif
        (*ctx->block1)(hdl, uiAlgID, ctx->key1, ctx->key1_len, scratch.c, 16, scratch.c);
        scratch.u[0] ^= tweak1.u[0];
        scratch.u[1] ^= tweak1.u[1];

        for (i = 0; i < len; ++i) {
            u8 c = inp[16 + i];
            out[16 + i] = scratch.c[i];
            scratch.c[i] = c;
        }
        scratch.u[0] ^= tweak.u[0];
        scratch.u[1] ^= tweak.u[1];
        (*ctx->block1)(hdl, uiAlgID, ctx->key1, ctx->key1_len, scratch.c, 16, scratch.c);
#if defined(STRICT_ALIGNMENT)
        scratch.u[0] ^= tweak.u[0];
        scratch.u[1] ^= tweak.u[1];
        memcpy(out, scratch.c, 16);
#else
        ((u64 *)out)[0] = scratch.u[0] ^ tweak.u[0];
        ((u64 *)out)[1] = scratch.u[1] ^ tweak.u[1];
#endif
    }

    return 0;
}

int CRYPTO_xts_encrypt(void *hdl, unsigned int uiAlgID, const unsigned char iv[16], const unsigned char *puckey,
                       unsigned int uikeylen, const unsigned char *tweakkey, unsigned int tweakkeylen,
                       const unsigned char *inp, unsigned char *out, size_t len, int enc, unsigned int isnation,
                       block128_f block1, block128_f block2)
{
    XTS128_CONTEXT ctx;
    int ret;

    // block2: encrypt the iv with key2(tweakkey)
    // block1: en(de)crypt the inp with key1(puckey)
    ctx.key1 = (unsigned char *)puckey;
    ctx.key2 = (unsigned char *)tweakkey;
    ctx.key1_len = uikeylen;
    ctx.key2_len = tweakkeylen;
    ctx.block1 = block1;
    ctx.block2 = block2;

    if (isnation == IS_INTERNATIONAL_AES_ALGO) {
        ret = CRYPTO_xts128_encrypt(hdl, uiAlgID, &ctx, iv, inp, out, len, enc);
    } else if (isnation == IS_NATIONAL_SM4_ALGO) {
        ret = CRYPTO_xts128_gb_encrypt(hdl, uiAlgID, &ctx, iv, inp, out, len, enc);
    } else {
        return -1;
    }

    if (ret) {
        return -2;
    }

    return 0;
}
