#ifndef GENOS_RESOURCE_FILE_HEAD_H
#define GENOS_RESOURCE_FILE_HEAD_H

#include <fcntl.h>
#include <igris/compiler.h>
#include <stdint.h>

namespace genos
{
    class file_head
    {
        uint8_t refs;
        uint8_t flags;

    public:
        bool is_open()
        {
            return refs;
        }
        virtual int write(const void *data, unsigned int size) = 0;
        virtual int read(void *read, unsigned int size) = 0;
        virtual int on_open() = 0;
        virtual int on_release() = 0;

        virtual int wait_for_avail()
        {
            return 0;
        }

        char getc()
        {
            char c;
            read(&c, 1);
            return c;
        }
    };
}

#endif