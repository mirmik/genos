#ifndef EVIDENT_TYPES_H
#define EVIDENT_TYPES_H

#include "genos_configure.h"
#include "inttypes.h"
#include "genos/evident/typeid.h"
#include "mem/allocator.h"

class evident {
	void* data;
	uint16_t type;

public:

	evident() : data(nullptr), type(EVITYPE_NIL) {};

	template<typename T>
	evident& operator= (T val) {
		void* place = standart_allocator.allocate(sizeof(T));
		memcpy(place, (void*) &val, sizeof(T));
		data = place;
		type = evitype<T>::id;
	}

//	evident operator+

	void invalidate() {
		if (data) standart_allocator.deallocate(data);
		type = EVITYPE_NIL;
	};

	~evident() {
		if (data) standart_allocator.deallocate(data);
	};
};

#endif