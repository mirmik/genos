#ifndef IGRIS_FIELD_ANNOTATOR_H
#define IGRIS_FIELD_ANNOTATOR_H

#include <cassert>
#include <igris/series/field_annotation.h>
#include <igris/util/size_incrementor.h>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

namespace igris
{
    class series_field_annotator
    {
    public:
        igris::size_incrementor inc = {};
        std::shared_ptr<std::vector<series_field_annotation>> _annotations =
            std::make_shared<std::vector<series_field_annotation>>();
        std::shared_ptr<std::map<std::string, series_field_annotation>>
            _annotations_dict = std::make_shared<
                std::map<std::string, series_field_annotation>>();

    public:
        series_field_annotator() = default;
        series_field_annotator(const series_field_annotator &oth) = default;
        series_field_annotator(series_field_annotator &&oth) = default;

        series_field_annotator &
        operator=(const series_field_annotator &oth) = default;
        series_field_annotator &
        operator=(series_field_annotator &&oth) = default;

        series_field_annotator(int offset) : inc(offset)
        {
            assert(_annotations->size() == _annotations_dict->size());
        }
        series_field_annotator(const igris::size_incrementor &oth) : inc(oth)
        {
            assert(_annotations->size() == _annotations_dict->size());
        }

        std::shared_ptr<std::map<std::string, series_field_annotation>>
        annotations_dict()
        {
            return _annotations_dict;
        }

        template <class T>
        series_field_annotation &add(const std::string &machname,
                                     const std::string &username)
        {
            auto annot = igris::make_series_field_annotation<T>(
                machname, username, inc.increment<T>());
            _annotations->push_back(annot);
            (*_annotations_dict)[machname] = _annotations->back();
            return _annotations->back();
        }

        template <class T> series_field_annotation &add(const std::string &name)
        {
            return add<T>(name, name);
        }

        std::shared_ptr<std::vector<series_field_annotation>> annotations()
        {
            return _annotations;
        }

        std::vector<series_field_annotation> &annotations_ref()
        {
            return *_annotations;
        }

        const std::vector<series_field_annotation> &annotations_ref() const
        {
            return *_annotations;
        }

        series_field_annotation *find_annotation(const std::string &name)
        {
            for (auto &obj : *_annotations)
            {
                if (name == obj.machname)
                    return &obj;
            }

            return nullptr;
        }

        series_field_annotation *find(const std::string &name)
        {
            for (auto &a : *_annotations)
            {
                if (a.machname == name)
                    return &a;
            }
            return nullptr;
        }
    };
}

#endif
