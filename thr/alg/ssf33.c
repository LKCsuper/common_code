#include <string.h>

#define SSF33 0x11

#define SSF33_ROTL16(X, n) (unsigned short int)((((X) << n)) | ((X) >> (16 - n)))
#define SSF33_ROTR16(X, n) (unsigned short int)((((X) >> n)) | ((X) << (16 - n)))

static unsigned SSF33_SBox_0[16] = {0xF, 0x4, 0x0, 0x3, 0x5, 0xB, 0x6, 0x8, 0x9, 0x2, 0x7, 0xE, 0xA, 0xC, 0xD, 0x1};
static unsigned SSF33_SBox_1[16] = {0x2, 0x1, 0x5, 0xA, 0xB, 0xE, 0x8, 0x7, 0xC, 0xF, 0x9, 0x4, 0x0, 0x3, 0x6, 0xD};
static unsigned SSF33_SBox_2[16] = {0x8, 0xF, 0x7, 0x0, 0x6, 0x1, 0xD, 0xB, 0x5, 0xC, 0x2, 0x9, 0x3, 0xA, 0xE, 0x4};
static unsigned SSF33_SBox_3[16] = {0x4, 0xA, 0x8, 0x6, 0x1, 0x7, 0xE, 0xD, 0x3, 0x0, 0x5, 0xF, 0xC, 0x9, 0xB, 0x2};
static unsigned SSF33_SBox_4[16] = {0x7, 0x0, 0x8, 0x3, 0x9, 0x5, 0x4, 0xE, 0xB, 0x6, 0xD, 0xA, 0xC, 0xF, 0x2, 0x1};
static unsigned SSF33_SBox_5[16] = {0x1, 0x6, 0x2, 0xC, 0x7, 0xA, 0xD, 0x3, 0xE, 0x5, 0x8, 0xF, 0x0, 0x9, 0xB, 0x4};
static unsigned SSF33_SBox_6[16] = {0xC, 0x3, 0xB, 0xD, 0x5, 0x0, 0xE, 0x7, 0x2, 0x8, 0x4, 0x1, 0xF, 0x6, 0x9, 0xA};
static unsigned SSF33_SBox_7[16] = {0x2, 0xC, 0xE, 0x1, 0x8, 0xF, 0x3, 0xA, 0xD, 0xB, 0x7, 0x4, 0x6, 0x5, 0x0, 0x9};

static unsigned char SK_init[16] = {0xbb, 0x1f, 0x73, 0x32, 0xcd, 0x41, 0x30, 0xf5,
                                    0x76, 0x28, 0x64, 0x1b, 0xf6, 0xaf, 0x74, 0xb8};

static void SSF33_F(unsigned long int *res, unsigned long int *L, unsigned long int *R, unsigned long int *roundKey)
{
    unsigned long int tmp[2];
    unsigned long int A, B, C, D;
    unsigned short int a, b, c, d, U1, U2, V1, V2, T;
    unsigned char aa[8], bb[8];
    int i;

    A = L[0];
    B = L[1];
    C = R[0];
    D = R[1];

    A = A ^ roundKey[0];
    B = B ^ roundKey[1];

    for (i = 0; i < 8; i++) {
        aa[i] = (A >> (32 - 4 - i * 4)) & 0xF;
        bb[i] = (B >> (32 - 4 - i * 4)) & 0xF;
    }

    a = (((unsigned short int)(SSF33_SBox_0[aa[6]])) << 12) | (((unsigned short int)(SSF33_SBox_1[aa[4]])) << 8) |
        (((unsigned short int)(SSF33_SBox_2[aa[7]])) << 4) | ((unsigned short int)(SSF33_SBox_3[aa[5]]));
    b = (((unsigned short int)(SSF33_SBox_4[bb[5]])) << 12) | (((unsigned short int)(SSF33_SBox_5[bb[7]])) << 8) |
        (((unsigned short int)(SSF33_SBox_6[bb[4]])) << 4) | ((unsigned short int)(SSF33_SBox_7[bb[6]]));
    c = (((unsigned short int)(SSF33_SBox_0[aa[1]])) << 12) | (((unsigned short int)(SSF33_SBox_1[aa[3]])) << 8) |
        (((unsigned short int)(SSF33_SBox_2[aa[2]])) << 4) | ((unsigned short int)(SSF33_SBox_3[aa[0]]));
    d = (((unsigned short int)(SSF33_SBox_4[bb[2]])) << 12) | (((unsigned short int)(SSF33_SBox_5[bb[0]])) << 8) |
        (((unsigned short int)(SSF33_SBox_6[bb[3]])) << 4) | ((unsigned short int)(SSF33_SBox_7[bb[1]]));

    T = a ^ SSF33_ROTR16(c, 2);
    U2 = (b ^ SSF33_ROTR16(d, 5)) + SSF33_ROTL16(T, 7);
    U1 = T + U2;

    T = c ^ b;
    V2 = (d ^ a) + SSF33_ROTL16(T, 3);
    V1 = T + V2;

    A = (((unsigned long int)(U1))) << 16 | ((unsigned long int)(U2));
    B = (((unsigned long int)(V1))) << 16 | ((unsigned long int)(V2));
    tmp[0] = A ^ C;
    tmp[1] = B ^ D;
    memcpy(res, tmp, sizeof(unsigned long int) * 2);
}

