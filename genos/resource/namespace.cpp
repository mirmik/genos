#include <genos/resource/namespace.h>
#include <igris/container/dlist.h>
#include <string.h>

INIT_PRIORITY(140)
igris::dlist<genos::namespace_manager,
             &genos::namespace_manager::namespace_list_lnk>
    genos::namespace_list;

genos::namespace_manager::namespace_manager(const char *path)
{
    _path = path;
    genos::namespace_list.move_back(*this);
}

genos::namespace_manager::~namespace_manager()
{
    genos::namespace_list.pop(*this);
}
