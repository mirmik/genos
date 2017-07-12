#ifndef GQ_SERIAL_BUFFER_H
#define GQ_SERIAL_BUFFER_H

#include <genos/serialization.h>
#include <gQ/gQByteAllocated.h>

class gQSKB : public gQBasicAllocated {

private:
	class Record {
	public:
		char* data;
		uint16_t refs;
		size_t rsize;
		size_t lsize;
		size_t capacity;
		size_t nullpos;
		
		Record() : data(nullptr), refs(0), rsize(0), lsize(0) 
			, capacity(0), nullpos(0) {};
	};

	Record* m_record;

public:
	gQSKB() : gQBasicAllocated() {
		m_record = m_alloc->constructObject<Record>();
		m_record->refs++;
	}; 

	gQSKB(const gQSKB& other) {
		m_record = other.m_record;
		m_record->refs++;
	}

	gQSKB(gQSKB&& other) {
		m_record = other.m_record;
	}
	
	~gQSKB() {
		m_record->refs--;
		if (m_record->refs <= 0) {
			if (m_record->data) m_alloc->deallocate(m_record->data);
			m_alloc->destructObject(m_record);
		}
	}	

	unsigned char reserve(size_t presize, size_t postsize) {
		if (m_record->refs > 1) do_copy();

		if (m_record->capacity < presize + postsize) 
			changeBuffer(presize, postsize);
	
		if (presize > m_record->nullpos)
			moveData(presize - m_record->lsize);

		else return 1;
	}

	unsigned char do_copy() {
		auto lptr = leftptr();
		Record* newrecord = m_alloc->constructObject<Record>();
		memcpy(newrecord, m_record, sizeof(Record));
		newrecord->data = (char*)m_alloc->allocate(capacity());
		memcpy(leftptr(), lptr, size());
		--m_record->refs;
		newrecord->refs = 1;
		m_record = newrecord;
	}

	unsigned char moveData(ssize_t off) {
		dpr("moveData"); dprln(off);
		memmove(leftptr() + off, leftptr(), size());
		m_record->nullpos += off;
	};

	unsigned char changeBuffer(size_t presize, size_t postsize) {
		char* newbuf = (char*) m_alloc->allocate(presize + postsize);
		auto oldbuf = m_record->data;

		memcpy(newbuf + presize - m_record->lsize, 
			oldbuf + m_record->nullpos - m_record->lsize,
			m_record->lsize + m_record->rsize);

		m_record->capacity = presize + postsize;
		m_record->nullpos = presize;
		m_record->data = newbuf;

		if (oldbuf) m_alloc->deallocate(oldbuf);
		return 1;
	};

	void concat_back(const char* str, size_t sz) {
		reserve(leftsize(), rightsize() + sz);
		memcpy(rightptr(),str,sz);
		m_record->rsize += sz;
	}

	void concat_front(const char* str, size_t sz) {
		reserve(leftsize() + sz, rightsize());
		m_record->lsize += sz;
		memcpy(leftptr(),str,sz);
	}

	size_t leftsize() const {
		return m_record->lsize;
	}

	size_t rightsize() const {
		return m_record->rsize;
	}

	char* leftptr() const {
		return m_record->data + m_record->nullpos - m_record->lsize;
	}

	char* rightptr() const {
		return m_record->data + m_record->nullpos + m_record->rsize;
	}

	size_t size() const {
		return m_record->rsize + m_record->lsize;
	}

	size_t capacity() const {
		return m_record->capacity;
	}

	char* data() {
		return leftptr();
	}	

	//RSerial rserial() {
	//	return RSerial(this);
	//}

	const char* c_str() {
		reserve(leftsize(), rightsize() + 1);
		*rightptr() = 0;
		return leftptr();
	}

};

#endif