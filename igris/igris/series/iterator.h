#ifndef IGRIS_SERIES_ITERATOR_H
#define IGRIS_SERIES_ITERATOR_H

#include <igris/datastruct/dlist.h>
#include <iterator>

namespace igris
{
    class series_block;

    class series_iterator
    {
    public:
        dlist_head *block_lnk = nullptr;
        size_t num = 0;

    public:
        series_iterator() : block_lnk(nullptr), num(0){};
        series_iterator(dlist_head *block_lnk, size_t num);

        series_iterator(const series_iterator &) = default;

        bool inited()
        {
            return block_lnk != nullptr;
        }
        series_iterator &operator++();
        series_iterator operator++(int);
        series_iterator &operator--();
        series_iterator operator--(int);

        bool operator!=(const series_iterator &oth) const
        {
            return block_lnk != oth.block_lnk || num != oth.num;
        }

        bool operator==(const series_iterator &oth) const
        {
            return block_lnk == oth.block_lnk && num == oth.num;
        }

        void *pointer();
        const void *pointer() const;

        series_block *block();
        const series_block *block() const;

        template <class T> T &get()
        {
            return *(T *)pointer();
        }

        void *ptr()
        {
            return pointer();
        }

        /// Разметить над объектом шаблон для обращение через поля.
        template <class T> T &view()
        {
            return *(T *)pointer();
        }
    };
}

namespace std
{
    template <> struct iterator_traits<igris::series_iterator>
    {
        using difference_type = int;
        using iterator_category = std::bidirectional_iterator_tag;
    };
}

#endif
