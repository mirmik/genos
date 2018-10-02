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

#elif BYTE_ORDER == __LITTLE_ENDIAN

#	define INT16_HI(arg) *((uint8_t*)&arg + 1)
#	define INT16_LO(arg) *((uint8_t*)&arg + 0)
#	define INT32_HHI(arg) *((uint8_t*)&arg + 3)
#	define INT32_HLO(arg) *((uint8_t*)&arg + 2)
#	define INT32_LHI(arg) *((uint8_t*)&arg + 1)
#	define INT32_LLO(arg) *((uint8_t*)&arg + 0)

#endif 

#define UINT16_HI(arg) INT16_HI(arg)
#define UINT16_LO(arg) INT16_LO(arg)
#define UINT32_HHI(arg) INT32_HHI(arg)
#define UINT32_HLO(arg) INT32_HLO(arg)
#define UINT32_LHI(arg) INT32_LHI(arg)
#define UINT32_LLO(arg) INT32_LLO(arg)

#endif //GENOS_UTIL_ACCESS_H