static void SSF33_ShiftLeft7(unsigned long int *res, unsigned long int *a)
{
    unsigned long int tmp[4];

    tmp[0] = (a[0] << 7) | (a[1] >> (32 - 7));
    tmp[1] = (a[1] << 7) | (a[2] >> (32 - 7));
    tmp[2] = (a[2] << 7) | (a[3] >> (32 - 7));
    tmp[3] = (a[3] << 7) | (a[0] >> (32 - 7));

    memcpy(res, tmp, sizeof(unsigned long int) * 4);
}

static void SSF33_layerKey(unsigned long int *BK, unsigned long int *Key, int num)
{
    unsigned long int tmp[42], res[4];
    int i;

    memcpy(res, Key, sizeof(unsigned long int) * 4);
    for (i = 0; i < num; i++) {
        tmp[4 * i] = res[0];
        tmp[4 * i + 1] = res[1];
        tmp[4 * i + 2] = res[2];
        tmp[4 * i + 3] = res[3];
        SSF33_ShiftLeft7(res, res);
    }
    memcpy(BK, tmp, sizeof(unsigned long int) * num * 4);
}
static void SSF33_InitKey(unsigned long int *InitKey, unsigned long int *Key, unsigned long int *SK)
{
    unsigned long int subRoundKey[18];
    unsigned long int tmp_L[2], L[2], R[2];
    int num;
    int i;
    num = 4;

    SSF33_layerKey(subRoundKey, SK, num);

    L[0] = Key[0];
    L[1] = Key[1];
    R[0] = Key[2];
    R[1] = Key[3];
    for (i = 0; i < 8; i++) {
        SSF33_F(tmp_L, L, R, &subRoundKey[2 * i]);
        memcpy(R, L, sizeof(unsigned long int) * 2);
        memcpy(L, tmp_L, sizeof(unsigned long int) * 2);
    }
    InitKey[0] = L[0];
    InitKey[1] = L[1];
    InitKey[2] = R[0];
    InitKey[3] = R[1];
}

static void SSF33_CalcRoundKey(unsigned long int *roundKey, unsigned long int *Key, unsigned long int *SK,
                               unsigned long int keyflag)
{
    unsigned long int InitKey[4];
    unsigned long int tmp[42];
    int len = 40;
    int i;

    SSF33_InitKey(InitKey, Key, SK);

    SSF33_layerKey(tmp, InitKey, 10);

    if (keyflag == 0) // encrypt
    {
        for (i = 0; i < len; i++) {
            roundKey[i] = tmp[i];
        }
    } else // decrypt
    {
        memcpy(&roundKey[0], &tmp[36], sizeof(unsigned long int) * 4);
        for (i = 1; i < 17; i++) {
            memcpy(&roundKey[2 + 2 * i], &tmp[38 - 2 - 2 * i], sizeof(unsigned long int) * 2);
        }
        memcpy(&roundKey[36], &tmp[0], sizeof(unsigned long int) * 4);
    }
}

