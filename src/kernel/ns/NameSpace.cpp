#include <kernel/ns/NameSpace.h>
#include <gxx/DList.h>

gxx::DList<NameSpace, &NameSpace::lnk> namespace_list;

NameSpace::NameSpace(const gxx::string& name) : m_name(name) {
	//namespace_list.push_back(*this);
	namespace_list.insert_sorted(*this);
}


void debug_print_namespace_list() {
	for(auto n : namespace_list) {
		debug_print(n.m_name.c_str());dln();
	}
};