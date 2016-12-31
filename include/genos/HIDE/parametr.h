#ifndef GENOS_PARAMETR_H
#define GENOS_PARAMETR_H

#include "gxx/container/hashtable.h"
#include <genos/serialization/serialization.h>
#include <genos/sigslot/sigslot.h>

#include <genos/metainf.h>
#include <genos/atomic.h>

class AbstractParametr {
public:
	hlist_node m_link;
	const char* m_name;
	//uint8_t m_type;

	AbstractParametr(const char* nname) : m_name(nname) {};

	const char* getName() const {
		return m_name;
	};

	void setName(const char* nname) {
		m_name = nname;
	};

	virtual unsigned int rserial(char* buf, uint8_t flag) = 0;
	virtual gxx::string to_str() const = 0;
};

template <typename T>
class Parametr : public AbstractParametr {
private:
	T m_data;
	genos::signal<Parametr<T>&> updateValue;

	template<typename Arg>
	void update(Arg arg) {
		m_data = gxx::forward<Arg>(arg);
		updateValue(*this);
	};

public:
	template<typename ... Args>
	Parametr(const char* nname, Args&& ... args) : m_data(gxx::forward<Args ...>(args ...)) 
		, AbstractParametr(nname) {};

	gxx::string to_str() const {
		lock_atomic lock;
		return gxx::string(m_name) + ":" + ::to_str(m_data);
	}

	unsigned int rserial(char* buf, uint8_t flag) {
		lock_atomic lock;
		::rserial(buf, &m_data, flag);
	}

	template<typename Arg>
	void atomicUpdate(Arg arg) {
		lock_atomic atom;
		update(gxx::forward<Arg>(arg));
	};

	template<typename Arg>
	inline Parametr& operator= (Arg value) {
		atomicUpdate(gxx::forward<Arg>(value));
		return *this;
	}

	T value() const {
		return m_data;
	}
};

hashtable<
		AbstractParametr, &AbstractParametr::m_link, const char*, &AbstractParametr::m_name
	> parametrTable;


#endif