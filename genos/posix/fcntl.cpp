#include <fcntl.h>
#include <genos/resource/lookup.h>
#include <genos/schedee.h>

int open(const char *path, int __oflag)
{
    auto *sch = genos::current_schedee();
    auto &res = sch->resource_table();
    auto &ores = res.create_openres();
    return ores.open(path, __oflag);
}