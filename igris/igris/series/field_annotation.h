#ifndef IGRIS_series_field_annotation_H
#define IGRIS_series_field_annotation_H

#include <string>

namespace igris
{
    class series_iterator;

    enum class FieldDataType
    {
        Float,
        Double,

        I8,
        I16,
        I32,
        I64,

        U8,
        U16,
        U32,
        U64,

        Bool,
        Undefined
    };

    class series_field_annotation
    {
    public:
        std::string machname = {}; // имя поля
        std::string username = {}; // имя поля
        size_t offset = 0;         // смещение в битах
        size_t size = 0;           // размер
        FieldDataType datatype = FieldDataType::Undefined; // тип

        union _u
        {
            uint8_t flags;
            struct _f
            {
                uint8_t scatter : 1; // подсказка, что данные следует отображать
                // поточечно
            } f = {};

            _u() : flags(0) {}
        } u = {};

    public:
        series_field_annotation(const std::string &machname,
                                const std::string &username,
                                size_t offset,
                                size_t size,
                                FieldDataType datatype);

        series_field_annotation() = default;
        series_field_annotation(const series_field_annotation &oth) = default;
        series_field_annotation &
        operator=(const series_field_annotation &oth) = default;

        bool is_scatter() const;
        series_field_annotation &scatter(bool en);

        double expand_numeric(const void *record_pointer) const;
        double expand_numeric(const series_iterator &iterator) const;
        double expand(const series_iterator &iterator) const;
    };

    template <class T, FieldDataType Field = FieldDataType::Undefined>
    struct series_field_annotation_helper
    {
        constexpr static int size = sizeof(T);
        constexpr static FieldDataType type = Field;
    };

    template <>
    struct series_field_annotation_helper<bool>
        : series_field_annotation_helper<bool, FieldDataType::Bool>
    {
    };

    template <>
    struct series_field_annotation_helper<int8_t>
        : series_field_annotation_helper<int8_t, FieldDataType::I8>
    {
    };
    template <>
    struct series_field_annotation_helper<int16_t>
        : series_field_annotation_helper<int16_t, FieldDataType::I16>
    {
    };
    template <>
    struct series_field_annotation_helper<int32_t>
        : series_field_annotation_helper<int32_t, FieldDataType::I32>
    {
    };
    template <>
    struct series_field_annotation_helper<int64_t>
        : series_field_annotation_helper<int64_t, FieldDataType::I64>
    {
    };

    template <>
    struct series_field_annotation_helper<uint8_t>
        : series_field_annotation_helper<uint8_t, FieldDataType::U8>
    {
    };
    template <>
    struct series_field_annotation_helper<uint16_t>
        : series_field_annotation_helper<uint16_t, FieldDataType::U16>
    {
    };
    template <>
    struct series_field_annotation_helper<uint32_t>
        : series_field_annotation_helper<uint32_t, FieldDataType::U32>
    {
    };
    template <>
    struct series_field_annotation_helper<uint64_t>
        : series_field_annotation_helper<uint64_t, FieldDataType::U64>
    {
    };

    template <>
    struct series_field_annotation_helper<float>
        : series_field_annotation_helper<float, FieldDataType::Float>
    {
    };
    template <>
    struct series_field_annotation_helper<double>
        : series_field_annotation_helper<double, FieldDataType::Double>
    {
    };

    template <class T>
    series_field_annotation make_series_field_annotation(
        const std::string &machname, const std::string &username, int offset)
    {
        return series_field_annotation(machname,
                                       username,
                                       offset,
                                       series_field_annotation_helper<T>::size,
                                       series_field_annotation_helper<T>::type);
    }
}

#endif
