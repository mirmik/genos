#ifndef STREAM_STRQUEUE
#define STREAM_STRQUEUE

#include <mem/sysalloc.h>
#include <gxx/container/dlist.h>
#include <gxx/buffer.h>
#include <gxx/string.h>
#include <inttypes.h>

class strqueue {

	struct record {
		dlist_head lnk;
		void* buf;
		int len;
	};

	dlist<record, &record::lnk> list;
	size_t total;

private:
	record* create(const void* buf, int len);
	int pop(record* rec, void* buf, int len);

public:
	int push_back(const void* buf, int len);
	int push_front(const void* buf, int len);
	
	int pop_back(void* buf, int len);
	int pop_front(void* buf, int len);

	strqueue() : list(), total(0) {};

	gxx::string to_str() const {
		gxx::string str;
		str.reserve(128);
		str << "[";
		for (auto r : const_cast<dlist<record, &record::lnk>&>(list)) {
			str << gxx::buffer(r.buf, r.len) << gxx::string(",");
		};
		str << "]";
		return str;
	};
};

#endif