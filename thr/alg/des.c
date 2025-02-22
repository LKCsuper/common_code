#include <string.h>

#include "des.h"

void sf(unsigned char *indt);
void transfer(unsigned char *dt_blk, unsigned char *tans_arry, unsigned char length);

unsigned char init_tr[] = { 58, 50, 42, 34, 26, 18, 10, 2,  60, 52, 44, 36, 28, 20, 12, 4,  62, 54, 46, 38, 30, 22,
                            14, 6,  64, 56, 48, 40, 32, 24, 16, 8,  57, 49, 41, 33, 25, 17, 9,  1,  59, 51, 43, 35,
                            27, 19, 11, 3,  61, 53, 45, 37, 29, 21, 13, 5,  63, 55, 47, 39, 31, 23, 15, 7 };

unsigned char fian_tr[] = { 40, 8,  48, 16, 56, 24, 64, 32, 39, 7,  47, 15, 55, 23, 63, 31, 38, 6,  46, 14, 54, 22,
                            62, 30, 37, 5,  45, 13, 53, 21, 61, 29, 36, 4,  44, 12, 52, 20, 60, 28, 35, 3,  43, 11,
                            51, 19, 59, 27, 34, 2,  42, 10, 50, 18, 58, 26, 33, 1,  41, 9,  49, 17, 57, 25 };

unsigned char key_tr1[] = { 57, 49, 41, 33, 25, 17, 9,  1,  58, 50, 42, 34, 26, 18, 10, 2,  59, 51, 43,
                            35, 27, 19, 11, 3,  60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7,  62, 54,
                            46, 38, 30, 22, 14, 6,  61, 53, 45, 37, 29, 21, 13, 5,  28, 20, 12, 4 };

unsigned char key_tr2[] = { 14, 17, 11, 24, 1,  5,  3,  28, 15, 6,  21, 10, 23, 19, 12, 4,
                            26, 8,  16, 7,  27, 20, 13, 2,  41, 52, 31, 37, 47, 55, 30, 40,
                            51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32 };


unsigned char rots[17] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1, 1 };

unsigned char e_trs[] = { 32, 1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,  8,  9,  10, 11,
                          12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21,
                          22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1 };

unsigned char p_tr[] = { 16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5,  18, 31, 10,
                         2,  8, 24, 14, 32, 27, 3,  9,  19, 13, 30, 6,  22, 11, 4,  25 };


