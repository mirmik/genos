#include <kernel/fs/fops.h>
#include <kernel/ns/ns.h>
#include <kernel/sched/resources.h>

#include <assert.h>
#include <stdio.h>

void pathToList(char* str, char** v, argvc_t& argvc, const char* path) {
	if (path[0] != '/') {
		strcpy(str, Kernel::currentResources->pwd.c_str());
		strcpy(str + Kernel::currentResources->pwd.size(), path);
	} else
		strcpy(str, path);

	argvc.v = v; 

	argvc.internal_split(str, '/');
}

Kernel::Inode* Kernel::rootLookup(argvc_t& argvc) {
	
	NamedObject* nobj = &rootNameSpace;

	for(int c = 0; c < argvc.c; ++c) {
		if (nobj->type != Kernel::NamedObject::NOTYPE_NAMESPACE) {
			Kernel::currentResources->__errno = Error_WrongDir;
			return nullptr;
		}

		nobj = ((NameSpace*)nobj)->find(argvc.v[c]);
		if (nobj == nullptr)  {
			Kernel::currentResources->__errno = Error_WrongPath;
			return nullptr;
		}
	}

	return nobj;
}


Kernel::Inode* Kernel::rootLookup(const char* path) {
	char str[128];
	char* v[10];
	argvc_t argvc;

	pathToList(str, v, argvc, path);
	return rootLookup(argvc);
}

int Kernel::open(const char* path, int mode) {
	//printf("open %s", path);

	Kernel::Resources* res = Kernel::currentResources;
	assert(res);

	Inode* inode = rootLookup(path);
	if (inode == nullptr) return -1;

	//if (inode->is_dir()) {
	//	Kernel::currentResources->__errno = Error_IsDir;
	//	return -1;
	//}

	File* file = inode->getFile();
	if (file == nullptr) return -1;

	int fd = 0;
	for(; fd < res->fd_vector.size(); ++fd) {
		if (res->fd_vector[fd] == nullptr) break;
	};

	res->fd_vector[fd] = file;

	return fd;
}

int Kernel::write(int fd, const char* data, int length) {
	Kernel::Resources* res = Kernel::currentResources;
	assert(res);

	if (fd >= Kernel::currentResources->fd_vector.size()) { 
		Kernel::currentResources->__errno = Error_WrongFD;
		return -1;
	}

	File* file = Kernel::currentResources->fd_vector[fd];
	
	if (file == nullptr) { 
		Kernel::currentResources->__errno = Error_WrongFD;
		return -1;
	}

	return file -> write(data, length, 0);
}



Kernel::dirent* Kernel::readdir(int fd, char* buf, int lmax) {
	Kernel::Resources* res = Kernel::currentResources;
	assert(res);

	if (fd >= Kernel::currentResources->fd_vector.size()) { 
		Kernel::currentResources->__errno = Error_WrongFD;
		return nullptr;
	}

	File* file = Kernel::currentResources->fd_vector[fd];
	
	if (file == nullptr) { 
		Kernel::currentResources->__errno = Error_WrongFD;
		return nullptr;
	}

	return file -> readdir(buf, lmax); 
}

int Kernel::mkdir(const char* path) {
	char str[128];
	char* v[10];
	argvc_t argvc;

	pathToList(str, v, argvc, path);
	argvc.c--;

	Inode* parent = rootLookup(argvc);	

	return parent -> mkdir(argvc.v[argvc.c]);
}

int Kernel::mkchardev(const char* path, Kernel::CharDev* dev) {
	char str[128];
	char* v[10];
	argvc_t argvc;

	pathToList(str, v, argvc, path);
	argvc.c--;

	Inode* parent = rootLookup(argvc);	

	return parent -> mkchardev(argvc.v[argvc.c], dev);
}