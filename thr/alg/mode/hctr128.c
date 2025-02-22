#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <endian.h>
#include <stdbool.h>
#include <stdarg.h>
#include <inttypes.h>

#include "symm_modes.h"

hctr_ctx hctrCtx;

static void put_u64_big_endian(uint8_t *buf, size_t offset, uint64_t val)
{
    *(uint64_t *)(buf + offset) = htobe64(val);
}

static void get_u64_big_endian(uint8_t *buf, size_t offset, uint64_t *val)
{
    *val = be64toh(*(uint64_t *)(buf + offset));
}

uint64_t get_uint64_be(uint8_t *buf)
{
    uint32_t high = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
    uint32_t low = ((uint32_t)buf[4] << 24) | ((uint32_t)buf[5] << 16) | ((uint32_t)buf[6] << 8) | (uint32_t)buf[7];
    return ((uint64_t)high << 32) | low;
}

void put_uint64_be(uint8_t *buf, uint64_t value)
{
    buf[0] = (uint8_t)(value >> 56);
    buf[1] = (uint8_t)(value >> 48);
    buf[2] = (uint8_t)(value >> 40);
    buf[3] = (uint8_t)(value >> 32);
    buf[4] = (uint8_t)(value >> 24);
    buf[5] = (uint8_t)(value >> 16);
    buf[6] = (uint8_t)(value >> 8);
    buf[7] = (uint8_t)(value);
}

int XorBytes(uint8_t *dst, uint8_t *x, uint8_t *y, size_t len_x, size_t len_y)
{
    size_t i;
    size_t n = len_x;

    if (len_y < n) {
        n = len_y;
    }

    if (n == 0) {
        return 0;
    }

    if (n > SYMM_SOFT_ALGO_SM4_BLOCK_SIZE) {
        return -1;
    }

    for (i = 0; i < n; i++) {
        dst[i] = x[i] ^ y[i];
    }

    return (int)n;
}

// reverseBits reverses the order of the bits of 4-bit number in i.
int reverseBits(int i)
{
    i = ((i << 2) & 0xc) | ((i >> 2) & 0x3);
    i = ((i << 1) & 0xa) | ((i >> 1) & 0x5);

    return i;
}

// hctrReductionTable is stored irreducible polynomial's double & add precomputed results.
// 0000 - 0
// 0001 - irreducible polynomial >> 3
// 0010 - irreducible polynomial >> 2
// 0011 - (irreducible polynomial >> 3 xor irreducible polynomial >> 2)
// ...
// 1000 - just the irreducible polynomial
static uint16_t hctrReductionTable[] = {
    0x0000, 0x1c20, 0x3840, 0x2460, 0x7080, 0x6ca0, 0x48c0, 0x54e0,
    0xe100, 0xfd20, 0xd940, 0xc560, 0x9180, 0x8da0, 0xa9c0, 0xb5e0,
};

// hctrAdd adds two elements of GF(2¹²⁸) and returns the sum.
hctrFieldElement hctrAdd(hctrFieldElement *x, hctrFieldElement *y)
{
    hctrFieldElement h;

    h.low = x->low ^ y->low;
    h.high = x->high ^ y->high;

    return h;
}

// hctrDouble returns the result of doubling an element of GF(2¹²⁸).
hctrFieldElement hctrDouble(hctrFieldElement *x)
{
    hctrFieldElement doubleElem;

    int msbSet = ((x->high & 1) == 1);

    doubleElem.high = x->high >> 1;
    doubleElem.high |= (x->low << 63);
    doubleElem.low = x->low >> 1;

    if (msbSet) {
        doubleElem.low ^= 0xe100000000000000ULL;
    }

    return doubleElem;
}

