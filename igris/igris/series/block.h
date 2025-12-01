#ifndef IGRIS_TABLE_SERIES_BLOCK_H
#define IGRIS_TABLE_SERIES_BLOCK_H

#include <igris/datastruct/dlist.h>

namespace igris
{
    class series;

    class series_block
    {
    public:
        dlist_head lnk = DLIST_HEAD_INIT(lnk);

    public:
        igris::series *parent = nullptr;
        void *ptr = nullptr;
        size_t size = 0;
        size_t strt = 0;
        size_t fini = 0;

    public:
        size_t elemsize() const;
        void *get(size_t num);
        const void *get(size_t num) const;
        void *last();

        series_block(igris::series *parent, void *ptr, size_t size);

        bool has_place() const;
        void *emplace();
    };
}

#endif