static unsigned int SSF33_Encrypt_ECB(unsigned char *Key_byte, unsigned char *input, unsigned char *output,
                                      unsigned long int len)
{
    unsigned long int L[2], R[2], tmp_L[2], tmp_R[2], res[4], Key[4], SK[4], In[4];
    unsigned long int roundkey[42];
    unsigned char SK_byte[16];
    int SSF33_round = 16;
    int keyflag = 0;
    int i, j;

    if (len % 16 != 0) {
        return 1;
    }
    // 	if( SK_flag==1 )
    // 	{
    // 		memcpy(SK_byte, SK0, sizeof(unsigned char)*16);
    // 	}
    // 	else
    // 	{
    memcpy(SK_byte, SK_init, 16);
    // 	}
    for (i = 0; i < 4; i++) {
        SK[i] = 0x0;
        Key[i] = 0x0;
        for (j = 0; j < 4; j++) {
            SK[i] = (SK[i] << 8) | ((unsigned long int)(SK_byte[4 * i + j]));
            Key[i] = (Key[i] << 8) | ((unsigned long int)(Key_byte[4 * i + j]));
        }
    }

    SSF33_CalcRoundKey(roundkey, Key, SK, keyflag);

    for (j = 0; j < len / 16; j++) {
        In[0] = (((unsigned long int)input[j * 16]) << 24) | (((unsigned long int)input[j * 16 + 1]) << 16) |
                (((unsigned long int)input[j * 16 + 2]) << 8) | ((unsigned long int)input[j * 16 + 3]);
        In[1] = (((unsigned long int)input[j * 16 + 4]) << 24) | (((unsigned long int)input[j * 16 + 5]) << 16) |
                (((unsigned long int)input[j * 16 + 6]) << 8) | ((unsigned long int)input[j * 16 + 7]);
        In[2] = (((unsigned long int)input[j * 16 + 8]) << 24) | (((unsigned long int)input[j * 16 + 9]) << 16) |
                (((unsigned long int)input[j * 16 + 10]) << 8) | ((unsigned long int)input[j * 16 + 11]);
        In[3] = (((unsigned long int)input[j * 16 + 12]) << 24) | (((unsigned long int)input[j * 16 + 13]) << 16) |
                (((unsigned long int)input[j * 16 + 14]) << 8) | ((unsigned long int)input[j * 16 + 15]);

        L[0] = In[0] ^ roundkey[0];
        L[1] = In[1] ^ roundkey[1];
        R[0] = In[2] ^ roundkey[2];
        R[1] = In[3] ^ roundkey[3];

        for (i = 0; i < SSF33_round - 1; i++) {
            SSF33_F(tmp_L, L, R, &roundkey[2 * (i + 1) + 2]);
            R[0] = L[0];
            R[1] = L[1];
            //	memcpy(R, L, sizeof(unsigned long int)*2);
            L[0] = tmp_L[0];
            L[1] = tmp_L[1];
            //	memcpy(L, tmp_L, sizeof(unsigned long int)*2);
        }
        SSF33_F(tmp_R, L, R, &roundkey[2 * (i + 1) + 2]);
        R[0] = tmp_R[0];
        R[1] = tmp_R[1];
        //	memcpy(R, tmp_R, sizeof(unsigned long int)*2);

        res[0] = L[0] ^ roundkey[2 * SSF33_round + 4];
        res[1] = L[1] ^ roundkey[2 * SSF33_round + 5];
        res[2] = R[0] ^ roundkey[2 * SSF33_round + 6];
        res[3] = R[1] ^ roundkey[2 * SSF33_round + 7];

        output[16 * j] = (unsigned char)((res[0] >> 24) & 0xFF);
        output[16 * j + 1] = (unsigned char)((res[0] >> 16) & 0xFF);
        output[16 * j + 2] = (unsigned char)((res[0] >> 8) & 0xFF);
        output[16 * j + 3] = (unsigned char)(res[0] & 0xFF);
        output[16 * j + 4] = (unsigned char)((res[1] >> 24) & 0xFF);
        output[16 * j + 5] = (unsigned char)((res[1] >> 16) & 0xFF);
        output[16 * j + 6] = (unsigned char)((res[1] >> 8) & 0xFF);
        output[16 * j + 7] = (unsigned char)(res[1] & 0xFF);
        output[16 * j + 8] = (unsigned char)((res[2] >> 24) & 0xFF);
        output[16 * j + 9] = (unsigned char)((res[2] >> 16) & 0xFF);
        output[16 * j + 10] = (unsigned char)((res[2] >> 8) & 0xFF);
        output[16 * j + 11] = (unsigned char)(res[2] & 0xFF);
        output[16 * j + 12] = (unsigned char)((res[3] >> 24) & 0xFF);
        output[16 * j + 13] = (unsigned char)((res[3] >> 16) & 0xFF);
        output[16 * j + 14] = (unsigned char)((res[3] >> 8) & 0xFF);
        output[16 * j + 15] = (unsigned char)(res[3] & 0xFF);
    }
    return 0;
}

