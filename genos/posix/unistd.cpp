#include <genos/schedee.h>
#include <genos/schedee_api.h>
#include <unistd.h>

ssize_t write(int fd, const void *data, size_t size)
{
    genos::resource_table &resources =
        genos::current_schedee()->resource_table();

    if (resources.size() <= fd)
    {
        return -1;
    }

    auto &openr = resources[fd];
    auto res = openr.write(data, size);
    return res;
}

ssize_t read(int fd, void *data, size_t size)
{
    genos::resource_table &resources =
        genos::current_schedee()->resource_table();

    if (resources.size() <= fd)
        return -1;

    auto &openr = resources[fd];
    auto res = openr.read(data, size);
    return res;
}