#ifndef GENOS_RESOURCE_OPENRES_H
#define GENOS_RESOURCE_OPENRES_H

#include <genos/resource/file_head.h>
#include <igris/compiler.h>

namespace genos
{
    class openres
    {
        genos::file_head *file = nullptr;
        int pos = 0;

    public:
        void init(genos::file_head *file)
        {
            this->file = file;
            pos = 0;
        }
    };
}

#endif