unsigned char s[] = {
    14, 4,  13, 1,  2,  15, 11, 8,  3,  10, 6,  12, 5,  9,  0,  7,  0,  15, 7,  4,  14, 2,  13, 1,  10, 6,  12, 11, 9,
    5,  3,  8,  4,  1,  14, 8,  13, 6,  2,  11, 15, 12, 9,  7,  3,  10, 5,  0,  15, 12, 8,  2,  4,  9,  1,  7,  5,  11,
    3,  14, 10, 0,  6,  13, 15, 1,  8,  14, 6,  11, 3,  4,  9,  7,  2,  13, 12, 0,  5,  10, 3,  13, 4,  7,  15, 2,  8,
    14, 12, 0,  1,  10, 6,  9,  11, 5,  0,  14, 7,  11, 10, 4,  13, 1,  5,  8,  12, 6,  9,  3,  2,  15, 13, 8,  10, 1,
    3,  15, 4,  2,  11, 6,  7,  12, 0,  5,  14, 9,  10, 0,  9,  14, 6,  3,  15, 5,  1,  13, 12, 7,  11, 4,  2,  8,  13,
    7,  0,  9,  3,  4,  6,  10, 2,  8,  5,  14, 12, 11, 15, 1,  13, 6,  4,  9,  8,  15, 3,  0,  11, 1,  2,  12, 5,  10,
    14, 7,  1,  10, 13, 0,  6,  9,  8,  7,  4,  15, 14, 3,  11, 5,  2,  12, 7,  13, 14, 3,  0,  6,  9,  10, 1,  2,  8,
    5,  11, 12, 4,  15, 13, 8,  11, 5,  6,  15, 0,  3,  4,  7,  2,  12, 1,  10, 14, 9,  10, 6,  9,  0,  12, 11, 7,  13,
    15, 1,  3,  14, 5,  2,  8,  4,  3,  15, 0,  6,  10, 1,  13, 8,  9,  4,  5,  11, 12, 7,  2,  14, 2,  12, 4,  1,  7,
    10, 11, 6,  8,  5,  3,  15, 13, 0,  14, 9,  14, 11, 2,  12, 4,  7,  13, 1,  5,  0,  15, 10, 3,  9,  8,  6,  4,  2,
    1,  11, 10, 13, 7,  8,  15, 9,  12, 5,  6,  3,  0,  14, 11, 8,  12, 7,  1,  14, 2,  13, 6,  15, 0,  9,  10, 4,  5,
    3,  12, 1,  10, 15, 9,  2,  6,  8,  0,  13, 3,  4,  14, 7,  5,  11, 10, 15, 4,  2,  7,  12, 9,  5,  6,  1,  13, 14,
    0,  11, 3,  8,  9,  14, 15, 5,  2,  8,  12, 3,  7,  0,  4,  10, 1,  13, 11, 6,  4,  3,  2,  12, 9,  5,  15, 10, 11,
    14, 1,  7,  6,  0,  8,  13, 4,  11, 2,  14, 15, 0,  8,  13, 3,  12, 9,  7,  5,  10, 6,  1,  13, 0,  11, 7,  4,  9,
    1,  10, 14, 3,  5,  12, 2,  15, 8,  6,  1,  4,  11, 13, 12, 3,  7,  14, 10, 15, 6,  8,  0,  5,  9,  2,  6,  11, 13,
    8,  1,  4,  10, 7,  9,  5,  0,  15, 14, 2,  3,  12, 13, 2,  8,  4,  6,  15, 11, 1,  10, 9,  3,  14, 5,  0,  12, 7,
    1,  15, 13, 8,  10, 3,  7,  4,  12, 5,  6,  11, 0,  14, 9,  2,  7,  11, 4,  1,  9,  12, 14, 2,  0,  6,  10, 13, 15,
    3,  5,  8,  2,  1,  14, 7,  4,  10, 8,  13, 15, 12, 9,  0,  3,  5,  6,  11
};

void des(unsigned char *input, unsigned char *key, unsigned char *out, int flag)
{
    unsigned char plain_text[64], ikey[64], cipher_text[64];

    int i, j, m, n;
    unsigned char iikey[64], l[32], r[32], e[48];


    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            plain_text[8 * i + j] = (input[i] & (1 << (7 - j))) >> (7 - j);
            ikey[8 * i + j]       = (key[i] & (1 << (7 - j))) >> (7 - j);
        }
    }

    transfer(plain_text, init_tr, 64);
    transfer(ikey, key_tr1, 56);
    for (i = 0; i < 32; i++) {
        l[i] = plain_text[i];
        r[i] = plain_text[i + 32];
        e[i] = r[i];
    }
    if (flag == 1) {
        ikey[56] = ikey[28];
        ikey[28] = ikey[0];
        for (n = 0; n < 56; n++) {
            ikey[n]  = ikey[n + 1];
            iikey[n] = ikey[n];
        }
    }
    for (i = 0; i < 16; i++) {
        if (flag == 0) {
            for (m = 1; m <= rots[i]; m++) {
                ikey[56] = ikey[28];
                ikey[28] = ikey[0];
                for (n = 0; n < 56; n++) {
                    ikey[n]  = ikey[n + 1];
                    iikey[n] = ikey[n];
                }
            }
        }
        if (flag == 1) {
            for (m = 1; m <= rots[16 - i]; m++) {
                for (n = 56; n > 0; n--) {
                    ikey[n] = ikey[n - 1];
                }
                ikey[0]  = ikey[28];
                ikey[28] = ikey[56];
                for (n = 0; n < 56; n++)
                    iikey[n] = ikey[n];
            }
        }

        transfer(iikey, key_tr2, 48);
        transfer(e, e_trs, 48);
        for (n = 0; n < 48; n++)
            e[n] = e[n] ^ iikey[n];
        sf(e);
        transfer(e, p_tr, 32);
        for (n = 0; n < 32; n++) {
            e[n] = l[n] ^ e[n];
            l[n] = r[n];
            r[n] = e[n];
        }
    }
    for (i = 0; i < 32; i++) {
        *(cipher_text + i)      = r[i];
        *(cipher_text + 32 + i) = l[i];
    }
    transfer(cipher_text, fian_tr, 64);

    for (i = 0; i < 8; i++) {
        out[i] = 0;
        for (j = 0; j < 8; j++) {
            out[i] = out[i] | (cipher_text[8 * i + j] << (7 - j));
        }
    }
}


