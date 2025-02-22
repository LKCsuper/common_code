/*
 * @Description: ???
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2024-05-11 16:56:54
 * @LastEditors: Please set LastEditors
 * @detail:
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
//#include "common.h"
//#include "Zayk_Algo.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
// typedef struct HMacCtx_st {
//     int uiAlgId;
//     void *HashCtx;
//     unsigned char opad[128];
//     int block_size;
//     int out_size;
// } HMAC_CTX_ST;

// typedef struct HashCtx_st {
//     int uiAlgId;
// } HASH_CTX_ST;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
// static HMAC_CTX_ST gstHmacCtx;
// static HASH_CTX_ST gstHashCtx;

// HASH_CTX hash_sm3;
// HASH_CTX hash_sh1;
// HASH_CTX hash_sh256;
// HASH_CTX hash_sh384;
// HASH_CTX hash_sh512;
// /* Private function prototypes -----------------------------------------------*/
// /* Private functions ---------------------------------------------------------*/
// /**
//  * @description:
//  * @detail:
//  * @return {*}
//  * @author: lkc
//  */
// ULONG ZAYK_HashInit(ULONG ulAlgoId)
// {
//     switch (ulAlgoId) {
//         case SGD_SM3:
//             Hash_Init(&hash_sm3, SM3_256);
//             break;
//         case SGD_SHA1:
//             Hash_Init(&hash_sh1, SHA_1);
//             break;
//         case SGD_SHA256:
//             Hash_Init(&hash_sh256, SHA_256);
//             break;
//         case SGD_SHA512:
//             Hash_Init(&hash_sh512, SHA_512);
//             break;
//         case SGD_SHA384:
//             Hash_Init(&hash_sh384, SHA_384);
//             break;
//         default:
//             return 0x6A9D;
//     }

//     gstHashCtx.uiAlgId = ulAlgoId;

//     return ZAYK_SUCCESS;
// }

// /**
//  * @description:
//  * @detail:
//  * @param {VOID} *pState
//  * @param {UCHAR} *pucBuf
//  * @param {ULONG} ulLen
//  * @return {*}
//  * @author: lkc
//  */
// ULONG ZAYK_HashUpdate(UCHAR *pucBuf, ULONG ulLen)
// {
//     ULONG ulAlgoId = gstHashCtx.uiAlgId;

//     switch (ulAlgoId) {
//         case SGD_SM3:
//             Hash_Updata(&hash_sm3, pucBuf, ulLen);
//             break;
//         case SGD_SHA1:
//             Hash_Updata(&hash_sh1, pucBuf, ulLen);
//             break;
//         case SGD_SHA256:
//             Hash_Updata(&hash_sh256, pucBuf, ulLen);
//             break;
//         case SGD_SHA512:
//             Hash_Updata(&hash_sh512, pucBuf, ulLen);
//             break;
//         case SGD_SHA384:
//             Hash_Updata(&hash_sh384, pucBuf, ulLen);
//             break;
//         default:
//             return 0x6A9D;
//     }

//     return ZAYK_SUCCESS;
// }

// /**
//  * @description:
//  * @detail:
//  * @param {VOID} *pState
//  * @param {UCHAR} *pucDst
//  * @return {*}
//  * @author: lkc
//  */
// ULONG ZAYK_HashFinal(UCHAR *pucDst, ULONG *pulOutLen)
// {
//     ULONG ulAlgoId = gstHashCtx.uiAlgId;
//     int block_size;
//     int out_size;

//     UNUSED(block_size);
//     UNUSED(out_size);

//     switch (ulAlgoId) {
//         case SGD_SM3:
//             block_size = 64;
//             out_size = 32;
//             Hash_Final(&hash_sm3, pucDst);
//             break;
//         case SGD_SHA1:
//             block_size = 64;
//             out_size = 20;
//             Hash_Final(&hash_sh1, pucDst);
//             break;
//         case SGD_SHA256:
//             block_size = 64;
//             out_size = 32;
//             Hash_Final(&hash_sh256, pucDst);
//             break;
//         case SGD_SHA512:
//             block_size = 128;
//             out_size = 64;
//             Hash_Final(&hash_sh512, pucDst);
//             break;
//         case SGD_SHA384:
//             block_size = 128;
//             out_size = 48;
//             Hash_Final(&hash_sh384, pucDst);
//             break;
//         default:
//             return 0x6A9D;
//     }

