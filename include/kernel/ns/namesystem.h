#ifndef GENOS_NAMESYSTEM_H
#define GENOS_NAMESYSTEM_H

#include <gxx/ByteArray.h>

enum FileType {
	Directory = 1,
	CharDev = 2,
	BlockDev = 4,
};

struct Inode {		
	virtual FileAbstract* openFile() = 0;

	//virtual bool is_chardev() = 0;
	//virtual bool is_blockdev() = 0;	
	//virtual bool is_dir() = 0;

	//virtual int mkdir(const char* path) {
	//	return -1;
	//};

	//virtual int mkchardev(const char* path, Kernel::CharDev* dev) {
	//	return -1;
	//};	
};

struct NamedObject : public Inode {
	//manage
	dlist_head lnk;

	//data
	gxx::string m_name;
	uint8_t m_flags;

	NamedObject(const char* name, uint8_t flags) 
		: m_name(name), m_flags(flags) {
		dlist_init(&lnk);
	};
};

struct NamedCharDev : public NamedObject {
	DeviceFile* m_dev;

	FileAbstract* openFile() {
		return m_dev;
	};	

	NamedCharDev(const char* name, DeviceFile* dev) : 
		NamedObject(name, FileType::CharDev),
		m_dev(dev) {};
};

struct NameSpace : public NamedObject {
	gxx::DList<NamedObject, &NamedObject::lnk> childs;

	NameSpace(const char* name) 
		: childs(), NamedObject(name, FileType::Directory) {};

	NamedObject* find(const char* target) {
		auto begin = childs.begin();
		auto end = childs.end();
		for (auto it = begin; it != end; ++it) {
			if (!strcmp(it->m_name.c_str(), target)) return &*it;
		}
		return nullptr;
	}

	void addObject(NamedObject* obj) {
		childs.push_back(*obj);
	}

	gxx::string to_str() const {
		gxx::string str;
		str.reserve(128);
		auto begin = childs.begin();
		auto end = childs.end();
		for(auto it = begin; it != end; ++it) {
			str << it->m_name << "\r\n";
		}
		return str;
	}

	FileAbstract* openFile() {
		panic("TODO");
	//	return new NameSpaceStream(this);
	};	
};

#endif