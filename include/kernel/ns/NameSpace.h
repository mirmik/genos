#ifndef GENOS_NAMESPACE_H
#define GENOS_NAMESPACE_H

#include <gxx/ByteArray.h>
#include <datastruct/dlist_head.h>

class File; //forward defenition

class NameSpace {
public:
	dlist_head lnk;

	gxx::string m_name;
	//virtual File* lookup(const char* str);

	bool operator < (const NameSpace& other) const {
		return m_name < other.m_name;
	}

	NameSpace(const gxx::string& name);
};

class TestNameSpace : public NameSpace {
public:
	TestNameSpace(const gxx::string& name) : NameSpace(name) {}
};

void debug_print_namespace_list();

#endif