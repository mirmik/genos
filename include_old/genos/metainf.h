#ifndef GENOS_METAINF_H
#define GENOS_METAINF_H

#define TYPE_NIL 0
#define TYPE_INT8 1
#define TYPE_INT16 2
#define TYPE_INT32 3
#define TYPE_INT64 4
#define TYPE_UINT8 5
#define TYPE_UINT16 6
#define TYPE_UINT32 7
#define TYPE_UINT64 8
#define TYPE_CSTR 9		/*строка с казанием числа байт*/
#define TYPE_NSTR 10	/*нуль терминированная строка*/
#define TYPE_SFLOAT 11	/*плавающее 4 байта*/
#define TYPE_DFLOAT 12	/*плавающее 8 байт*/

using MetaInfId = uint8_t;

template<typename T> struct MetaInf;

template<> struct MetaInf<void> {
	static constexpr MetaInfId id = TYPE_NIL;
};

template<> struct MetaInf<uint8_t> {
	static constexpr MetaInfId id = TYPE_UINT8;
};

template<> struct MetaInf<uint16_t> {
	static constexpr MetaInfId id = TYPE_UINT16;
};

template<> struct MetaInf<uint32_t> {
	static constexpr MetaInfId id = TYPE_UINT32;
};

template<> struct MetaInf<uint64_t> {
	static constexpr MetaInfId id = TYPE_UINT64;
};

template<> struct MetaInf<int8_t> {
	static constexpr MetaInfId id = TYPE_INT8;
};

template<> struct MetaInf<int16_t> {
	static constexpr MetaInfId id = TYPE_INT16;
};

template<> struct MetaInf<int32_t> {
	static constexpr MetaInfId id = TYPE_INT32;
};

template<> struct MetaInf<int64_t> {
	static constexpr MetaInfId id = TYPE_INT64;
};

#endif