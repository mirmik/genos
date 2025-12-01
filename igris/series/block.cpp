#include <cassert>
#include <igris/series/block.h>
#include <igris/series/series.h>

void *igris::series_block::get(size_t num)
{
    return (void *)((char *)ptr + (elemsize() * (strt + num)));
}

const void *igris::series_block::get(size_t num) const
{
    return (const void *)((char *)ptr + (elemsize() * (strt + num)));
}

void *igris::series_block::last()
{
    assert(fini - 1 < size);
    return (void *)((char *)ptr + (elemsize() * (fini - 1)));
}

igris::series_block::series_block(igris::series *parent, void *ptr, size_t size)
    : parent(parent), ptr(ptr), size(size), strt(0), fini(0)
{
    assert(parent);
}

bool igris::series_block::has_place() const
{
    return size != fini;
}

void *igris::series_block::emplace()
{
    size_t pos = fini++;
    return (void *)((char *)ptr + (elemsize() * pos));
}

size_t igris::series_block::elemsize() const
{
    return parent->elemsize();
}