void sf(unsigned char *indt)
{
    int i, j;

    for (i = 0; i < 8; i++) {
        j = 32 * (*(indt + 6 * i)) + 16 * (*(indt + 6 * i + 5)) + 8 * (*(indt + 6 * i + 1)) +
            4 * (*(indt + 6 * i + 2)) + 2 * (*(indt + 6 * i + 3)) + (*(indt + 6 * i + 4));
        if (*(s + 64 * i + j) & 1)
            *(indt + 4 * i + 3) = 1;
        else
            *(indt + 4 * i + 3) = 0;
        if (*(s + 64 * i + j) & 2)
            *(indt + 4 * i + 2) = 1;
        else
            *(indt + 4 * i + 2) = 0;
        if (*(s + 64 * i + j) & 4)
            *(indt + 4 * i + 1) = 1;
        else
            *(indt + 4 * i + 1) = 0;
        if (*(s + 64 * i + j) & 8)
            *(indt + 4 * i) = 1;
        else
            *(indt + 4 * i) = 0;
    }
}

void transfer(unsigned char *dt_blk, unsigned char *tans_arry, unsigned char length)
{
    int i;
    unsigned char temp[64];

    for (i = 0; i < 64; i++) {
        temp[i] = dt_blk[i];
    }
    for (i = 0; i < length; i++) {
        *(dt_blk + i) = *(temp + tans_arry[i] - 1);
    }
}


void des_3(unsigned char *input, unsigned char *key, int keylen, unsigned char *out, int flag)
{
    unsigned char l_key[8];
    unsigned char m_key[8];
    unsigned char r_key[8];
    unsigned char intmp[8];
    unsigned char tmp[8];

    memcpy(intmp, input, 8);
    if (keylen == 8) {
        if (!flag) // enc
        {
            des(intmp, key, out, 0);
        } else // dec
        {
            des(intmp, key, out, 1);
        }
    } else if (keylen == 16) {
        memcpy(l_key, key, 8);
        memcpy(r_key, key + 8, 8);

        if (!flag) // enc
        {
            des(intmp, l_key, tmp, 0);
            des(tmp, r_key, intmp, 1);
            des(intmp, l_key, out, 0);
        } else // dec
        {
            des(intmp, l_key, tmp, 1);
            des(tmp, r_key, intmp, 0);
            des(intmp, l_key, out, 1);
        }
    } else {
        if (!flag) // enc
        {
            memcpy(l_key, key, 8);
            memcpy(m_key, key + 8, 8);
            memcpy(r_key, key + 16, 8);

            des(intmp, l_key, tmp, 0);
            des(tmp, m_key, intmp, 1);
            des(intmp, r_key, out, 0);
        } else // dec
        {
            memcpy(r_key, key, 8);
            memcpy(m_key, key + 8, 8);
            memcpy(l_key, key + 16, 8);

            des(intmp, l_key, tmp, 1);
            des(tmp, m_key, intmp, 0);
            des(intmp, r_key, out, 1);
        }
    }
}

#ifndef CONFIG_ENABLE_MBEDTLS_DES
int des_ecb_run(int flag, unsigned char *pKey, unsigned int KeyLen, unsigned char *pInData, unsigned int inDataLen,
                unsigned char *pOutData, unsigned int *pOutDataLen)
{
    int i    = 0;
    int loop = 0;

    *pOutDataLen = 0;

    if (KeyLen != 8 && KeyLen != 16 && KeyLen != 24) {
        return 1;
    }
    if (inDataLen % 8 != 0) {
        return 1;
    }

    loop = inDataLen / 8;
    for (i = 0; i < loop; i++) {
        des_3(pInData + i * 8, pKey, KeyLen, pOutData + i * 8, flag);
    }
    *pOutDataLen = inDataLen;
    return 0;
}

