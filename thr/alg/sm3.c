/* ============================================================================
 * Copyright (c) 2010-2015.  All rights reserved.
 * SM3 Hash Cipher Algorithm: Digest length is 256-bit
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sm3.h"

#define SM3_DIGEST_LENGTH 32
#define SM3_BLOCK_LENGTH  64
#define SM3_PAD_MAXLEN    56

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

#define P0(X) (X ^ rol(X, 9) ^ rol(X, 17))
#define P1(X) (X ^ rol(X, 15) ^ rol(X, 23))

#define FF_00_15(X, Y, Z) (X ^ Y ^ Z)
#define FF_16_63(X, Y, Z) ((X & Y) | (Y & Z) | (Z & X))

#define GG_00_15(X, Y, Z) (X ^ Y ^ Z)
#define GG_16_63(X, Y, Z) ((X & Y) | ((~X) & Z))

typedef long long int64;
typedef unsigned long long uint64;
typedef int int32;
typedef unsigned int uint32;

static const uint32 T_00_15 = 0x79cc4519;
static const uint32 T_16_63 = 0x7a879d8a;

#ifdef VERBOSE /* SAK */
void SM3PrintContext(SM3_CTX *context, char *msg)
{
    printf("%s (%d,%d) %x %x %x %x %x %x %x %x\n",
           msg,
           context->count[0],
           context->count[1],
           context->state[0],
           context->state[1],
           context->state[2],
           context->state[3],
           context->state[4],
           context->state[5],
           context->state[6],
           context->state[7]);
}
#endif

#define ROTL(x, y) ((((x) << (y & (32 - 1))) | ((x) >> (32 - (y & (32 - 1))))) & 0xffffffff)

static void sm3_ms(uint32_t *pW, const uint32_t *pMsg)
{
    int32_t j;

    for (j = 0; j < 16; j++) {
        pW[j] = pMsg[j];
    }

    for (; j < 68; j++) {
        pW[j] = pW[j - 16] ^ pW[j - 9] ^ ROTL(pW[j - 3], 15);
        pW[j] ^= (ROTL(pW[j], 15) ^ ROTL(pW[j], 23));
        pW[j] ^= ((ROTL(pW[j - 13], 7) ^ pW[j - 6]));
    }

    for (j = 0; j < 64; j++) {
        pW[68 + j] = pW[j] ^ pW[j + 4];
    }
}

static void sm3_cf(uint32_t *pH, const uint8_t *pMsg)
{
    uint32_t A = pH[0], B = pH[1], C = pH[2], D = pH[3], E = pH[4], F = pH[5], G = pH[6], H = pH[7];
    uint32_t w[68 + 64];
    uint32_t SS1, SS2, TT1, TT2;
    uint32_t T;

    // w[0..67] for W
    // w[68+0 .. 68+63] for W'

    int32_t j;
    // unsigned char --> uint32_t
    uint32_t BB[16];
    for (j = 0; j < 16; j++) {
        BB[j] = (pMsg[4 * j] << 24) | (pMsg[4 * j + 1] << 16) | (pMsg[4 * j + 2] << 8) | pMsg[4 * j + 3];
    }

    sm3_ms(w, BB);

    T = 0x79CC4519;
    for (j = 0; j < 16; j++) {
        SS1 = (ROTL(A, 12) + E + ROTL(T, j)) % 0x100000000;
        SS1 = ROTL(SS1, 7);
        SS2 = SS1 ^ ROTL(A, 12);
        TT1 = (((A ^ B ^ C) + D + SS2) + w[j + 68]) % 0x100000000;
        TT2 = (((E ^ F ^ G) + H + SS1) + w[j]) % 0x100000000;
        D   = C;
        C   = ROTL(B, 9);
        B   = A;
        A   = TT1;
        H   = G;
        G   = ROTL(F, 19);
        F   = E;
        E   = TT2 ^ ROTL(TT2, 9) ^ ROTL(TT2, 17);
    }

    T = 0x7A879D8A;
    for (; j < 64; j++) {
        SS1 = (ROTL(A, 12) + E + ROTL(T, j)) % 0x100000000;
        SS1 = ROTL(SS1, 7);
        SS2 = SS1 ^ ROTL(A, 12);
        TT1 = ((((A & B) | (A & C) | (B & C)) + D + SS2) + w[j + 68]) % 0x100000000;
        TT2 = ((((E & F) | ((~E) & G)) + H + SS1) + w[j]) % 0x100000000;
        D   = C;
        C   = ROTL(B, 9);
        B   = A;
        A   = TT1;
        H   = G;
        G   = ROTL(F, 19);
        F   = E;
        E   = TT2 ^ ROTL(TT2, 9) ^ ROTL(TT2, 17);
    }

    pH[0] ^= A;
    pH[1] ^= B;
    pH[2] ^= C;
    pH[3] ^= D;
    pH[4] ^= E;
    pH[5] ^= F;
    pH[6] ^= G;
    pH[7] ^= H;
}

