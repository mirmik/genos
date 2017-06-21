#ifndef GENOS_RAMFS_DIRECTORY_H
#define GENOS_RAMFS_DIRECTORY_H

#include <gxx/Vector.h>

class RamAbstractFile {
	const char* name;
};

//class RamFile {};

class RamDirectory : public RamFile {
	gxx::Vector<RamFile*> childs;
};

#endif