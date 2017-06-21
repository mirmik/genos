#ifndef GENOS_SERDATA
#define GENOS_SERDATA

#include "genos/serialization.h"
#include "genos/serialization/hserial.h"

template<typename ... Types> struct serdata;

template<typename T0>
struct serdata<T0>
{
	T0 arg0;

	unsigned int hserial(char* buf, uint8_t flag)
	{
		unsigned int bias = 0;
		bias += ::hserial(buf+bias, &arg0, flag);
		return bias;
	};
};

template<typename T0, typename T1>
struct serdata<T0,T1>
{
	T0 arg0;
	T1 arg1;

	unsigned int hserial(char* buf, uint8_t flag)
	{
		unsigned int bias = 0;
		bias += ::hserial(buf+bias, &arg0, flag);
		bias += ::hserial(buf+bias, &arg1, flag);
		return bias;
	};
};

template<typename T0, typename T1, typename T2>
struct serdata<T0,T1,T2>
{
	T0 arg0;
	T1 arg1;
	T2 arg2;

	unsigned int hserial(char* buf, uint8_t flag)
	{
		unsigned int bias = 0;
		bias += ::hserial(buf+bias, &arg0, flag);
		bias += ::hserial(buf+bias, &arg1, flag);
		bias += ::hserial(buf+bias, &arg2, flag);
		return bias;
	};
};

#endif