int32_t sm3_init(struct sm3_ctx *ctx)
{
    ctx->h[0] = 0x7380166F;
    ctx->h[1] = 0x4914B2B9;
    ctx->h[2] = 0x172442D7;
    ctx->h[3] = 0xDA8A0600;
    ctx->h[4] = 0xA96F30BC;
    ctx->h[5] = 0x163138AA;
    ctx->h[6] = 0xE38DEE4D;
    ctx->h[7] = 0xB0FB0E4E;

    ctx->len = 0;

    return 0;
}

int32_t sm3_update(struct sm3_ctx *ctx, const uint8_t *in, uint32_t inlen)
{
    uint32_t i, iCurPos = (uint32_t)(ctx->len % 64);
    uint32_t iMin = (64 - iCurPos < inlen) ? (64 - iCurPos) : (inlen);

    for (i = 0; i < iMin; i++) {
        ctx->bb[i + iCurPos] = in[i];
    }

    ctx->len += inlen;

    if (inlen >= 64 - iCurPos) {
        sm3_cf(ctx->h, ctx->bb);

        inlen -= (64 - iCurPos);
        in += (64 - iCurPos);
        for (i = 0; i < inlen / 64; i++) {
            sm3_cf(ctx->h, in + i * 64);
        }

        for (i = 0; i < (inlen & 63); i++) {
            ctx->bb[i] = in[(inlen & 0xffffffc0) + i];
        }
    }

    return 0;
}

int32_t sm3_final(struct sm3_ctx *ctx, uint8_t *hash)
{
    uint32_t i;
    uint8_t bb[128];
    uint32_t low = 0, high = 0;

    for (i = 0; i < sizeof(bb); i++) {
        bb[i] = 0x00;
    }

    for (i = 0; i < (ctx->len & 63); i++) {
        bb[i] = ctx->bb[i];
    }

    high = (uint32_t)(ctx->len >> 29);
    high = (high >> 24) | ((high & 0x00ff0000) >> 8) | ((high & 0x0000ff00) << 8) | (high << 24);
    low  = (uint32_t)(ctx->len << 3);
    low  = (low >> 24) | ((low & 0x00ff0000) >> 8) | ((low & 0x0000ff00) << 8) | (low << 24);

    if ((ctx->len & 63) < 56) {
        // 1 block
        bb[i] = 0x80;

        // BB[56 .. 63]
        *(uint32_t *)(bb + 56) = high;
        *(uint32_t *)(bb + 60) = low;

        sm3_cf(ctx->h, bb);
    } else {
        // 2 block
        bb[i] = 0x80;
        sm3_cf(ctx->h, bb);

        // BB[120 .. 127]
        *(uint32_t *)(bb + 120) = high;
        *(uint32_t *)(bb + 124) = low;

        sm3_cf(ctx->h, bb + 64);
    }

    for (i = 0; i < 8; i++) {
        hash[i * 4]     = (unsigned char)(ctx->h[i] >> 24);
        hash[i * 4 + 1] = (unsigned char)(ctx->h[i] >> 16);
        hash[i * 4 + 2] = (unsigned char)(ctx->h[i] >> 8);
        hash[i * 4 + 3] = (unsigned char)(ctx->h[i]);
    }

    return 0;
}

