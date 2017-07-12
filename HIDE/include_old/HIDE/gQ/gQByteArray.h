#ifndef GQ_SERIAL_BUFFER_H
#define GQ_SERIAL_BUFFER_H

#include "genos_configure.h"
#include <gxx/utility.h>
#include <genos/serialization.h>
#include <assert.h>
#include <gQ/gQByteAllocated.h>
#include <genos/panic.h>

class gQSerialBuffer : public gQBasicAllocated {

public:
	class Record {
	public:
		char* buff;
		char* data;
		size_t size;
		size_t capacity;
		uint16_t refs;

		Record() : data(nullptr), buff(nullptr)
			, size(0), refs(0), capacity(0) {};
		friend class gQSerialBuffer;
	};

	Record* m_record;

public:
	gQSerialBuffer() : gQBasicAllocated()  {
		m_record = m_alloc->constructObject<Record>();
		m_record->refs = 1;
	}; 

	gQSerialBuffer(const gQSerialBuffer& other) {
		m_record = other.m_record;
		m_record->refs++;
	}

	gQSerialBuffer(gQSerialBuffer&& other) {
		m_record = other.m_record;
	}
	
	~gQSerialBuffer() {
		release();
	}	

	void release() {
		m_record->refs--;
		if (m_record->refs <= 0) {
			if (m_record->buff) m_alloc->deallocate(m_record->buff);
			m_alloc->destructObject(m_record);
		}
	}

	void invalidate() {
		if (m_record->buff) m_alloc->deallocate(m_record->buff);
		m_record->buff = nullptr;
		m_record->data = nullptr;
		m_record->capacity = 0;
		m_record->size = 0;
	}

	void changed() {
		if (m_record->refs != 1) do_copy();
	}

	size_t room_front() {
		return m_record->data - m_record->buff;
	}

	size_t room_back() {
		return m_record->capacity - m_record->size - room_front();
	}
	
	unsigned char reserve_back(size_t sz) {
		changed();
		if (room_back() > sz) return 1;
		return changeExtBuffer(0, sz - room_back());
	}

	unsigned char reserve_front(size_t sz) {
		changed();
		if (room_front() > sz) return 1;
		return changeExtBuffer(sz - room_front(), 0);
	}

	unsigned char changeExtBuffer(size_t prefix, size_t postfix) {
		size_t newcap = m_record->capacity + prefix + postfix;
		char* newbuf = (char*) m_alloc->allocate(newcap);
		char* oldbuf = m_record->buff;
		char* dst = newbuf + prefix + room_front();
		char* src = oldbuf + room_front();
		memcpy(dst, src, m_record->size);
		m_record->buff = newbuf;
		m_record->capacity = newcap;
		m_record->data = dst;	
		if (oldbuf) m_alloc->deallocate(oldbuf);
	};

	unsigned char do_copy() {
		Record* newrecord = m_alloc->constructObject<Record>();
		memcpy(newrecord, m_record, sizeof(Record));
		newrecord->buff = (char*)m_alloc->allocate(capacity());
		newrecord->data = newrecord->buff + room_front();
		memcpy(newrecord->data, m_record->data, size());
		--m_record->refs;
		newrecord->refs = 1;
		m_record = newrecord;
	}

	size_t size() const {
		return m_record->size;
	}

	size_t capacity() const {
		return m_record->capacity;
	}

	void put_back(const char* cnct, size_t sz) {
		assert(sz <= room_back());
		changed();
		memcpy(m_record->data + m_record->size, cnct, sz);
		m_record->size += sz;
	}

	void put_front(const char* cnct, size_t sz) {
		assert(sz <= room_front());
		changed();
		memcpy(m_record->data - m_record->size, cnct, sz);
		m_record->data -= sz;
		m_record->size += sz;
	}

	void push_back(const char* cnct, size_t sz) {
		reserve_back(sz);
		memcpy(m_record->data + m_record->size, cnct, sz);
		m_record->size += sz;
	}

	void push_front(const char* cnct, size_t sz) {
		reserve_front(sz);
		memcpy(m_record->data - m_record->size, cnct, sz);
		m_record->data -= sz;
		m_record->size += sz;
	}


	/*template <typename T>
	void putserial_back(const T& ref) {
		putserial_back(ref, GENOS_SERIAL_ORDER);
	}*/

	template <typename T>
	void putserial_back(T&& ref, uint8_t flag = GENOS_SERIAL_ORDER) {
		size_t presize = serialSize(ref);
		if (presize > room_back()) panic("TODO");
		changed();
		size_t sz = ::serialization(end(), ref, room_back(), flag);
		m_record->size += sz;
	}

	template <typename T>
	void putserial_front(T&& ref, uint8_t flag = GENOS_SERIAL_ORDER) {
		size_t presize = serialSize(ref);
		if (presize > room_front()) panic("TODO");
		changed();
		size_t sz = ::serialization(m_record->data - presize, ref, room_front(), flag);
		m_record->data -= presize;
		m_record->size += sz;
	}

	template <typename T>
	void pushserial_back(T&& ref, uint8_t flag = GENOS_SERIAL_ORDER) {
		size_t presize = serialSize(ref);
		reserve_back(presize);
		size_t sz = ::serialization(end(), ref, room_back(), flag);
		m_record->size += sz;
	}

	template <typename T>
	void pushserial_front(T&& ref, uint8_t flag = GENOS_SERIAL_ORDER) {
		size_t presize = serialSize(ref);
		reserve_front(presize);
		size_t sz = ::serialization(m_record->data - presize, ref, room_front(), flag);
		m_record->data -= presize;
		m_record->size += sz;
	}

	void pop_front(size_t sz) {
		changed();
		assert(sz <= size());
		m_record->data += sz;
		m_record->size -= sz;
	}

	/*template <typename T>
	void popdeserial_front(T& ref, uint8_t) {
		size_t presize = deserialSize(data());
		if (presize < size()) panic("TODO");
		size_t sz = ::deserialization(m_record->data, ref, presize, flag);
		pop_front(sz);
	}*/

	template <typename T>
	void popdeserial_front(T& ref, uint8_t flag = GENOS_SERIAL_ORDER) {
		dprhexln(m_record->size);
		dprhexln(m_record->capacity);
		dprptrln(m_record->buff);
		dprptrln(m_record->data);
		dprln("here");
		size_t presize = deserialSize<T>(data());
		dprln("here");
		if (presize > size()) panic("TODO");
		size_t sz = ::deserialization(m_record->data, ref, presize, flag);
		dprln("here");
		pop_front(sz);
	}

	gQSerialBuffer& operator= (const gQSerialBuffer& other) {
		release();
		m_record = other.m_record;
		m_record->refs++;		
	}

	gQSerialBuffer& operator= (gQSerialBuffer&& other) {
		release();
		m_record = other.m_record;
	}

	void shrink() {
		changed();
		if (size() == 0) invalidate();
		changeExtBuffer(0,0);
	}

	void clear() {
		invalidate();
	}

	char* begin() {
		return m_record->data;
	}

	char* end() {
		return m_record->data + m_record->size;
	}

	char* data() const {
		return m_record->data;
	}

	DeserialBuffer toDeserialBuffer() const {
		return DeserialBuffer(data(), size());
	}

	const char* c_str() {
		reserve_back(1);
		*(m_record->data + m_record->size) = 0;
		return m_record->data;
	}
};

#endif