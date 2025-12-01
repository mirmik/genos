#ifndef IGRIS_SERIES_FIELD_VIEW_H
#define IGRIS_SERIES_FIELD_VIEW_H

#include <igris/series/field_annotation.h>
#include <string.h>

namespace igris
{
    class series_field_view
    {
        void *ptr = nullptr;
        const series_field_annotation &annotation = {};

    public:
        series_field_view(void *ptr, const series_field_annotation &annotation)
            : ptr(ptr), annotation(annotation)
        {
        }

        /*series_field_view &operator=(const series_field_view &oth)
        {
            memcpy(ptr, oth.ptr, annotation.size);
            return *this;
        }*/

        series_field_view &operator=(double data)
        {
            assign(data);
            return *this;
        }

        void assign(double data)
        {
            switch (annotation.datatype)
            {
            case FieldDataType::Float:
            {
                float fdata = static_cast<float>(data);
                memcpy(ptr, &fdata, sizeof(float));
            }
            break;
            case FieldDataType::Double:
            {
                memcpy(ptr, &data, sizeof(double));
            }
            break;
            case FieldDataType::I8:
            {
                int8_t i8data = static_cast<int8_t>(data);
                memcpy(ptr, &i8data, sizeof(int8_t));
            }
            break;
            case FieldDataType::I16:
            {
                int16_t i16data = static_cast<int16_t>(data);
                memcpy(ptr, &i16data, sizeof(int16_t));
            }
            break;
            case FieldDataType::I32:
            {
                int32_t i32data = static_cast<int32_t>(data);
                memcpy(ptr, &i32data, sizeof(int32_t));
            }
            break;
            case FieldDataType::I64:
            {
                int64_t i64data = static_cast<int64_t>(data);
                memcpy(ptr, &i64data, sizeof(int64_t));
            }
            break;
            case FieldDataType::U8:
            {
                uint8_t u8data = static_cast<uint8_t>(data);
                memcpy(ptr, &u8data, sizeof(uint8_t));
            }
            break;
            case FieldDataType::U16:
            {
                uint16_t u16data = static_cast<uint16_t>(data);
                memcpy(ptr, &u16data, sizeof(uint16_t));
            }
            break;
            case FieldDataType::U32:
            {
                uint32_t u32data = static_cast<uint32_t>(data);
                memcpy(ptr, &u32data, sizeof(uint32_t));
            }
            break;
            case FieldDataType::U64:
            {
                uint64_t u64data = static_cast<uint64_t>(data);
                memcpy(ptr, &u64data, sizeof(uint64_t));
            }
            break;
            case FieldDataType::Bool:
            {
                bool bdata = static_cast<bool>(data);
                memcpy(ptr, &bdata, sizeof(bool));
            }
            break;

            case FieldDataType::Undefined:
                break;
            }
        }
    };
}

#endif
