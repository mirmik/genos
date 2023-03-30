#ifndef GENOS_RESOURCE_NAMESPACE_H
#define GENOS_RESOURCE_NAMESPACE_H

#include <genos/resource/file_head.h>
#include <igris/compiler.h>
#include <igris/container/dlist.h>

namespace genos
{
    class namespace_manager
    {
        const char *_path;

    public:
        dlist_head namespace_list_lnk = DLIST_HEAD_INIT(namespace_list_lnk);

    public:
        namespace_manager(const char *zone);
        ~namespace_manager();
        virtual int lookup(file_head **filpp, const char *path) = 0;
        const char *path()
        {
            return _path;
        }
    };

    extern igris::dlist<namespace_manager,
                        &namespace_manager::namespace_list_lnk>
        namespace_list;
}

#endif