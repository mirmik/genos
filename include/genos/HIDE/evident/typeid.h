#ifndef GENOS_EVIDENT_TYPES
#define GENOS_EVIDENT_TYPES

#define TYPE_NIL 0
#define TYPE_INT8 1
#define TYPE_INT16 2
#define TYPE_INT32 3
#define TYPE_INT64 4
#define TYPE_UINT8 5
#define TYPE_UINT16 6
#define TYPE_UINT32 7
#define TYPE_UINT64 8
#define TYPE_SFLOAT 11	/*плавающее 4 байта*/
#define TYPE_DFLOAT 12	/*плавающее 8 байт*/

#define TYPE_STRING 13	/*плавающее 8 байт*/


template <typename type> struct evitype;

template<> struct evitype<void> {
	static constexpr eviid_t id = TYPE_NIL;
};

template<> struct evitype<int8_t> {
	static constexpr eviid_t id = TYPE_INT8;
};

template<> struct evitype<int16_t> {
	static constexpr eviid_t id = TYPE_INT16;
};

template<> struct evitype<int32_t> {
	static constexpr eviid_t id = TYPE_INT32;
};

template<> struct evitype<int64_t> {
	static constexpr eviid_t id = TYPE_INT64;
};

template<> struct evitype<uint8_t> {
	static constexpr eviid_t id = TYPE_UINT8;
};

template<> struct evitype<uint16_t> {
	static constexpr eviid_t id = TYPE_UINT16;
};

template<> struct evitype<uint32_t> {
	static constexpr eviid_t id = TYPE_UINT32;
};

template<> struct evitype<uint64_t> {
	static constexpr eviid_t id = TYPE_UINT64;
};
#endif