#ifndef GENOS_UTIL_BYTE_ACCESSOR
#define GENOS_UTIL_BYTE_ACCESSOR

#include "hal/arch.h"

union uint8_accessor{
	uint8_t num;
	struct
	{
		uint8_t l:4;
		uint8_t h:4;
	};
	struct
	{
		uint8_t bit0:1;
		uint8_t bit1:1;
		uint8_t bit2:1;
		uint8_t bit3:1;
		uint8_t bit4:1;
		uint8_t bit5:1;
		uint8_t bit6:1;
		uint8_t bit7:1;
	};
};

#if BYTE_ORDER == __LITTLE_ENDIAN
union uint16_accessor
{
	uint16_t num;
	struct 
	{
		uint8_t lo;
		uint8_t hi;		
	};
	struct 
	{
		uint8_t lo_l:4;
		uint8_t lo_h:4;
		uint8_t hi_l:4;
		uint8_t hi_h:4;		
	};

};

union uint32_accessor
{
	uint32_t num;
	struct 
	{
		uint8_t llo;
		uint8_t lhi;
		uint8_t hlo;
		uint8_t hhi;
	};
	struct 
	{
		uint8_t llo_l:4;
		uint8_t llo_h:4;
		uint8_t lhi_l:4;
		uint8_t lhi_h:4;
		uint8_t hlo_l:4;
		uint8_t hlo_h:4;
		uint8_t hhi_l:4;
		uint8_t hhi_h:4;
	};
};

union uint64_accessor
{
	uint64_t num;
	struct 
	{
		uint8_t lllo;
		uint8_t llhi;
		uint8_t lhlo;
		uint8_t lhhi;
		uint8_t hllo;
		uint8_t hlhi;
		uint8_t hhlo;
		uint8_t hhhi;
	};
	struct 
	{
		uint8_t lllo_l:4;
		uint8_t lllo_h:4;
		uint8_t llhi_l:4;
		uint8_t llhi_h:4;
		uint8_t lhlo_l:4;
		uint8_t lhlo_h:4;
		uint8_t lhhi_l:4;
		uint8_t lhhi_h:4;
		uint8_t hllo_l:4;
		uint8_t hllo_h:4;
		uint8_t hlhi_l:4;
		uint8_t hlhi_h:4;
		uint8_t hhlo_l:4;
		uint8_t hhlo_h:4;
		uint8_t hhhi_l:4;
		uint8_t hhhi_h:4;
	};
};


#elif BYTE_ORDER == __BIG_ENDIAN
#error "ARCH_BIG_ENDIAN TODO"
union uint32_accessor
{
	uint32_t num;
	struct 
	{
		uint8_t hhi;
		uint8_t hlo;
		uint8_t lhi;
		uint8_t llo;
	};
};

union uint16_accessor
{
	uint16_t num;
	struct 
	{
		uint8_t hi;
		uint8_t lo;		
	};
};
#else
#error "NEED ENDIAN"
#endif

#endif