int des_cbc_run(int flag, unsigned char *pKey, unsigned int KeyLen, unsigned char *pInData, unsigned int inDataLen,
                unsigned char *pOutData, unsigned int *pOutDataLen, unsigned char *pIv)
{
    int i = 0, j = 0;
    int loop                = 0;
    unsigned char intmp[8]  = { 0 };
    unsigned char outtmp[8] = { 0 };
    unsigned char ivtmp[8]  = { 0 };

    *pOutDataLen = 0;

    if (KeyLen != 8 && KeyLen != 16 && KeyLen != 24) {
        return 1;
    }
    if (inDataLen % 8 != 0) {
        return 1;
    }
    memcpy(ivtmp, pIv, 8);
    loop = inDataLen / 8;
    for (i = 0; i < loop; i++) {
        if (!flag) // enc
        {
            memcpy(intmp, pInData + i * 8, 8);
            for (j = 0; j < 8; j++) {
                intmp[j] = intmp[j] ^ ivtmp[j];
            }
            des_3(intmp, pKey, KeyLen, outtmp, flag);
            memcpy(ivtmp, outtmp, 8);
            memcpy(pOutData + i * 8, outtmp, 8);
        } else // dec
        {
            memcpy(intmp, pInData + i * 8, 8);
            des_3(intmp, pKey, KeyLen, outtmp, flag);
            for (j = 0; j < 8; j++) {
                outtmp[j] = outtmp[j] ^ ivtmp[j];
            }
            memcpy(pOutData + i * 8, outtmp, 8);
            memcpy(ivtmp, intmp, 8);
        }
    }
    *pOutDataLen = inDataLen;
    return 0;
}

#else

    #include "mbedtls/des.h"

int des_ecb_run(int encdec, uint8_t *key, unsigned int keylen, uint8_t *indata, unsigned int indataLen,
                uint8_t *outdata, unsigned int *outdataLen)
{
    mbedtls_des_context ctx;
    mbedtls_des3_context ctx3;
    uint32_t i;

    encdec = !encdec;
    // 1-enc 0-dec

    mbedtls_des_init(&ctx);
    mbedtls_des3_init(&ctx3);

    if (keylen == 8) {
        if (encdec) {
            mbedtls_des_setkey_enc(&ctx, key);
        } else {
            mbedtls_des_setkey_dec(&ctx, key);
        }

        for (i = 0; i < indataLen;) {
            mbedtls_des_crypt_ecb(&ctx, indata, outdata);
            indata += 8;
            outdata += 8;
            i += 8;
        }
    } else if (keylen == 16) {
        if (encdec) {
            mbedtls_des3_set2key_enc(&ctx3, key);
        } else {
            mbedtls_des3_set2key_dec(&ctx3, key);
        }

        for (i = 0; i < indataLen;) {
            mbedtls_des3_crypt_ecb(&ctx3, indata, outdata);
            indata += 8;
            outdata += 8;
            i += 8;
        }
    } else if (keylen == 24) {
        if (encdec) {
            mbedtls_des3_set3key_enc(&ctx3, key);
        } else {
            mbedtls_des3_set3key_dec(&ctx3, key);
        }

        for (i = 0; i < indataLen;) {
            mbedtls_des3_crypt_ecb(&ctx3, indata, outdata);
            indata += 8;
            outdata += 8;
            i += 8;
        }
    }

    *outdataLen = indataLen;

    mbedtls_des_free(&ctx);
    mbedtls_des3_free(&ctx3);

    return 0;
}