static unsigned int SSF33_Decrypt_ECB(unsigned char *Key_byte, unsigned char *input, unsigned char *output,
                                      unsigned long int len)
{
    unsigned long int L[2], R[2], tmp_L[2], tmp_R[2], res[4], Key[4], SK[4], In[4];
    unsigned long int roundkey[42];
    unsigned char SK_byte[16];
    int SSF33_round = 16;
    int keyflag = 1;
    int i, j;

    if (len % 16 != 0) {
        return 1;
    }
    // 	if( SK_flag==1 )
    // 	{
    // 		memcpy(SK_byte, SK0, sizeof(unsigned char)*16);
    // 	}
    // 	else
    // 	{
    memcpy(SK_byte, SK_init, sizeof(unsigned char) * 16);
    // 	}
    for (i = 0; i < 4; i++) {
        SK[i] = 0x0;
        Key[i] = 0x0;
        for (j = 0; j < 4; j++) {
            SK[i] = (SK[i] << 8) | ((unsigned long int)(SK_byte[4 * i + j]));
            Key[i] = (Key[i] << 8) | ((unsigned long int)(Key_byte[4 * i + j]));
        }
    }

    SSF33_CalcRoundKey(roundkey, Key, SK, keyflag);

    for (j = 0; j < len / 16; j++) {
        In[0] = (((unsigned long int)input[j * 16]) << 24) | (((unsigned long int)input[j * 16 + 1]) << 16) |
                (((unsigned long int)input[j * 16 + 2]) << 8) | ((unsigned long int)input[j * 16 + 3]);
        In[1] = (((unsigned long int)input[j * 16 + 4]) << 24) | (((unsigned long int)input[j * 16 + 5]) << 16) |
                (((unsigned long int)input[j * 16 + 6]) << 8) | ((unsigned long int)input[j * 16 + 7]);
        In[2] = (((unsigned long int)input[j * 16 + 8]) << 24) | (((unsigned long int)input[j * 16 + 9]) << 16) |
                (((unsigned long int)input[j * 16 + 10]) << 8) | ((unsigned long int)input[j * 16 + 11]);
        In[3] = (((unsigned long int)input[j * 16 + 12]) << 24) | (((unsigned long int)input[j * 16 + 13]) << 16) |
                (((unsigned long int)input[j * 16 + 14]) << 8) | ((unsigned long int)input[j * 16 + 15]);

        L[0] = In[0] ^ roundkey[0];
        L[1] = In[1] ^ roundkey[1];
        R[0] = In[2] ^ roundkey[2];
        R[1] = In[3] ^ roundkey[3];

        for (i = 0; i < SSF33_round - 1; i++) {
            SSF33_F(tmp_L, L, R, &roundkey[2 * (i + 1) + 2]);
            //	memcpy(R, L, sizeof(unsigned long int)*2);
            R[0] = L[0];
            R[1] = L[1];
            //	memcpy(L, tmp_L, sizeof(unsigned long int)*2);
            L[0] = tmp_L[0];
            L[1] = tmp_L[1];
        }
        SSF33_F(tmp_R, L, R, &roundkey[2 * (i + 1) + 2]);
        //	memcpy(R, tmp_R, sizeof(unsigned long int)*2);
        R[0] = tmp_R[0];
        R[1] = tmp_R[1];

        res[0] = L[0] ^ roundkey[2 * SSF33_round + 4];
        res[1] = L[1] ^ roundkey[2 * SSF33_round + 5];
        res[2] = R[0] ^ roundkey[2 * SSF33_round + 6];
        res[3] = R[1] ^ roundkey[2 * SSF33_round + 7];

        output[16 * j] = (unsigned char)((res[0] >> 24) & 0xFF);
        output[16 * j + 1] = (unsigned char)((res[0] >> 16) & 0xFF);
        output[16 * j + 2] = (unsigned char)((res[0] >> 8) & 0xFF);
        output[16 * j + 3] = (unsigned char)(res[0] & 0xFF);
        output[16 * j + 4] = (unsigned char)((res[1] >> 24) & 0xFF);
        output[16 * j + 5] = (unsigned char)((res[1] >> 16) & 0xFF);
        output[16 * j + 6] = (unsigned char)((res[1] >> 8) & 0xFF);
        output[16 * j + 7] = (unsigned char)(res[1] & 0xFF);
        output[16 * j + 8] = (unsigned char)((res[2] >> 24) & 0xFF);
        output[16 * j + 9] = (unsigned char)((res[2] >> 16) & 0xFF);
        output[16 * j + 10] = (unsigned char)((res[2] >> 8) & 0xFF);
        output[16 * j + 11] = (unsigned char)(res[2] & 0xFF);
        output[16 * j + 12] = (unsigned char)((res[3] >> 24) & 0xFF);
        output[16 * j + 13] = (unsigned char)((res[3] >> 16) & 0xFF);
        output[16 * j + 14] = (unsigned char)((res[3] >> 8) & 0xFF);
        output[16 * j + 15] = (unsigned char)(res[3] & 0xFF);

        //	memcpy(&output[4*j], res, sizeof(unsigned long int)*4);
    }
    return 0;
}