// in HCTR mode. The lenght of tweak and hash key must be the same as the [Block]'s block size.
int CRYPTO_hctr_init(HCTR128_CONTEXT *ctx, uint8_t *tweak, uint32_t tweakLen)
{
    hctrFieldElement x;
    int i;

    if (ctx == NULL) {
        return -1;
    }

    if (ctx->encKeyLen != SYMM_SOFT_ALGO_SM4_BLOCK_SIZE || ctx->hashKeyLen != SYMM_SOFT_ALGO_SM4_BLOCK_SIZE ||
        tweakLen != SYMM_SOFT_ALGO_SM4_BLOCK_SIZE) {
        return -1;
    }

    memcpy(hctrCtx.tweak, tweak, tweakLen);
    hctrCtx.tweakLen = tweakLen;

    x.low = get_uint64_be(ctx->hashKey);
    x.high = get_uint64_be(ctx->hashKey + 8);

    hctrCtx.productTable[reverseBits(1)].low = x.low;
    hctrCtx.productTable[reverseBits(1)].high = x.high;
    for (i = 2; i < 16; i += 2) {
        hctrCtx.productTable[reverseBits(i)] = hctrDouble(&hctrCtx.productTable[reverseBits(i / 2)]);
        hctrCtx.productTable[reverseBits(i + 1)] = hctrAdd(&hctrCtx.productTable[reverseBits(i)], &x);
    }

    return 0;
}

// mul sets y to y*H, where H is the GCM key
void mul(hctrFieldElement *y)
{
    int i, j;
    uint64_t word;
    uint16_t msw;
    hctrFieldElement z;
    hctrFieldElement t;

    z.low = 0;
    z.high = 0;

    (void)hctrReductionTable[0xf];

    for (i = 0; i < 2; i++) {
        word = (i == 0) ? y->high : y->low;

        // Multiplication works by multiplying z by 16 and adding in
        // one of the precomputed multiples of hash key.
        for (j = 0; j < 64; j += 4) {
            msw = (uint64_t)(z.high) & 0xf;

            z.high = (z.high >> 4) | (z.low << 60);
            z.low = (z.low >> 4);
            z.low ^= (uint64_t)(hctrReductionTable[msw]) << 48;

            t = hctrCtx.productTable[word & 0xf];

            z.low ^= t.low;
            z.high ^= t.high;
            word >>= 4;
        }
    }
    y->low = z.low;
    y->high = z.high;
}

void updateBlock(uint8_t *block, hctrFieldElement *y)
{
    y->low ^= get_uint64_be(block);
    y->high ^= get_uint64_be(block + 8);

    mul(y);
}

// Universal Hash Function.
// Chapter 3.3 in https://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.470.5288.
void uhash(uint8_t *m, uint32_t mLen, uint8_t *out)
{
    int i;
    hctrFieldElement y = {0, 0};
    uint8_t *msg = m;
    uint32_t uiMsgOriginalLen = mLen;
    uint8_t partialBlock[SYMM_SOFT_ALGO_SM4_BLOCK_SIZE] = {0};

    // update blocks
    while (mLen >= SYMM_SOFT_ALGO_SM4_BLOCK_SIZE) {
        updateBlock(msg, &y);
        msg += SYMM_SOFT_ALGO_SM4_BLOCK_SIZE;
        mLen -= SYMM_SOFT_ALGO_SM4_BLOCK_SIZE;
    }

    // update partial block & tweak
    if (mLen > 0) {
        memcpy(partialBlock, msg, mLen);
        memcpy(partialBlock + mLen, hctrCtx.tweak, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE - mLen);
        updateBlock(partialBlock, &y);

        memcpy(partialBlock, hctrCtx.tweak + mLen, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE - mLen);
        memset(partialBlock + mLen, 0x0, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE - mLen);
        updateBlock(partialBlock, &y);
    } else {
        updateBlock(hctrCtx.tweak, &y);
    }

    // update bit string length (|M|)2
    y.high ^= (uint64_t)(uiMsgOriginalLen + SYMM_SOFT_ALGO_SM4_BLOCK_SIZE) * 8;
    mul(&y);

    // output result
    put_uint64_be(out, y.low);
    put_uint64_be(out + 8, y.high);
}

