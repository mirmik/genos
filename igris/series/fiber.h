#ifndef IGRIS_SERIES_FIBER_H
#define IGRIS_SERIES_FIBER_H

#include <igris/series/field_annotation.h>
#include <igris/series/iterator.h>
#include <igris/series/series.h>

namespace igris
{
    template <class T> class series_fiber_iterator
    {
    public:
        igris::series_iterator it = {};
        const igris::series_field_annotation *annot = nullptr;

        series_fiber_iterator(igris::series_iterator it,
                              const igris::series_field_annotation *annot)
            : it(it), annot(annot)
        {
        }

        series_fiber_iterator(const series_fiber_iterator &oth)
            : it(oth.it), annot(oth.annot)
        {
        }

        series_fiber_iterator &operator=(const series_fiber_iterator &oth)
        {
            it = oth.it;
            annot = oth.annot;
            return *this;
        }

        T &operator*()
        {
            auto offset = annot->offset;
            auto ptr = it.pointer();
            return *reinterpret_cast<T *>(((char *)ptr) + offset);
        }

        series_fiber_iterator &operator++()
        {
            ++it;
            return *this;
        }

        series_fiber_iterator operator++(int)
        {
            series_fiber_iterator tmp = *this;
            ++it;
            return tmp;
        }
    };

    template <class T> class series_fiber
    {
    public:
        igris::series &series;
        const igris::series_field_annotation &annot;

    public:
        series_fiber(igris::series &series,
                     const igris::series_field_annotation &annot)
            : series(series), annot(annot)
        {
        }

        series_fiber_iterator<T> get_fiber_iterator(int num)
        {
            return {series.get_iterator(num), &annot};
        }
    };
}

#endif