#ifndef NOS_IO_IOSTREAM_H
#define NOS_IO_IOSTREAM_H

#include <nos/io/istream.h>
#include <nos/io/ostream.h>

namespace nos
{
    class iostream : public istream, public ostream
    {
    };
}

#endif
