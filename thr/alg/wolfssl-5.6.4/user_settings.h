#ifndef USER_SETTINGS_H
#define USER_SETTINGS_H

#ifdef __cplusplus
extern "C" {
#endif

// #define DEBUG_WOLFSSL

// NOTE 增加此定义以后DSA3072运算失败
// #define USE_FAST_MATH

#define WOLFSSL_SMALL_STACK

#define WC_NO_HARDEN

// #define NO_INLINE
// #define WOLFSSL_MISC_INCLUDED
#define WOLFSSL_IGNORE_FILE_WARN

#define WOLFSSL_PTHREADS

#define WOLFCRYPT_ONLY

#define HAVE_AES_ECB
#define WOLFSSL_AES_DIRECT

#define HAVE_ED25519
#define WOLFSSL_SHA512

#define HAVE_ED448
#define WOLFSSL_SHAKE256
// #define WOLFSSL_ED448_PERSISTENT_SHA
#define WOLFSSL_SHA3
#define WOLFSSL_SHAKE256
#define WOLFSSL_SHAKE128

#define WOLFSSL_KEY_GEN

// NOTE
// #define HAVE_INTEL_RDRAND
// #define HAVE_CPUID
// #define HAVE_CPUID_INTEL

#ifdef __cplusplus
}
#endif

#endif
