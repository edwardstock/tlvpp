/*!
 * tlvpp.
 * endian.h
 *
 * \date 01/28/2021
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 *
 * @todo: use force big endian
 */
#ifndef TLVPP_BYTE_ORDER_H
#define TLVPP_BYTE_ORDER_H

#include <stdint.h>
#include <string.h>

#ifdef __APPLE__
#include <libkern/OSByteOrder.h>
#include <machine/endian.h>

#define htobe16(x) OSSwapHostToBigInt16(x)
#define htole16(x) OSSwapHostToLittleInt16(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define le16toh(x) OSSwapLittleToHostInt16(x)

#define htobe32(x) OSSwapHostToBigInt32(x)
#define htole32(x) OSSwapHostToLittleInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#define le32toh(x) OSSwapLittleToHostInt32(x)

#define htobe64(x) OSSwapHostToBigInt64(x)
#define htole64(x) OSSwapHostToLittleInt64(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#define le64toh(x) OSSwapLittleToHostInt64(x)
#elif defined(WIN32)
#include "win_endian.h"

#include <stdio.h>
#include <winsock2.h>

#if BYTE_ORDER == LITTLE_ENDIAN
#define le16toh(x) (x)
#define le32toh(x) (x)
#define le64toh(x) (x)
#define htole16(x) (x)
#define htole32(x) (x)
#define htole64(x) (x)
#define htobe16(x) __swap16gen(x)
#define htobe32(x) __swap32gen(x)
#define htobe64(x) __swap64gen(x)
#define be16toh(x) __swap16gen(x)
#define be32toh(x) __swap32gen(x)
#define be64toh(x) __swap32gen(x)
#endif

#elif defined(ARDUINO)

/* Note that these macros evaluate their arguments several times.  */
#define __swap16gen(x) (__uint16_t)(((__uint16_t)(x) &0xff) << 8 | ((__uint16_t)(x) &0xff00) >> 8)

#define __swap32gen(x)                                                                                                 \
    (uint32_t)(((uint32_t)(x) &0xff) << 24 | ((uint32_t)(x) &0xff00) << 8 | ((uint32_t)(x) &0xff0000) >> 8 |           \
               ((uint32_t)(x) &0xff000000) >> 24)

#define __swap64gen(x)                                                                                                 \
    (uint64_t)((((uint64_t)(x) &0xff) << 56) | ((uint64_t)(x) &0xff00ULL) << 40 | ((uint64_t)(x) &0xff0000ULL) << 24 | \
               ((uint64_t)(x) &0xff000000ULL) << 8 | ((uint64_t)(x) &0xff00000000ULL) >> 8 |                           \
               ((uint64_t)(x) &0xff0000000000ULL) >> 24 | ((uint64_t)(x) &0xff000000000000ULL) >> 40 |                 \
               ((uint64_t)(x) &0xff00000000000000ULL) >> 56)

#define __swap16 __swap16gen
#define __swap32 __swap32gen
#define __swap64 __swap64gen

#define swap16 __swap16
#define swap32 __swap32
#define swap64 __swap64

#define le16toh(x) (x)
#define le32toh(x) (x)
#define le64toh(x) (x)
#define htole16(x) (x)
#define htole32(x) (x)
#define htole64(x) (x)
#define htobe16(x) __swap16gen(x)
#define htobe32(x) __swap32gen(x)
#define htobe64(x) __swap64gen(x)
#define be16toh(x) __swap16gen(x)
#define be32toh(x) __swap32gen(x)
#define be64toh(x) __swap32gen(x)

#else
#include <endian.h>
#endif

#endif // TLVPP_BYTE_ORDER_H
