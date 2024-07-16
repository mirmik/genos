#ifndef IGRIS_SERIES_FIELD_ITERATOR_H
#define IGRIS_SERIES_FIELD_ITERATOR_H

namespace igris
{
    class series_field_iterator
    {
        data_iterator objiter = {};
        field_annotation *annot = nullptr;

        operator*();
    }
}

#endif
