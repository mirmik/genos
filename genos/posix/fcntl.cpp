#include <fcntl.h>
#include <genos/resource/lookup.h>
#include <genos/schedee.h>

int open(const char *path, int __oflag)
{
    auto *sch = genos::current_schedee();
    if (sch == nullptr)
        return -1;

    auto &res = sch->resource_table();
    int fd = res.create_openres_fd();
    auto &ores = res[fd];
    int sts = ores.open(path, __oflag);
    if (sts < 0)
        return sts;
    return fd;
}
