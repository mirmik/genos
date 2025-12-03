#include <genos/schedee.h>
#include <genos/schedee_api.h>
#include <unistd.h>

ssize_t write(int fd, const void *data, size_t size)
{
    auto *sch = genos::current_schedee();
    if (sch == nullptr)
        return -1;

    genos::resource_table &resources = sch->resource_table();

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
    auto *sch = genos::current_schedee();
    if (sch == nullptr)
        return -1;

    genos::resource_table &resources = sch->resource_table();

    if (resources.size() <= fd)
        return -1;

    auto &openr = resources[fd];
    auto res = openr.read(data, size);
    return res;
}