static unsigned int SSF33_Encrypt_CBC(unsigned char *Key_byte, unsigned char *input, unsigned char *output,
                                      unsigned long int len, unsigned char *IV)
{
    unsigned long int L[2], R[2], tmp_L[2], tmp_R[2], res[4], Key[4], SK[4], In[4];
    unsigned long int roundkey[42];
    unsigned char SK_byte[16], tmp_IV[16];
    int SSF33_round = 16;
    int keyflag = 0;
    int i, j;

    if (len % 16 != 0) {
        return 1;
    }
    // 	if( SK_flag==1 )
    // 	{
    // 		memcpy(SK_byte, SK0, sizeof(unsigned char)*16);
    // 	}
    // 	else
    // 	{
    memcpy(SK_byte, SK_init, sizeof(unsigned char) * 16);
    // 	}
    for (i = 0; i < 4; i++) {
        SK[i] = 0x0;
        Key[i] = 0x0;
        for (j = 0; j < 4; j++) {
            SK[i] = (SK[i] << 8) | ((unsigned long int)(SK_byte[4 * i + j]));
            Key[i] = (Key[i] << 8) | ((unsigned long int)(Key_byte[4 * i + j]));
        }
    }

    SSF33_CalcRoundKey(roundkey, Key, SK, keyflag);
    memcpy((unsigned char *)tmp_IV, IV, sizeof(unsigned char) * 16);
    //	memcpy(B_tmp_IV, IV, 16);

    for (j = 0; j < len / 16; j++) {
        In[0] = (((unsigned long int)input[j * 16]) << 24) | (((unsigned long int)input[j * 16 + 1]) << 16) |
                (((unsigned long int)input[j * 16 + 2]) << 8) | ((unsigned long int)input[j * 16 + 3]);
        In[1] = (((unsigned long int)input[j * 16 + 4]) << 24) | (((unsigned long int)input[j * 16 + 5]) << 16) |
                (((unsigned long int)input[j * 16 + 6]) << 8) | ((unsigned long int)input[j * 16 + 7]);
        In[2] = (((unsigned long int)input[j * 16 + 8]) << 24) | (((unsigned long int)input[j * 16 + 9]) << 16) |
                (((unsigned long int)input[j * 16 + 10]) << 8) | ((unsigned long int)input[j * 16 + 11]);
        In[3] = (((unsigned long int)input[j * 16 + 12]) << 24) | (((unsigned long int)input[j * 16 + 13]) << 16) |
                (((unsigned long int)input[j * 16 + 14]) << 8) | ((unsigned long int)input[j * 16 + 15]);

        In[0] = In[0] ^ ((((unsigned long int)tmp_IV[0]) << 24) | (((unsigned long int)tmp_IV[1]) << 16) |
                         (((unsigned long int)tmp_IV[2]) << 8) | ((unsigned long int)tmp_IV[3]));
        In[1] = In[1] ^ ((((unsigned long int)tmp_IV[4]) << 24) | (((unsigned long int)tmp_IV[5]) << 16) |
                         (((unsigned long int)tmp_IV[6]) << 8) | ((unsigned long int)tmp_IV[7]));
        In[2] = In[2] ^ ((((unsigned long int)tmp_IV[8]) << 24) | (((unsigned long int)tmp_IV[9]) << 16) |
                         (((unsigned long int)tmp_IV[10]) << 8) | ((unsigned long int)tmp_IV[11]));
        In[3] = In[3] ^ ((((unsigned long int)tmp_IV[12]) << 24) | (((unsigned long int)tmp_IV[13]) << 16) |
                         (((unsigned long int)tmp_IV[14]) << 8) | ((unsigned long int)tmp_IV[15]));

        L[0] = In[0] ^ roundkey[0];
        L[1] = In[1] ^ roundkey[1];
        R[0] = In[2] ^ roundkey[2];
        R[1] = In[3] ^ roundkey[3];

        for (i = 0; i < SSF33_round - 1; i++) {
            SSF33_F(tmp_L, L, R, &roundkey[2 * (i + 1) + 2]);
            R[0] = L[0];
            R[1] = L[1];
            //	memcpy(R, L, sizeof(unsigned long int)*2);
            L[0] = tmp_L[0];
            L[1] = tmp_L[1];
            //	memcpy(L, tmp_L, sizeof(unsigned long int)*2);
        }
        SSF33_F(tmp_R, L, R, &roundkey[2 * (i + 1) + 2]);
        R[0] = tmp_R[0];
        R[1] = tmp_R[1];
        //	memcpy(R, tmp_R, sizeof(unsigned long int)*2);

        res[0] = L[0] ^ roundkey[2 * SSF33_round + 4];
        res[1] = L[1] ^ roundkey[2 * SSF33_round + 5];
        res[2] = R[0] ^ roundkey[2 * SSF33_round + 6];
        res[3] = R[1] ^ roundkey[2 * SSF33_round + 7];

        output[16 * j] = (unsigned char)((res[0] >> 24) & 0xFF);
        output[16 * j + 1] = (unsigned char)((res[0] >> 16) & 0xFF);
        output[16 * j + 2] = (unsigned char)((res[0] >> 8) & 0xFF);
        output[16 * j + 3] = (unsigned char)(res[0] & 0xFF);
        output[16 * j + 4] = (unsigned char)((res[1] >> 24) & 0xFF);
        output[16 * j + 5] = (unsigned char)((res[1] >> 16) & 0xFF);
        output[16 * j + 6] = (unsigned char)((res[1] >> 8) & 0xFF);
        output[16 * j + 7] = (unsigned char)(res[1] & 0xFF);
        output[16 * j + 8] = (unsigned char)((res[2] >> 24) & 0xFF);
        output[16 * j + 9] = (unsigned char)((res[2] >> 16) & 0xFF);
        output[16 * j + 10] = (unsigned char)((res[2] >> 8) & 0xFF);
        output[16 * j + 11] = (unsigned char)(res[2] & 0xFF);
        output[16 * j + 12] = (unsigned char)((res[3] >> 24) & 0xFF);
        output[16 * j + 13] = (unsigned char)((res[3] >> 16) & 0xFF);
        output[16 * j + 14] = (unsigned char)((res[3] >> 8) & 0xFF);
        output[16 * j + 15] = (unsigned char)(res[3] & 0xFF);
        memcpy(tmp_IV, &output[16 * j], sizeof(unsigned char) * 16);
    }
    return 0;
}

