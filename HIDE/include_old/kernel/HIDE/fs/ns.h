#ifndef GENOS_NAMESPACE
#define GENOS_NAMESPACE

//#include <genos/defines/atomic_t.h>

#include <gxx/DList.h>
#include <gxx/ByteArray.h>

//#include <string.h>
//#include <datastruct/tree.h>

#include <kernel/fs/file.h>
#include <kernel/fs/chardev.h>
#include <kernel/fs/fops.h>
//#include <kernel/sched/resources.h>
//#include <datastruct/argvc.h>

#include <kernel/fs/inode.h>

#include <genos/panic.h>

namespace Kernel {

	class NamedObject : public Inode {
	public:
		enum Type {
			NOTYPE_CHARDEV,	
			NOTYPE_NAMESPACE,
			NOTYPE_JUSTNAME,	
			NOTYPE_STUBFILE,
		};

	public:
		dlist_head 			lnk;

		Type 				type;
		gxx::ByteArray 		name;
		//satomic_t			refs;

		virtual File* getFile() {
			dprln("Open NamedObject");
		};

		bool is_dir() {
			if (type == NOTYPE_NAMESPACE) return true;
			return false;
		}

		bool is_chardev() {
			if (type == NOTYPE_CHARDEV) return true;
			return false;
		}

		bool is_blockdev() {
			//if (type == NOTYPE_NAMESPACE) return true;
			return false;
		}

		int mkdir(const char* name);
		int mkchardev(const char* name, Kernel::CharDev* dev);

		NamedObject(const char* _name) : name(_name), type(NOTYPE_JUSTNAME) {
			dlist_init(&lnk);
		};

		NamedObject(const char* _name, Type _type) : name(_name), type(_type) {
			dlist_init(&lnk);
		};
	};


	class NamedCharDev : public NamedObject {
		CharDev* device;

	public:
		File* getFile() {
			return device;
		};	

		NamedCharDev(const char* _name, CharDev* _dev) : 
			NamedObject(_name, NamedObject::NOTYPE_STUBFILE),
			device(_dev) {};
	};

	class NameSpace;

	class NameSpaceStream : public File {		
	public:
		NameSpace* m_ns;
		gxx::DList<NamedObject,&NamedObject::lnk>::iterator it;


		NameSpaceStream(NameSpace* ns);

		int close() { delete this; };
		
		int write(const char* data, int length, uint8_t opt) {
			panic("lalala");
		}
		
		int read(char* data, int length, uint8_t opt) {
			panic("lalala");
		}

		Kernel::dirent* readdir(char* data, int lmax);
	};

	class NameSpace : public NamedObject {
	public:
		gxx::DList<NamedObject, &NamedObject::lnk> childs;

		NameSpace(const char* _name) : childs(), NamedObject(_name, NamedObject::NOTYPE_NAMESPACE) {};

		NamedObject* find(const char* target) {
			auto begin = childs.begin();
			auto end = childs.end();
			for (auto it = begin; it != end; ++it) {
				if (!strcmp(it->name.c_str(), target)) return &*it;
			}
			return nullptr;
		}

		void addObject(NamedObject* obj) {
			childs.push_back(*obj);
		}

		gxx::string to_str() const {
			gxx::string str;
			str.reserve(128);
			for(auto r : childs) {
				str << r.name << "\r\n";
			}
			return str;
		}

		File* getFile() {
			return new NameSpaceStream(this);
		};	
	};

	extern NameSpace rootNameSpace;
};

#endif