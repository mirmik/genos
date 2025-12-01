#include <igris/series/field_annotation.h>
#include <igris/series/iterator.h>

igris::series_field_annotation::series_field_annotation(
    const std::string &machname,
    const std::string &username,
    size_t offset,
    size_t size,
    FieldDataType datatype)
    : machname(machname), username(username), offset(offset), size(size),
      datatype(datatype), u()
{
}

double
igris::series_field_annotation::expand_numeric(const void *record_pointer) const
{
    char *ptr = ((char *)record_pointer) + offset;

    switch (datatype)
    {
    case FieldDataType::Float:
        return *(float *)ptr;
    case FieldDataType::Double:
        return *(double *)ptr;
    case FieldDataType::I8:
        return *(int8_t *)ptr;
    case FieldDataType::I16:
        return *(int16_t *)ptr;
    case FieldDataType::I32:
        return *(int32_t *)ptr;
    case FieldDataType::I64:
        return static_cast<double>(*(int64_t *)ptr);
    case FieldDataType::U8:
        return *(uint8_t *)ptr;
    case FieldDataType::U16:
        return *(uint16_t *)ptr;
    case FieldDataType::U32:
        return *(uint32_t *)ptr;
    case FieldDataType::U64:
        return static_cast<double>(*(uint64_t *)ptr);
    case FieldDataType::Bool:
        return *(bool *)ptr;
    default:
        return 0;
    }

    return 0;
}

double igris::series_field_annotation::expand_numeric(
    const series_iterator &iterator) const
{
    return expand_numeric(iterator.pointer());
}

double
igris::series_field_annotation::expand(const series_iterator &iterator) const
{
    return expand_numeric(iterator.pointer());
}

bool igris::series_field_annotation::is_scatter() const
{
    return u.f.scatter;
}

igris::series_field_annotation &igris::series_field_annotation::scatter(bool en)
{
    u.f.scatter = en;
    return *this;
}