int des_cbc_run(int encdec, uint8_t *key, unsigned int keylen, uint8_t *indata, unsigned int indataLen,
                uint8_t *outdata, unsigned int *outdataLen, uint8_t *iv)
{
    mbedtls_des_context ctx;
    mbedtls_des3_context ctx3;
    uint32_t i;
    uint8_t ucaiv[8];

    encdec = !encdec;
    // 1-enc 0-dec

    mbedtls_des_init(&ctx);
    mbedtls_des3_init(&ctx3);

    if (keylen == 8) {
        if (encdec) {
            mbedtls_des_setkey_enc(&ctx, key);
        } else {
            mbedtls_des_setkey_dec(&ctx, key);
        }

        memcpy(ucaiv, iv, 8);
        mbedtls_des_crypt_cbc(&ctx, encdec, indataLen, ucaiv, indata, outdata);
    } else if (keylen == 16) {
        if (encdec) {
            mbedtls_des3_set2key_enc(&ctx3, key);
        } else {
            mbedtls_des3_set2key_dec(&ctx3, key);
        }

        memcpy(ucaiv, iv, 8);
        mbedtls_des3_crypt_cbc(&ctx3, encdec, indataLen, ucaiv, indata, outdata);
    } else if (keylen == 24) {
        if (encdec) {
            mbedtls_des3_set3key_enc(&ctx3, key);
        } else {
            mbedtls_des3_set3key_dec(&ctx3, key);
        }

        memcpy(ucaiv, iv, 8);
        mbedtls_des3_crypt_cbc(&ctx3, encdec, indataLen, ucaiv, indata, outdata);
    }

    *outdataLen = indataLen;

    mbedtls_des_free(&ctx);
    mbedtls_des3_free(&ctx3);

    return 0;
}


int des_cfb_run(int flag, unsigned char *pKey, unsigned int KeyLen, unsigned char *pInData, unsigned int inDataLen,
                unsigned char *pOutData, unsigned int *pOutDataLen, unsigned char *pIv)
{
    int i = 0, j = 0;
    unsigned char intmp[8]  = { 0 };
    unsigned char outtmp[8] = { 0 };
    unsigned char ivtmp[8]  = { 0 };
    unsigned int uiLen;
    unsigned int uiOutDataLen;
    unsigned int uiInDataLen;

    *pOutDataLen = 0;
    uiInDataLen  = inDataLen;

    if (KeyLen != 8 && KeyLen != 16 && KeyLen != 24) {
        return 1;
    }

    memcpy(ivtmp, pIv, 8);
    while (inDataLen) {
        uiLen = inDataLen > 8 ? 8 : inDataLen;

        des_ecb_run(0, pKey, KeyLen, ivtmp, 8, outtmp, &uiOutDataLen);
        // des_3(ivtmp, pKey, KeyLen, outtmp, 0);
        memcpy(intmp, pInData, uiLen);
        for (j = 0; j < uiLen; j++) {
            outtmp[j] = outtmp[j] ^ intmp[j];
        }

        if (!flag) {
            memcpy(ivtmp, outtmp, 8);
        } else {
            memcpy(ivtmp, intmp, 8);
        }
        memcpy(pOutData, outtmp, uiLen);
        inDataLen -= uiLen;
        pInData += uiLen;
        pOutData += uiLen;
    }

    *pOutDataLen = uiInDataLen;

    return 0;
}

int des_ofb_run(int flag, unsigned char *pKey, unsigned int KeyLen, unsigned char *pInData, unsigned int inDataLen,
                unsigned char *pOutData, unsigned int *pOutDataLen, unsigned char *pIv)
{
    int i = 0, j = 0;
    unsigned char intmp[8]  = { 0 };
    unsigned char outtmp[8] = { 0 };
    unsigned char ivtmp[8]  = { 0 };
    unsigned int uiLen;
    unsigned int uiOutDataLen;
    unsigned int uiInDataLen;

    *pOutDataLen = 0;
    uiInDataLen  = inDataLen;

    if (KeyLen != 8 && KeyLen != 16 && KeyLen != 24) {
        return 1;
    }

    memcpy(ivtmp, pIv, 8);
    while (inDataLen) {
        uiLen = inDataLen > 8 ? 8 : inDataLen;

        des_ecb_run(0, pKey, KeyLen, ivtmp, 8, outtmp, &uiOutDataLen);
        // des_3(ivtmp, pKey, KeyLen, outtmp, 0);
        memcpy(intmp, pInData, uiLen);

        memcpy(ivtmp, outtmp, 8);
        for (j = 0; j < uiLen; j++) {
            outtmp[j] = outtmp[j] ^ intmp[j];
        }

        memcpy(pOutData, outtmp, uiLen);
        inDataLen -= uiLen;
        pInData += uiLen;
        pOutData += uiLen;
    }

    *pOutDataLen = uiInDataLen;

    return 0;
}

#endif