//     *pulOutLen = out_size;

//     return ZAYK_SUCCESS;
// }

// /**
//  * @description: hash 
//  * @detail:
//  * @param {ULONG} ulAlgoId
//  * @param {UCHAR} *pucBuf
//  * @param {ULONG} ulLen
//  * @param {UCHAR} *pucDst
//  * @param {ULONG} *pulOutLen
//  * @return {*}
//  * @author: lkc
//  */
// ULONG ZAYK_Hash(ULONG ulAlgoId, UCHAR *pucBuf, ULONG ulLen, UCHAR *pucDst, ULONG *pulOutLen)
// {
//     int block_size;
//     int out_size;
//     HASH_CTX hash;
	
//     UNUSED(block_size);
//     UNUSED(out_size);

//     switch (ulAlgoId) {
//         case SGD_SM3:
//             block_size = 64;
//             out_size = 32;
//             Hash_Init(&hash, SM3_256);
//             break;
//         case SGD_SHA1:
//             block_size = 64;
//             out_size = 20;
//             Hash_Init(&hash, SHA_1);
//             break;
//         case SGD_SHA256:
//             block_size = 64;
//             out_size = 32;
//             Hash_Init(&hash, SHA_256);
//             break;
//         case SGD_SHA512:
//             block_size = 128;
//             out_size = 64;
//             Hash_Init(&hash, SHA_512);
//             break;
//         case SGD_SHA384:
//             block_size = 128;
//             out_size = 48;
//             Hash_Init(&hash, SHA_384);
//             break;
//         default:
//             return 0x6A9D;
//     }

//     Hash_Updata(&hash, pucBuf, ulLen);
//     Hash_Final(&hash, pucDst);

//     *pulOutLen = out_size;

//     return ZAYK_SUCCESS;
// }

// /**
//  * @description: hmac ?????
//  * @detail:
//  * @param {ULONG} ulAlgoId
//  * @param {UCHAR} *pucKey
//  * @param {ULONG} ulKeyLen
//  * @param {UCHAR} *pucInData
//  * @param {ULONG} ulInDataLen
//  * @param {UCHAR} *pucHMac
//  * @param {ULONG} *pulHmacLen
//  * @return {*}
//  * @author: lkc
//  */
// ULONG ZAYK_Hmac(ULONG ulAlgoId, UCHAR *pucKey, ULONG ulKeyLen, UCHAR *pucInData, ULONG ulInDataLen, UCHAR *pucHMac,
//                 ULONG *pulHmacLen)
// {
//     ULONG i;
//     UCHAR kh[128] = {0};
//     UCHAR out[128] = {0};
//     UCHAR ipad[128] = {0};
//     UCHAR opad[128] = {0};
//     ULONG block_size;
//     ULONG out_size;
//     ULONG uiOutDataLen;
//     ULONG ulRet;

//     switch (ulAlgoId) {
//         case SGD_SHA1:
//             block_size = 64;
//             out_size = 20;
//             break;
//         case SGD_SHA256:
//             block_size = 64;
//             out_size = 32;
//             break;
//         case SGD_SM3:
//             block_size = 64;
//             out_size = 32;
//             break;
//         case SGD_SHA384:
//             block_size = 128;
//             out_size = 48;
//             break;
//         case SGD_SHA512:
//             block_size = 128;
//             out_size = 64;
//             break;
//         case SGD_SHA224:
//             block_size = 64;
//             out_size = 28;
//             break;
//         default:
//             return 0x6A9D;
//     }

//     if (ulKeyLen > block_size) {
//         ulRet = ZAYK_HashInit(ulAlgoId);
//         if (ZAYK_SUCCESS != ulRet) {
//             return ulRet;
//         }
//         ulRet = ZAYK_HashUpdate(pucKey, ulKeyLen);
//         if (ZAYK_SUCCESS != ulRet) {
//             return ulRet;
//         }
//         ulRet = ZAYK_HashFinal(kh, &uiOutDataLen);
//         if (ZAYK_SUCCESS != ulRet) {
//             return ulRet;
//         }

//         ulKeyLen = out_size;
//         pucKey = kh;
//     }

//     memset(ipad, 0x36, block_size);
//     memset(opad, 0x5C, block_size);

