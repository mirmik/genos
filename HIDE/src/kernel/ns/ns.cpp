#include <kernel/ns/ns.h>

namespace Kernel {

	Kernel::NameSpace rootNameSpace("/");

};

Kernel::NameSpaceStream::NameSpaceStream(Kernel::NameSpace* ns) : m_ns(ns)
	, it(m_ns->childs.begin()) {};

Kernel::dirent* Kernel::NameSpaceStream::readdir(char* data, int lmax) {
	
	if (m_ns->childs.end() == it) return nullptr;

	Kernel::dirent* dir = new (data) Kernel::dirent;
	dir->type = m_ns->type;
	strcpy(dir->name, (it++)->name.c_str()); 
	return dir;
}

int Kernel::NamedObject::mkdir(const char* name)  {
	if (!is_dir()) { 
		return -1; 
	}

	NameSpace* parent = (NameSpace*) this;
	NameSpace* child  = new NameSpace(name);
	
	parent->childs.push_back(*child);
	return 0;
};

int Kernel::NamedObject::mkchardev(const char* name, Kernel::CharDev* dev)  {
	if (!is_dir()) { 
		return -1; 
	}

	NameSpace* parent = (NameSpace*) this;
	NamedCharDev* child  = new NamedCharDev(name, dev);
	
	parent->childs.push_back(*child);
	return 0;
};