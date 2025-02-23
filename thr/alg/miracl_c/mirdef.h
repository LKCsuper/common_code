/*
 *   MIRACL compiler/hardware definitions - mirdef.h
 *   This version suitable for use with most 64-bit computers
 *   e.g. 80386+ PC, VAX, ARM etc. Assembly language versions of muldiv,
 *   muldvm, muldvd and muldvd2 will be necessary. See mrmuldv.any
 *
 *   Suitable for Unix/Linux and for DJGPP GNU C Compiler
 *   Copyright (c) 1988-2006 Shamus Software Ltd.
 */

#define MIRACL 32
#define MR_LITTLE_ENDIAN    /* This may need to be changed        */
/*#define MR_BIG_ENDIAN*/    /* This may need to be changed        */
#define mr_utype int
/* the underlying type is usually int *
 * but see mrmuldv.any                */
#define mr_unsign32 unsigned int
/* 64 bit unsigned type               */
#define MR_IBITS      32    /* bits in int  */
#define MR_LBITS      64    /* bits in long */
#define MR_FLASH      52
/* delete this definition if integer  *
 * only version of MIRACL required    */
/* Number of bits per double mantissa */

#define mr_dltype long long   /* ... or __int64 for Windows       */
#define mr_unsign64 unsigned long long

#define MAXBASE ((mr_small)1<<(MIRACL-1))
#define MRBITSINCHAR 8
#define MR_NOASM
#define MR_NO_STANDARD_IO /* no printf support */
#define MR_UNIX_MT  /* multi-threaded */