int32_t sm3(const uint8_t *in, uint32_t inlen, uint8_t *hash)
{
    uint32_t i;
    uint32_t low = 0, high = 0;
    unsigned char BB2[128];

    uint32_t H[8] = {
        0x7380166F, 0x4914B2B9, 0x172442D7, 0xDA8A0600, 0xA96F30BC, 0x163138AA, 0xE38DEE4D, 0xB0FB0E4E,
    };

    for (i = 0; i < inlen / 64; i++) {
        sm3_cf(H, in + i * 64);
    }

    for (i = 0; i < sizeof(BB2); i++) {
        BB2[i] = 0x00;
    }

    for (i = 0; i < (inlen & 63); i++) {
        BB2[i] = in[(inlen & 0xffffffc0) + i];
    }

    // in bytes ==> in bits

    high = inlen >> 29;
    high = (high >> 24) | ((high & 0x00ff0000) >> 8) | ((high & 0x0000ff00) << 8) | (high << 24);
    low  = inlen << 3;
    low  = (low >> 24) | ((low & 0x00ff0000) >> 8) | ((low & 0x0000ff00) << 8) | (low << 24);

    if ((inlen & 63) < 56) {
        // 1 block
        BB2[i] = 0x80;

        // BB[56 .. 63]
        *(uint32_t *)(BB2 + 56) = high;
        *(uint32_t *)(BB2 + 60) = low;

        sm3_cf(H, BB2);
    } else {
        // 2 block
        BB2[i] = 0x80;
        sm3_cf(H, BB2);

        // BB[120 .. 127]
        *(uint32_t *)(BB2 + 120) = high;
        *(uint32_t *)(BB2 + 124) = low;

        sm3_cf(H, BB2 + 64);
    }

    for (i = 0; i < 8; i++) {
        hash[i * 4]     = (unsigned char)(H[i] >> 24);
        hash[i * 4 + 1] = (unsigned char)(H[i] >> 16);
        hash[i * 4 + 2] = (unsigned char)(H[i] >> 8);
        hash[i * 4 + 3] = (unsigned char)H[i];
    }

    return 0;
}

/* Hash a single 512-bit block. This is the core of the algorithm. */
void SM3Transform(SM3_CTX *context, unsigned char buffer[64])
{
    uint32 a, b, c, d, e, f, g, h;
    uint32 W[68];
    uint32 W1[64];
    uint32 SS1, SS2, TT1, TT2;
    register int j;
    unsigned char *datap;
    unsigned int *pState = &context->state[0];

    //	const uint32 *B=(const uint32 *)buffer;

    j     = 0;
    datap = buffer;
    do {
        W[j] = (((uint32)(datap[0])) << 24) | (((uint32)(datap[1])) << 16) | (((uint32)(datap[2])) << 8) |
               ((uint32)(datap[3]));
        datap += 4;
    } while (++j < 16);

    for (j = 16; j < 68; j++) {
        W[j] = P1(W[j - 16] ^ W[j - 9] ^ rol(W[j - 3], 15)) ^ rol(W[j - 13], 7) ^ W[j - 6];
    }
    for (j = 0; j < 64; j++) {
        W1[j] = W[j] ^ W[j + 4];
    }

    /* Copy pState[] to working vars */
    a = pState[0];
    b = pState[1];
    c = pState[2];
    d = pState[3];
    e = pState[4];
    f = pState[5];
    g = pState[6];
    h = pState[7];

    for (j = 0; j < 16; j++) {
        SS1 = rol(a, 12) + e + rol(T_00_15, j);
        SS1 = rol(SS1, 7);
        SS2 = SS1 ^ rol(a, 12);
        TT1 = FF_00_15(a, b, c) + d + SS2 + W1[j];
        TT2 = GG_00_15(e, f, g) + h + SS1 + W[j];
        d   = c;
        c   = rol(b, 9);
        b   = a;
        a   = TT1;
        h   = g;
        g   = rol(f, 19);
        f   = e;
        e   = P0(TT2);
    }
    for (j = 16; j < 64; j++) {
        SS1 = rol(a, 12) + e + rol(T_16_63, j);
        SS1 = rol(SS1, 7);
        SS2 = SS1 ^ rol(a, 12);
        TT1 = FF_16_63(a, b, c) + d + SS2 + W1[j];
        TT2 = GG_16_63(e, f, g) + h + SS1 + W[j];
        d   = c;
        c   = rol(b, 9);
        b   = a;
        a   = TT1;
        h   = g;
        g   = rol(f, 19);
        f   = e;
        e   = P0(TT2);
    }

    /* Add the working vars back into pState[] */
    pState[0] ^= a;
    pState[1] ^= b;
    pState[2] ^= c;
    pState[3] ^= d;
    pState[4] ^= e;
    pState[5] ^= f;
    pState[6] ^= g;
    pState[7] ^= h;
    /* Wipe variables */
    a = b = c = d = e = f = g = h = 0;
}