//     for (i = 0; i < ulKeyLen; i++) {
//         ipad[i] = (unsigned char)(ipad[i] ^ pucKey[i]);
//         opad[i] = (unsigned char)(opad[i] ^ pucKey[i]);
//     }

//     ulRet = ZAYK_HashInit(ulAlgoId);
//     if (ZAYK_SUCCESS != ulRet) {
//         return ulRet;
//     }
//     ulRet = ZAYK_HashUpdate(ipad, block_size);
//     if (ZAYK_SUCCESS != ulRet) {
//         return ulRet;
//     }
//     ulRet = ZAYK_HashUpdate(pucInData, ulInDataLen);
//     if (ZAYK_SUCCESS != ulRet) {
//         return ulRet;
//     }
//     ulRet = ZAYK_HashFinal(out, &uiOutDataLen);
//     if (ZAYK_SUCCESS != ulRet) {
//         return ulRet;
//     }

//     ulRet = ZAYK_HashInit(ulAlgoId);
//     if (ZAYK_SUCCESS != ulRet) {
//         return ulRet;
//     }
//     ulRet = ZAYK_HashUpdate(opad, block_size);
//     if (ZAYK_SUCCESS != ulRet) {
//         return ulRet;
//     }
//     ulRet = ZAYK_HashUpdate(out, out_size);
//     if (ZAYK_SUCCESS != ulRet) {
//         return ulRet;
//     }
//     ulRet = ZAYK_HashFinal(pucHMac, &uiOutDataLen);
//     if (ZAYK_SUCCESS != ulRet) {
//         return ulRet;
//     }
//     *pulHmacLen = out_size;

//     return ZAYK_SUCCESS;
// }

// /**
//  * @description:
//  * @detail:
//  * @return {*}
//  * @author: lkc
//  */
// ULONG ZAYK_HmacInit(ULONG ulAlgoId, UCHAR *ucKey, ULONG ulKeyLen)
// {
//     int block_size;
//     int out_size;
//     UCHAR *pucKeyTmp;
//     UCHAR aucKh[128];
//     UCHAR ipad[128];
//     UCHAR opad[128];
//     ULONG i = 0;
//     ULONG ulOutLen;
//     ULONG ulRet;

//     switch (ulAlgoId) {
//         case SGD_SHA1:
//             block_size = 64;
//             out_size = 20;
//             break;
//         case SGD_SHA256:
//             block_size = 64;
//             out_size = 32;
//             break;
//         case SGD_SM3:
//             block_size = 64;
//             out_size = 32;
//             break;
//         case SGD_SHA224:
//             block_size = 64;
//             out_size = 28;
//             break;
//         case SGD_SHA384:
//             block_size = 128;
//             out_size = 48;
//             break;
//         case SGD_SHA512:
//             block_size = 128;
//             out_size = 64;
//             break;
//         default:
//             return 0x6A9D;
//     }
//     pucKeyTmp = ucKey;

//     if (ulKeyLen > block_size) {
//         ulRet = ZAYK_HashInit(ulAlgoId);
//         if (ZAYK_SUCCESS != ulRet) {
//             return ulRet;
//         }
//         ulRet = ZAYK_HashUpdate(pucKeyTmp, ulKeyLen);
//         if (ZAYK_SUCCESS != ulRet) {
//             return ulRet;
//         }
//         ulRet = ZAYK_HashFinal(aucKh, &ulOutLen);
//         if (ZAYK_SUCCESS != ulRet) {
//             return ulRet;
//         }

//         ulKeyLen = out_size;
//         pucKeyTmp = aucKh;
//     } /*  ??????????�A???????????��???0 */
//     else if (ulKeyLen < block_size) {
//     }

//     /* XOR???? */
//     memset(ipad, 0x36, block_size);
//     memset(opad, 0x5C, block_size);
//     for (i = 0; i < ulKeyLen; i++) {
//         ipad[i] = (unsigned char)(ipad[i] ^ ucKey[i]);
//         opad[i] = (unsigned char)(opad[i] ^ ucKey[i]);
//     }

//     ulRet = ZAYK_HashInit(ulAlgoId);
//     if (ZAYK_SUCCESS != ulRet) {
//         return ulRet;
//     }
//     ulRet = ZAYK_HashUpdate(ipad, block_size);
//     if (ZAYK_SUCCESS != ulRet) {
//         return ulRet;
//     }

