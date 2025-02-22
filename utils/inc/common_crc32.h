#ifndef __COMMON_CRC32_H__
#define __COMMON_CRC32_H__

#include <sysdep.h>

/**
 * Note:
 * Select endian mode in case of machine endian type
 *
 * little endian: ARM(default), X86,...
 * big    endian: PowerPC, KEIL C51, network
 *
 */
#define LITTLE_ENDIAN

/**
 * There are multiple 16-bit CRC polynomials in common use, but this is
 * *the* standard CRC-32 polynomial, first popularized by Ethernet.
 * x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x^1+x^0
 */
#define CRCPOLY_LE 0xedb88320
#define CRCPOLY_BE 0x04c11db7

/**
 * How many bits at a time to use.  Valid values are 1, 2, 4, 8, 32 and 64.
 * For less performance-sensitive, use 4 or 8 to save table size.
 * For larger systems choose same as CPU architecture as default.
 */
#ifndef CRC_LE_BITS
#define CRC_LE_BITS 32
#endif
#ifndef CRC_BE_BITS
#define CRC_BE_BITS 32
#endif

#if CRC_LE_BITS > 8
#define LE_TABLE_ROWS (CRC_LE_BITS / 8)
#define LE_TABLE_SIZE 256
#endif

#if CRC_BE_BITS > 8
#define BE_TABLE_ROWS (CRC_BE_BITS / 8)
#define BE_TABLE_SIZE 256
#endif

/**
 * For CRC32, need table size using stack about 256*4*4 = 4KB
 *
 * For test and checking, we can generate CRC32 using WinRAR software mode
 * We suggest that:
 *     seed = ~0;
 *     crc32 = crc32(seed, data, length) ^ (~0);
 *     return crc32;
 */
#ifdef LITTLE_ENDIAN

uint32 crc32_le(uint32 crc, const uint8 *p, uint32 len);
#define crc32(seed, pbuf, length) crc32_le(seed, (const uint8 *)(pbuf), length)

#else

uint32 crc32_be(uint32 crc, const uint8 *p, uint32 len);
#define crc32(seed, pbuf, length) crc32_be(seed, (const uint8 *)(pbuf), length)

#endif

#endif /* __CRC32_H__ */
