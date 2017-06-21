#include <gxx/SerialBuffer.h>

namespace gxx {

	SerialBuffer::SerialBuffer() : BasicAllocated()  {
		m_record = m_alloc->constructObject<Record>();
		m_record->refs = 1;
	}; 

	SerialBuffer::SerialBuffer(const SerialBuffer& other) {
		m_record = other.m_record;
		m_record->refs++;
	}

	//SerialBuffer::SerialBuffer(SerialBuffer&& other) {
	//	m_record = other.m_record;
	//	other.m_record = nullptr;
	//}
	
	SerialBuffer::~SerialBuffer() {
		release();
	}	

	void SerialBuffer::release() {
	//	if (m_record == nullptr) return;
		m_record->refs--;
		if (m_record->refs <= 0) {
			if (m_record->buff) m_alloc->deallocate(m_record->buff);
			m_alloc->destructObject(m_record);
		}
	}

	void SerialBuffer::invalidate() {
		if (m_record->buff) m_alloc->deallocate(m_record->buff);
		m_record->buff = nullptr;
		m_record->data = nullptr;
		m_record->capacity = 0;
		m_record->size = 0;
	}

	void SerialBuffer::changed() {
		if (m_record->refs != 1) do_copy();
	}

	size_t SerialBuffer::room_front() {
		return m_record->data - m_record->buff;
	}

	size_t SerialBuffer::room_back() {
		return m_record->capacity - m_record->size - room_front();
	}
	
	unsigned char SerialBuffer::reserve_back(size_t sz) {
		changed();
		if (room_back() > sz) return 1;
		return changeExtBuffer(0, sz - room_back());
	}

	unsigned char SerialBuffer::reserve_front(size_t sz) {
		changed();
		if (room_front() > sz) return 1;
		return changeExtBuffer(sz - room_front(), 0);
	}

	unsigned char SerialBuffer::changeExtBuffer(size_t prefix, size_t postfix) {
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

	unsigned char SerialBuffer::do_copy() {
		Record* newrecord = m_alloc->constructObject<Record>();
		memcpy(newrecord, m_record, sizeof(Record));
		newrecord->buff = (char*)m_alloc->allocate(capacity());
		newrecord->data = newrecord->buff + room_front();
		memcpy(newrecord->data, m_record->data, size());
		--m_record->refs;
		newrecord->refs = 1;
		m_record = newrecord;
	}

	size_t SerialBuffer::size() const {
		return m_record->size;
	}

	size_t SerialBuffer::capacity() const {
		return m_record->capacity;
	}

	void SerialBuffer::put_back(const char* cnct, size_t sz) {
		assert(sz <= room_back());
		changed();
		memcpy(m_record->data + m_record->size, cnct, sz);
		m_record->size += sz;
	}

	void SerialBuffer::put_front(const char* cnct, size_t sz) {
		assert(sz <= room_front());
		changed();
		memcpy(m_record->data - m_record->size, cnct, sz);
		m_record->data -= sz;
		m_record->size += sz;
	}

	void SerialBuffer::push_back(const char* cnct, size_t sz) {
		reserve_back(sz);
		memcpy(m_record->data + m_record->size, cnct, sz);
		m_record->size += sz;
	}

	void SerialBuffer::push_front(const char* cnct, size_t sz) {
		reserve_front(sz);
		memcpy(m_record->data - m_record->size, cnct, sz);
		m_record->data -= sz;
		m_record->size += sz;
	}

	void SerialBuffer::pop_front(size_t sz) {
		changed();
		assert(sz <= size());
		m_record->data += sz;
		m_record->size -= sz;
	}

	SerialBuffer& SerialBuffer::operator= (const SerialBuffer& other) {
		release();
		m_record = other.m_record;
		m_record->refs++;		
	}

	//SerialBuffer& SerialBuffer::operator= (SerialBuffer&& other) {
	//	release();
	//	m_record = other.m_record;
	//}

	void SerialBuffer::shrink() {
		changed();
		if (size() == 0) invalidate();
		changeExtBuffer(0,0);
	}

	void SerialBuffer::clear() {
		invalidate();
	}

	char* SerialBuffer::begin() {
		return m_record->data;
	}

	char* SerialBuffer::end() {
		return m_record->data + m_record->size;
	}

	char* SerialBuffer::data() const {
		return m_record->data;
	}

	DeserialBuffer SerialBuffer::toDeserialBuffer() const {
		return DeserialBuffer(data(), size());
	}

	const char* SerialBuffer::c_str() {
		reserve_back(1);
		*(m_record->data + m_record->size) = 0;
		return m_record->data;
	}
};