//     memset(&gstHmacCtx, 0x00, sizeof(gstHmacCtx));

//     gstHmacCtx.uiAlgId = ulAlgoId;
//     memcpy(gstHmacCtx.opad, opad, sizeof(opad));
//     gstHmacCtx.block_size = block_size;
//     gstHmacCtx.out_size = out_size;

//     return ZAYK_SUCCESS;
// }

// /**
//  * @description:
//  * @detail:
//  * @param {VOID} *pState
//  * @param {UCHAR} *pucBuf
//  * @param {ULONG} ulLen
//  * @return {*}
//  * @author: lkc
//  */
// ULONG ZAYK_HmacUpdate(UCHAR *pucBuf, ULONG ulLen)
// {
//     // ULONG ulAlgoId = gstHmacCtx.uiAlgId;
//     ULONG ulRet;

//     ulRet = ZAYK_HashUpdate(pucBuf, ulLen);
//     if (ZAYK_SUCCESS != ulRet) {
//         return ZAYK_FAIL;
//     }

//     return ZAYK_SUCCESS;
// }

// /**
//  * @description:
//  * @detail:
//  * @param {VOID} *pState
//  * @param {UCHAR} *pucDst
//  * @return {*}
//  * @author: lkc
//  */
// ULONG ZAYK_HmacFinal(UCHAR *pucDst, ULONG *pulOutLen)
// {
//     ULONG ulAlgoId = gstHmacCtx.uiAlgId;
//     unsigned char out[128];
//     ULONG ulOutLen;
//     ULONG ulRet;

//     ulRet = ZAYK_HashFinal(out, &ulOutLen);
//     if (ZAYK_SUCCESS != ulRet) {
//         return ulRet;
//     }

//     ulRet = ZAYK_HashInit(ulAlgoId);
//     if (ZAYK_SUCCESS != ulRet) {
//         return ulRet;
//     }
//     ulRet = ZAYK_HashUpdate(gstHmacCtx.opad, gstHmacCtx.block_size);
//     if (ZAYK_SUCCESS != ulRet) {
//         return ulRet;
//     }
//     ulRet = ZAYK_HashUpdate(out, ulOutLen);
//     if (ZAYK_SUCCESS != ulRet) {
//         return ulRet;
//     }
//     ulRet = ZAYK_HashFinal(pucDst, pulOutLen);
//     if (ZAYK_SUCCESS != ulRet) {
//         return ulRet;
//     }

//     return ZAYK_SUCCESS;
// }

// ULONG ZAYK_HashTest(VOID)
// {
//     UCHAR keyStr[] = "618DD6BF3AB02AEAEDEEE83DB58E3AB6";
//     UCHAR aucKey[32];
//     ULONG uiOutDataLen;
//     UCHAR indata[32];
//     UCHAR aucHmac1[128];
//     UCHAR aucHmac2[128];
//     ULONG ulHmaclen;
//     USHORT usRet = 0;

//     memset(indata, 0x11, sizeof(indata));
//     memset(aucHmac1, 0x00, sizeof(aucHmac1));

//     hex_string_to_array(keyStr, aucKey, &uiOutDataLen);

//     usRet = ZAYK_Hmac(SGD_SM3, aucKey, uiOutDataLen, indata, 16, aucHmac1, &ulHmaclen);
//     if (ZAYK_SUCCESS != usRet) {
//         return ZAYK_FAIL;
//     }

//     usRet = ZAYK_HmacInit(SGD_SM3, aucKey, uiOutDataLen);
//     if (ZAYK_SUCCESS != usRet) {
//         return ZAYK_FAIL;
//     }

//     usRet = ZAYK_HmacUpdate(indata, 16);
//     if (ZAYK_SUCCESS != usRet) {
//         return ZAYK_FAIL;
//     }

//     memset(aucHmac2, 0x00, sizeof(aucHmac2));
//     usRet = ZAYK_HmacFinal(aucHmac2, &ulHmaclen);
//     if (ZAYK_SUCCESS != usRet) {
//         return ZAYK_FAIL;
//     }

//     return ZAYK_SUCCESS;
// }
#ifdef __cplusplus
}
#endif
