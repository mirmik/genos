#ifndef __GENOS_OPTIONS_H
#define __GENOS_OPTIONS_H

#include <util/macro.h>

//#define OPTION(name,type) const type name = CAT(__option,name)
//#define OPTION_STRING(name) const char* name = CAT(__option,name)

#define OPTION(name) CAT(__option,name)

#endif