void ctr(void *hdl, uint32_t uiAlgID, HCTR128_CONTEXT *ctx, uint8_t *dst, uint8_t *src, uint32_t srcLen,
         uint8_t *baseCtr)
{
    uint8_t ctr[SYMM_SOFT_ALGO_SM4_BLOCK_SIZE] = {0};
    uint8_t num[SYMM_SOFT_ALGO_SM4_BLOCK_SIZE] = {0};
    uint64_t i = 1;
    size_t n;

    while (srcLen > 0) {
        // put_u64_big_endian(num, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE - 8, i);
        put_uint64_be(num + 8, i);
        XorBytes(ctr, baseCtr, num, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE);

        (*ctx->block_enc)(hdl, uiAlgID, ctx->encKey, ctx->encKeyLen, ctr, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, ctr);

        n = srcLen < SYMM_SOFT_ALGO_SM4_BLOCK_SIZE ? srcLen : SYMM_SOFT_ALGO_SM4_BLOCK_SIZE;
        XorBytes(dst, src, ctr, n, n);

        src += n;
        dst += n;
        srcLen -= n;

        i++;
    }
}

int CRYPTO_hctr_encrypt(void *hdl, uint32_t uiAlgID, HCTR128_CONTEXT *ctx, uint8_t *in, uint32_t inLen, uint8_t *out)
{
    uint8_t z1[SYMM_SOFT_ALGO_SM4_BLOCK_SIZE];
    uint8_t z2[SYMM_SOFT_ALGO_SM4_BLOCK_SIZE];

    if (in == NULL || out == NULL) {
        return -1;
    }

    if (inLen < SYMM_SOFT_ALGO_SM4_BLOCK_SIZE) {
        return -1;
    }

    // a. z1 generation
    uhash(in + SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, inLen - SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, z1);
    XorBytes(z1, z1, in, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE);

    // b. z2 generation
    (*ctx->block_enc)(hdl, uiAlgID, ctx->encKey, ctx->encKeyLen, z1, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, z2);
 
    // c. ctr
    XorBytes(z1, z1, z2, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE);
    ctr(hdl, uiAlgID, ctx, out + SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, in + SYMM_SOFT_ALGO_SM4_BLOCK_SIZE,
        inLen - SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, z1);

    // d. first ciphertext block generation
    uhash(out + SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, inLen - SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, z1);    
    XorBytes(out, z1, z2, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE);

    return 0;
}

int CRYPTO_hctr_decrypt(void *hdl, uint32_t uiAlgID, HCTR128_CONTEXT *ctx, uint8_t *in, uint32_t inLen, uint8_t *out)
{
    uint8_t z1[SYMM_SOFT_ALGO_SM4_BLOCK_SIZE];
    uint8_t z2[SYMM_SOFT_ALGO_SM4_BLOCK_SIZE];

    if (in == NULL || out == NULL) {
        return -1;
    }

    if (inLen < SYMM_SOFT_ALGO_SM4_BLOCK_SIZE) {
        return -1;
    }

    // a. z2 generation
    uhash(in + SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, inLen - SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, z2);
    XorBytes(z2, z2, in, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE);

    // b. z1 generation
    (*ctx->block_dec)(hdl, uiAlgID, ctx->encKey, ctx->encKeyLen, z2, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, z1);

    // c. ctr
    XorBytes(z2, z2, z1, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE);
    ctr(hdl, uiAlgID, ctx, out + SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, in + SYMM_SOFT_ALGO_SM4_BLOCK_SIZE,
        inLen - SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, z2);
 
    // d. first ciphertext block generation
    uhash(out + SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, inLen - SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, z2);
    XorBytes(out, z2, z1, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE, SYMM_SOFT_ALGO_SM4_BLOCK_SIZE);

    return 0;
}
