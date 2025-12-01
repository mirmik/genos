#include <igris/datastruct/dlist.h>
#include <igris/math.h>
#include <igris/series/block.h>
#include <igris/series/iterator.h>
#include <igris/series/series.h>
#include <igris/util/bug.h>
#include <igris/util/numconvert.h>
#include <igris/util/string.h>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

igris::series::series() : _elemsize(0) {}
igris::series::series(size_t elemsize) : _elemsize(elemsize) {}

void igris::series::set_elemsize(size_t size)
{
    _elemsize = size;
}

void igris::series::reserve(size_t size)
{
    add_block(size);
}

void igris::series::add_block(size_t size)
{
    void *ptr = allocator.allocate(size * _elemsize);
    memset(ptr, 0, size * _elemsize);
    auto *block = new series_block(this, ptr, size);

    dlist_add(&block->lnk, &blocks);
}

size_t igris::series::right_capacity() const
{
    size_t accum = 0;

    igris::series_block *block;
    dlist_for_each_entry(block, &blocks, lnk)
    {
        accum += block->size - block->fini;
    }

    return accum;
}

igris::series::~series()
{
    igris::series_block *block;
    while (!dlist_empty(&blocks))
    {
        block = dlist_first_entry(&blocks, igris::series_block, lnk);
        dlist_del(&block->lnk);
        allocator.deallocate((char *)block->ptr, block->size * _elemsize);
        delete (block);
    }
}

size_t igris::series::size() const
{
    size_t accum = 0;

    igris::series_block *block;
    dlist_for_each_entry(block, &blocks, lnk)
    {
        accum += block->fini - block->strt;
    }

    return accum;
}

void *igris::series::emplace()
{
    igris::series_block *block;
    dlist_for_each_entry(block, &blocks, lnk)
    {
        if (block->has_place())
        {
            return block->emplace();
        }
    }

    add_block(block_size_hint);
    return last_block()->emplace();
}

igris::series_field_annotator &igris::series::annotator()
{
    return _annotator;
}

void igris::series::push_csv_string_parse(const std::string &str)
{
    auto lst = igris::split(str, ',');
    void *ptr = emplace();
    auto view = object_view(ptr);

    for (unsigned int i = 0; i < MIN(lst.size(), view.size()); ++i)
    {
        double data = strtod(lst[i].c_str(), nullptr);
        view[i].assign(data);
    }
}

igris::series_object_view igris::series::emplace_and_get_view()
{
    void *ptr = emplace();
    return series_object_view(
        ptr, _annotator._annotations, _annotator._annotations_dict);
}

int igris::series::push_object(void *data, size_t size)
{
    void *ptr = emplace();
    memcpy(ptr, data, elemsize());

    if (size != elemsize())
        return -1;

    return 0;
}

igris::series_iterator igris::series::get_iterator(size_t num)
{
    size_t accum = 0;
    size_t saccum;

    igris::series_block *block;
    dlist_for_each_entry(block, &blocks, lnk)
    {
        saccum = accum;
        accum += block->fini - block->strt;

        if (accum > num)
        {
            return igris::series_iterator(&block->lnk,
                                          block->strt + (num - saccum));
        }
    }

    return end();
    // throw std::range_error("series_iterator range error");
}

igris::series_iterator igris::series::begin()
{
    return {blocks.next, dlist_first_entry(&blocks, series_block, lnk)->strt};
}

igris::series_iterator igris::series::begin() const
{
    return const_cast<igris::series *>(this)->begin();
}
igris::series_iterator igris::series::end() const
{
    return const_cast<igris::series *>(this)->end();
}

igris::series_iterator igris::series::last_iterator()
{
    return {blocks.prev,
            dlist_last_entry(&blocks, series_block, lnk)->fini - 1};
}

igris::series_iterator igris::series::end()
{
    return {&blocks, 0};
}

igris::series_block *igris::series::last_block()
{
    return dlist_entry(blocks.prev, series_block, lnk);
}

igris::series_field_annotation *
igris::series::find_annotation(const std::string &name)
{
    return _annotator.find_annotation(name);
}

void igris::series::parse_csv_istream(std::istream &input)
{
    std::string str;

    // read header
    std::getline(input, str);
    auto headers = igris::split(str, ',');

    // Пока считаем все поля за тип double
    for (auto &header : headers)
    {
        annotator().add<double>(header);
    }
    set_elemsize(headers.size() * sizeof(double));

    while (std::getline(input, str))
    {
        auto values = igris::split(str, ',');
        double *data = (double *)emplace();
        for (size_t i = 0; i < values.size(); ++i)
        {
            data[i] = std::stod(values[i]);
        }
    }
}

void igris::series::parse_csv_file(const std::string &path)
{
    std::fstream file(path);
    if (!file)
    {
        throw std::invalid_argument("wrong file path");
    }
    parse_csv_istream(file);
}

std::vector<std::string> igris::series::headers() const
{
    std::vector<std::string> result;
    for (auto &annot : const_cast<igris::series_field_annotator &>(_annotator)
                           .annotations_ref())
    {
        result.push_back(annot.machname);
    }
    return result;
}

igris::series igris::series::slice(size_t start, size_t end)
{
    igris::series result;
    result.set_elemsize(_elemsize);
    result.reserve(end - start);

    auto &annotator = result.annotator();
    annotator = _annotator;

    igris::series_iterator it = std::next(begin(), start);
    igris::series_iterator eit = std::next(begin(), end);

    for (; it != eit; ++it)
    {
        void *ptr = result.emplace();
        memcpy(ptr, it.ptr(), _elemsize);
    }

    return result;
}