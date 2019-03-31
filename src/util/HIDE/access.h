#ifndef GENOS_UTIL_ACCESS_H
#define GENOS_UTIL_ACCESS_H

#include <hal/arch.h>

#if BYTE_ORDER == __BIG_ENDIAN

#	define INT16_HI(arg) *((uint8_t*)&arg + 0)
#	define INT16_LO(arg) *((uint8_t*)&arg + 1)

#	define INT32_HHI(arg) *((uint8_t*)&arg + 0)
#	define INT32_HLO(arg) *((uint8_t*)&arg + 1)
#	define INT32_LHI(arg) *((uint8_t*)&arg + 2)
#	define INT32_LLO(arg) *((uint8_t*)&arg + 3)

#	define INT64_HHHI(arg) *((uint8_t*)&arg + 0)
#	define INT64_HHLO(arg) *((uint8_t*)&arg + 1)
#	define INT64_HLHI(arg) *((uint8_t*)&arg + 2)
#	define INT64_HLLO(arg) *((uint8_t*)&arg + 3)
#	define INT64_LHHI(arg) *((uint8_t*)&arg + 4)
#	define INT64_LHLO(arg) *((uint8_t*)&arg + 5)
#	define INT64_LLHI(arg) *((uint8_t*)&arg + 6)
#	define INT64_LLLO(arg) *((uint8_t*)&arg + 7)

#elif BYTE_ORDER == __LITTLE_ENDIAN

#	define INT16_HI(arg) *((uint8_t*)&arg + 1)
#	define INT16_LO(arg) *((uint8_t*)&arg + 0)

#	define INT32_HHI(arg) *((uint8_t*)&arg + 3)
#	define INT32_HLO(arg) *((uint8_t*)&arg + 2)
#	define INT32_LHI(arg) *((uint8_t*)&arg + 1)
#	define INT32_LLO(arg) *((uint8_t*)&arg + 0)

#	define INT64_HHHI(arg) *((uint8_t*)&arg + 7)
#	define INT64_HHLO(arg) *((uint8_t*)&arg + 6)
#	define INT64_HLHI(arg) *((uint8_t*)&arg + 5)
#	define INT64_HLLO(arg) *((uint8_t*)&arg + 4)
#	define INT64_LHHI(arg) *((uint8_t*)&arg + 3)
#	define INT64_LHLO(arg) *((uint8_t*)&arg + 2)
#	define INT64_LLHI(arg) *((uint8_t*)&arg + 1)
#	define INT64_LLLO(arg) *((uint8_t*)&arg + 0)

#endif 

#define UINT16_HI(arg) INT16_HI(arg)
#define UINT16_LO(arg) INT16_LO(arg)

#define UINT32_HHI(arg) INT32_HHI(arg)
#define UINT32_HLO(arg) INT32_HLO(arg)
#define UINT32_LHI(arg) INT32_LHI(arg)
#define UINT32_LLO(arg) INT32_LLO(arg)

#define UINT64_HHHI(arg) INT64_HHHI(arg)
#define UINT64_HHLO(arg) INT64_HHLO(arg)
#define UINT64_HLHI(arg) INT64_HLHI(arg)
#define UINT64_HLLO(arg) INT64_HLLO(arg)
#define UINT64_LHHI(arg) INT64_LHHI(arg)
#define UINT64_LHLO(arg) INT64_LHLO(arg)
#define UINT64_LLHI(arg) INT64_LLHI(arg)
#define UINT64_LLLO(arg) INT64_LLLO(arg)

#endif //GENOS_UTIL_ACCESS_H