static unsigned int SSF33_Decrypt_CBC(unsigned char *Key_byte, unsigned char *input, unsigned char *output,
                                      unsigned long int len, unsigned char *IV)
{
    unsigned long int L[2], R[2], tmp_L[2], tmp_R[2], res[4], Key[4], SK[4], In[4], B_tmp_IV[4];
    unsigned long int roundkey[42];
    unsigned char SK_byte[16], tmp_IV[16];
    int SSF33_round = 16;
    int keyflag = 1;
    int i, j;

    if (len % 16 != 0) {
        return 1;
    }
    // 	if( SK_flag==1 )
    // 	{
    // 		memcpy(SK_byte, SK0, sizeof(unsigned char)*16);
    // 	}
    // 	else
    // 	{
    memcpy(SK_byte, SK_init, sizeof(unsigned char) * 16);
    // 	}
    for (i = 0; i < 4; i++) {
        SK[i] = 0x0;
        Key[i] = 0x0;
        for (j = 0; j < 4; j++) {
            SK[i] = (SK[i] << 8) | ((unsigned long int)(SK_byte[4 * i + j]));
            Key[i] = (Key[i] << 8) | ((unsigned long int)(Key_byte[4 * i + j]));
        }
    }

    SSF33_CalcRoundKey(roundkey, Key, SK, keyflag);
    memcpy(tmp_IV, IV, sizeof(unsigned char) * 16);
    for (j = 0; j < len / 16; j++) {
        In[0] = (((unsigned long int)input[j * 16]) << 24) | (((unsigned long int)input[j * 16 + 1]) << 16) |
                (((unsigned long int)input[j * 16 + 2]) << 8) | ((unsigned long int)input[j * 16 + 3]);
        In[1] = (((unsigned long int)input[j * 16 + 4]) << 24) | (((unsigned long int)input[j * 16 + 5]) << 16) |
                (((unsigned long int)input[j * 16 + 6]) << 8) | ((unsigned long int)input[j * 16 + 7]);
        In[2] = (((unsigned long int)input[j * 16 + 8]) << 24) | (((unsigned long int)input[j * 16 + 9]) << 16) |
                (((unsigned long int)input[j * 16 + 10]) << 8) | ((unsigned long int)input[j * 16 + 11]);
        In[3] = (((unsigned long int)input[j * 16 + 12]) << 24) | (((unsigned long int)input[j * 16 + 13]) << 16) |
                (((unsigned long int)input[j * 16 + 14]) << 8) | ((unsigned long int)input[j * 16 + 15]);
        B_tmp_IV[0] = In[0];
        B_tmp_IV[1] = In[1];
        B_tmp_IV[2] = In[2];
        B_tmp_IV[3] = In[3];

        L[0] = In[0] ^ roundkey[0];
        L[1] = In[1] ^ roundkey[1];
        R[0] = In[2] ^ roundkey[2];
        R[1] = In[3] ^ roundkey[3];

        for (i = 0; i < SSF33_round - 1; i++) {
            SSF33_F(tmp_L, L, R, &roundkey[2 * (i + 1) + 2]);
            //	memcpy(R, L, sizeof(unsigned long int)*2);
            R[0] = L[0];
            R[1] = L[1];
            //	memcpy(L, tmp_L, sizeof(unsigned long int)*2);
            L[0] = tmp_L[0];
            L[1] = tmp_L[1];
        }
        SSF33_F(tmp_R, L, R, &roundkey[2 * (i + 1) + 2]);
        //	memcpy(R, tmp_R, sizeof(unsigned long int)*2);
        R[0] = tmp_R[0];
        R[1] = tmp_R[1];

        res[0] = L[0] ^ roundkey[2 * SSF33_round + 4];
        res[1] = L[1] ^ roundkey[2 * SSF33_round + 5];
        res[2] = R[0] ^ roundkey[2 * SSF33_round + 6];
        res[3] = R[1] ^ roundkey[2 * SSF33_round + 7];
        // memcpy(B_tmp_IV,&res[0],16);

        res[0] = res[0] ^ ((((unsigned long int)tmp_IV[0]) << 24) | (((unsigned long int)tmp_IV[1]) << 16) |
                           (((unsigned long int)tmp_IV[2]) << 8) | ((unsigned long int)tmp_IV[3]));
        res[1] = res[1] ^ ((((unsigned long int)tmp_IV[4]) << 24) | (((unsigned long int)tmp_IV[5]) << 16) |
                           (((unsigned long int)tmp_IV[6]) << 8) | ((unsigned long int)tmp_IV[7]));
        res[2] = res[2] ^ ((((unsigned long int)tmp_IV[8]) << 24) | (((unsigned long int)tmp_IV[9]) << 16) |
                           (((unsigned long int)tmp_IV[10]) << 8) | ((unsigned long int)tmp_IV[11]));
        res[3] = res[3] ^ ((((unsigned long int)tmp_IV[12]) << 24) | (((unsigned long int)tmp_IV[13]) << 16) |
                           (((unsigned long int)tmp_IV[14]) << 8) | ((unsigned long int)tmp_IV[15]));

        output[16 * j] = (unsigned char)((res[0] >> 24) & 0xFF);
        output[16 * j + 1] = (unsigned char)((res[0] >> 16) & 0xFF);
        output[16 * j + 2] = (unsigned char)((res[0] >> 8) & 0xFF);
        output[16 * j + 3] = (unsigned char)(res[0] & 0xFF);
        output[16 * j + 4] = (unsigned char)((res[1] >> 24) & 0xFF);
        output[16 * j + 5] = (unsigned char)((res[1] >> 16) & 0xFF);
        output[16 * j + 6] = (unsigned char)((res[1] >> 8) & 0xFF);
        output[16 * j + 7] = (unsigned char)(res[1] & 0xFF);
        output[16 * j + 8] = (unsigned char)((res[2] >> 24) & 0xFF);
        output[16 * j + 9] = (unsigned char)((res[2] >> 16) & 0xFF);
        output[16 * j + 10] = (unsigned char)((res[2] >> 8) & 0xFF);
        output[16 * j + 11] = (unsigned char)(res[2] & 0xFF);
        output[16 * j + 12] = (unsigned char)((res[3] >> 24) & 0xFF);
        output[16 * j + 13] = (unsigned char)((res[3] >> 16) & 0xFF);
        output[16 * j + 14] = (unsigned char)((res[3] >> 8) & 0xFF);
        output[16 * j + 15] = (unsigned char)(res[3] & 0xFF);

        tmp_IV[0] = (unsigned char)((B_tmp_IV[0] >> 24) & 0xFF);
        tmp_IV[1] = (unsigned char)((B_tmp_IV[0] >> 16) & 0xFF);
        tmp_IV[2] = (unsigned char)((B_tmp_IV[0] >> 8) & 0xFF);
        tmp_IV[3] = (unsigned char)(B_tmp_IV[0] & 0xFF);
        tmp_IV[4] = (unsigned char)((B_tmp_IV[1] >> 24) & 0xFF);
        tmp_IV[5] = (unsigned char)((B_tmp_IV[1] >> 16) & 0xFF);
        tmp_IV[6] = (unsigned char)((B_tmp_IV[1] >> 8) & 0xFF);
        tmp_IV[7] = (unsigned char)(B_tmp_IV[1] & 0xFF);
        tmp_IV[8] = (unsigned char)((B_tmp_IV[2] >> 24) & 0xFF);
        tmp_IV[9] = (unsigned char)((B_tmp_IV[2] >> 16) & 0xFF);
        tmp_IV[10] = (unsigned char)((B_tmp_IV[2] >> 8) & 0xFF);
        tmp_IV[11] = (unsigned char)(B_tmp_IV[2] & 0xFF);
        tmp_IV[12] = (unsigned char)((B_tmp_IV[3] >> 24) & 0xFF);
        tmp_IV[13] = (unsigned char)((B_tmp_IV[3] >> 16) & 0xFF);
        tmp_IV[14] = (unsigned char)((B_tmp_IV[3] >> 8) & 0xFF);
        tmp_IV[15] = (unsigned char)(B_tmp_IV[3] & 0xFF);
        //		memcpy(tmp_IV,B_tmp_IV,16);
        //		memcpy(B_tmp_IV,&output[4*j],16);


        //	memcpy(&output[4*j], res, sizeof(unsigned long int)*4);
    }
    return 0;
}

unsigned int SSF33_crypto(unsigned int isenc, unsigned char *pucaKey, unsigned char *pucaIv, unsigned char *pucaInData,
                          unsigned int uiInDataLen, unsigned char *pucaOutData)
{
    int ret = 0;

    if (pucaIv == NULL) {
        if (isenc) {
            ret = SSF33_Encrypt_ECB(pucaKey, pucaInData, pucaOutData, uiInDataLen);
        } else {
            ret = SSF33_Decrypt_ECB(pucaKey, pucaInData, pucaOutData, uiInDataLen);
        }
    } else {
        if (isenc) {
            ret = SSF33_Encrypt_CBC(pucaKey, pucaInData, pucaOutData, uiInDataLen, pucaIv);
        } else {
            ret = SSF33_Decrypt_CBC(pucaKey, pucaInData, pucaOutData, uiInDataLen, pucaIv);
        }
    }

    return ret;
}