#ifndef GQ_SERIAL_BUFFER_H
#define GQ_SERIAL_BUFFER_H

//#include "genos_configure.h"

#include <gxx/utility.h>
#include <gxx/Allocator.h>

#include <genos/serialization.h>

#include <genos/panic.h>
#include <assert.h>

namespace gxx { 

	class SerialBuffer : public BasicAllocated {
	
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
			friend class SerialBuffer;
		};
	
		Record* m_record;
	
	private:
		void invalidate();
		void release();
		unsigned char changeExtBuffer(size_t prefix, size_t postfix);
		unsigned char do_copy();
	
	public:
		SerialBuffer();
		SerialBuffer(const SerialBuffer& other);
		//SerialBuffer(SerialBuffer&& other);
		~SerialBuffer();
		
		unsigned char reserve_back(size_t sz);
		unsigned char reserve_front(size_t sz);
		
		void put_back(const char* cnct, size_t sz);
		void put_front(const char* cnct, size_t sz);
		void push_back(const char* cnct, size_t sz);
		void push_front(const char* cnct, size_t sz);
	
		void pop_front(size_t sz);
	
		char* data() const;
		size_t size() const;
		size_t capacity() const;
		size_t room_front();
		size_t room_back();
		
		char* begin();
		char* end();
		
		void changed();
		void shrink();
		void clear();
		
		SerialBuffer& operator= (const SerialBuffer& other);
		//SerialBuffer& operator= (SerialBuffer&& other);
	
		DeserialBuffer toDeserialBuffer() const;
		const char* c_str();
	
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
	

		size_t refs() { return m_record->refs; };
	};

};

#endif