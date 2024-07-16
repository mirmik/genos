#ifndef NOS_IO_STD_H
#define NOS_IO_STD_H

#include <iostream>
#include <nos/io/iostream.h>
#include <nos/io/buffered_file.h>

namespace nos
{
	extern buffered_file cout;
	extern buffered_file cerr;
	extern buffered_file cin;

}

#endif
