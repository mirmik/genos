#ifndef IGRIS_SERIES_OBJECT_VIEW_H
#define IGRIS_SERIES_OBJECT_VIEW_H

#include <igris/series/field_view.h>
#include <stdio.h>

namespace igris
{
    class series_object_view
    {
    public:
        void *ptr = nullptr;
        std::shared_ptr<std::vector<series_field_annotation>> annotations = {};
        std::shared_ptr<std::map<std::string, series_field_annotation>>
            annotation_dict = {};

    public:
        series_object_view(
            void *ptr,
            std::shared_ptr<std::vector<series_field_annotation>> annotations,
            std::shared_ptr<std::map<std::string, series_field_annotation>>
                annotation_dict)
            : ptr(ptr), annotations(annotations),
              annotation_dict(annotation_dict)
        {
        }

        series_object_view() = default;
        series_object_view(const series_object_view &) = default;
        series_object_view &operator=(const series_object_view &) = default;
        series_object_view &operator=(series_object_view &&) = default;
        series_object_view(series_object_view &&) = default;

        series_field_view operator[](int i)
        {
            auto &annotation = (*annotations)[i];
            return series_field_view{(void *)((char *)ptr + annotation.offset),
                                     annotation};
        }

        series_field_view operator[](const std::string &name)
        {
            auto &annotation = annotation_dict->at(name);
            return series_field_view{(void *)((char *)ptr + annotation.offset),
                                     annotation};
        }

        size_t size()
        {
            return annotations->size();
        }
    };
}

#endif