/* SM3Init - Initialize new context */
void SM3Init(SM3_CTX *context)
{
    /* SM3 initialization constants */
    context->state[0] = 0x7380166f;
    context->state[1] = 0x4914b2b9;
    context->state[2] = 0x172442d7;
    context->state[3] = 0xda8a0600;
    context->state[4] = 0xa96f30bc;
    context->state[5] = 0x163138aa;
    context->state[6] = 0xe38dee4d;
    context->state[7] = 0xb0fb0e4e;
    context->tc       = 0;
    context->bc       = 0;
}


/* Run your data through this. */
void SM3Update(SM3_CTX *ctx, unsigned char *datap, uint32 len)
{
#ifdef VERBOSE
    SM3PrintContext(ctx, "before");
#endif
    // if ( ctx->bc == (ctx->tc % 64) ){printf("before sm3 bc = tc mod 64\n");}else{printf("end error\n");}
    ctx->tc += len;

    while (len > 0) {
        if (!ctx->bc) {
            while (len >= SM3_BLOCK_LENGTH) {
                SM3Transform(ctx, datap);
                datap += SM3_BLOCK_LENGTH;
                len -= SM3_BLOCK_LENGTH;
            }
            if (!len)
                return;
        }
        ctx->buffer[ctx->bc] = *datap++;
        len--;
        if (++ctx->bc == SM3_BLOCK_LENGTH) {
            SM3Transform(ctx, &ctx->buffer[0]);
            ctx->bc = 0;
        }
    }
    // if ( ctx->bc == (ctx->tc % 64) ){printf("after sm3 bc = tc mod 64\n");}else{printf("end error\n");}
#ifdef VERBOSE
    SM3PrintContext(ctx, "after ");
#endif
}

/* Add padding and return the message digest. */
void SM3Final(unsigned char digest[32], SM3_CTX *ctx)
{
    uint32 i;
    uint64 bitLength;
    register int j;
    unsigned char *datap;

    ctx->buffer[ctx->bc++] = 0x80;
    if (ctx->bc > SM3_PAD_MAXLEN) {
        while (ctx->bc < SM3_BLOCK_LENGTH) {
            ctx->buffer[ctx->bc++] = 0x00;
        }
        SM3Transform(ctx, &ctx->buffer[0]);
        ctx->bc = 0;
    }

    while (ctx->bc < SM3_PAD_MAXLEN) {
        ctx->buffer[ctx->bc++] = 0x00;
    }
    if (ctx->bc == SM3_PAD_MAXLEN) {
        bitLength       = ctx->tc * 8;
        ctx->buffer[56] = (unsigned char)(bitLength >> 56);
        ctx->buffer[57] = (unsigned char)(bitLength >> 48);
        ctx->buffer[58] = (unsigned char)(bitLength >> 40);
        ctx->buffer[59] = (unsigned char)(bitLength >> 32);
        ctx->buffer[60] = (unsigned char)(bitLength >> 24);
        ctx->buffer[61] = (unsigned char)(bitLength >> 16);
        ctx->buffer[62] = (unsigned char)(bitLength >> 8);
        ctx->buffer[63] = (unsigned char)bitLength;
    }
    SM3Transform(ctx, &ctx->buffer[0]);
    ctx->bc = 0;

    datap = digest;
    j     = 0;
    do {
        i        = ctx->state[j];
        datap[0] = i >> 24;
        datap[1] = i >> 16;
        datap[2] = i >> 8;
        datap[3] = i;
        datap += 4;
    } while (++j < 8);
    // memset(ctx, 0, sizeof *ctx);
}

void SM3_hash_buffer(unsigned char *ib, int ile, unsigned char *ob, int ole)
{
    SM3_CTX ctx;

    if (ole < 1)
        return;
    // if(ob == NULL) return;

    memset(ob, 0, ole);
    if (ole < 32)
        return;

    if (ole > 32)
        ole = 32;
    SM3Init(&ctx);
    SM3Update(&ctx, ib, ile);
    SM3Final(ob, &ctx);

    // memset(&ctx, 0, sizeof(ctx));
}

void SM3(unsigned char *data, unsigned int len, unsigned char digest[32])
{
    SM3_CTX sm3, *psm3;
    psm3 = &sm3;
    SM3Init(psm3);
    SM3Update(psm3, data, len);
    SM3Final(digest, psm3);
}
