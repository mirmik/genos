#ifndef IGRIS_SERIALIZE_ARCHIVE_H
#define IGRIS_SERIALIZE_ARCHIVE_H

/**
@file
*/

#include <igris/buffer.h>
#include <igris/serialize/helper.h>

#if __has_include(<string_view>)
#include <string_view>
#endif

namespace igris
{
    namespace archive
    {
        template <typename T> struct data
        {
            T *ptr;
            size_t sz;
            data(T *ptr, size_t sz) : ptr(ptr), sz(sz) {}
            data(const T *ptr, size_t sz) : ptr((T *)ptr), sz(sz) {}
            template <typename R> void reflect(R &r)
            {
                r.do_data((char *)ptr, sz * sizeof(T));
            }
        };

        class binary_serializer_basic
        {
        public:
            template <typename T> void operator&(const T &obj)
            {
                igris::serialize(*this, obj);
            }

            virtual ~binary_serializer_basic() = default;
            virtual void dump_data(const char *dat, uint16_t sz) = 0;
            void do_data(const char *dat, uint16_t sz) { dump_data(dat, sz); }

            void dump(const char *dat, uint16_t sz)
            {
                dump(sz);
                dump_data(dat, sz);
            }

            void dump(char i) { dump_data((char *)&i, sizeof(i)); }
            void dump(signed char i) { dump_data((char *)&i, sizeof(i)); }
            void dump(unsigned char i) { dump_data((char *)&i, sizeof(i)); }

            void dump(short i) { dump_data((char *)&i, sizeof(i)); }
            void dump(int i) { dump_data((char *)&i, sizeof(i)); }
            void dump(long i) { dump_data((char *)&i, sizeof(i)); }
            void dump(unsigned short i) { dump_data((char *)&i, sizeof(i)); }
            void dump(unsigned int i) { dump_data((char *)&i, sizeof(i)); }
            void dump(unsigned long i) { dump_data((char *)&i, sizeof(i)); }
            void dump(unsigned long long i)
            {
                dump_data((char *)&i, sizeof(i));
            }
            void dump(float i) { dump_data((char *)&i, sizeof(i)); }
            void dump(double i) { dump_data((char *)&i, sizeof(i)); }
            void dump(long double i) { dump_data((char *)&i, sizeof(i)); }
            void dump(igris::buffer buf)
            {
                dump((uint16_t)buf.size());
                dump_data(buf.data(), buf.size());
            }

#if __has_include(<string_view>)
            void dump(std::string_view buf)
            {
                dump((uint16_t)buf.size());
                dump_data(buf.data(), buf.size());
            }
#endif

            template <typename T> void dump(const T &ref)
            {
                ((std::remove_cv_t<std::remove_reference_t<T>> &)(ref))
                    .reflect(*this);
            }
        };

        class binary_string_writer : public binary_serializer_basic
        {
        public:
            std::string &sstr;

            void dump_data(const char *dat, uint16_t size) override
            {
                sstr.append(dat, size);
            }

            binary_string_writer(std::string &str) : sstr(str) {}
        };

        class binary_buffer_writer : public binary_serializer_basic
        {
        public:
            char *ptr = nullptr;
            char *_end = nullptr;

            void dump_data(const char *dat, uint16_t size) override
            {
                memcpy(ptr, dat, size);
                ptr += size;
            }

            binary_buffer_writer(char *str, size_t size)
                : ptr(str), _end(str + size)
            {
            }

            binary_buffer_writer(const binary_buffer_writer&) = default;
            binary_buffer_writer& operator=(const binary_buffer_writer&) = default;

            virtual ~binary_buffer_writer() = default;
        };

        class writable_buffer : public igris::buffer
        {
        public:
            writable_buffer &operator=(const igris::buffer &oth)
            {
                igris::buffer::operator=(oth);
                return *this;
            }
        };

        class settable_buffer
        {
        public:
            igris::buffer &ref;

        public:
            settable_buffer(igris::buffer &buf) : ref(buf) {}
        };

        class binary_deserializer_basic
        {
        public:
            virtual ~binary_deserializer_basic() = default;
            template <typename T> void operator&(T &&obj)
            {
                igris::deserialize(*this, obj);
            }

            virtual void load_data(char *dat, uint16_t sz) = 0;
            void do_data(char *dat, uint16_t sz) { load_data(dat, sz); }

            void load(char *dat, uint16_t maxsz)
            {
                uint16_t sz;
                load(sz);
                if (sz > maxsz)
                    sz = maxsz;
                load_data(dat, sz);
            }

            void load(int8_t &i) { load_data((char *)&i, sizeof(i)); }
            void load(int16_t &i) { load_data((char *)&i, sizeof(i)); }
            void load(int32_t &i) { load_data((char *)&i, sizeof(i)); }
            void load(int64_t &i) { load_data((char *)&i, sizeof(i)); }

            void load(uint8_t &i) { load_data((char *)&i, sizeof(i)); }
            void load(uint16_t &i) { load_data((char *)&i, sizeof(i)); }
            void load(uint32_t &i) { load_data((char *)&i, sizeof(i)); }
            void load(uint64_t &i) { load_data((char *)&i, sizeof(i)); }

            /*void load(char& i) { load_data((char*)&i, sizeof(i)); }
            void load(short& i) { load_data((char*)&i, sizeof(i)); }
            void load(int& i) { load_data((char*)&i, sizeof(i)); }
            void load(long& i) { load_data((char*)&i, sizeof(i)); }
            void load(unsigned char& i) { load_data((char*)&i, sizeof(i)); }
            void load(unsigned short& i) { load_data((char*)&i, sizeof(i)); }
            void load(unsigned int& i) { load_data((char*)&i, sizeof(i)); }
            void load(unsigned long& i) { load_data((char*)&i, sizeof(i)); }
            void load(unsigned long long& i) { load_data((char*)&i, sizeof(i));
            }*/

            void load(float &i) { load_data((char *)&i, sizeof(i)); }
            void load(double &i) { load_data((char *)&i, sizeof(i)); }
            void load(long double &i) { load_data((char *)&i, sizeof(i)); }

            void load(igris::archive::settable_buffer &buf)
            {
                uint16_t len;
                load(len);

                buf.ref = igris::buffer((char *)pointer(), len);

                skip(len);
            }

            void load(igris::archive::writable_buffer &buf)
            {
                uint16_t len;
                load(len);

                int readsize = buf.size() < len ? buf.size() : len;
                load_data((char *)buf.data(), readsize);

                buf = igris::buffer(buf.data(), readsize);
            }

            void load_set_buffer(igris::buffer &buf)
            {
                igris::archive::settable_buffer arch(buf);
                load(arch);
            }

            template <typename T> void load(T &ref) { ref.reflect(*this); }

            virtual void skip(int ptr) = 0;
            virtual void *pointer() = 0;
            virtual const void *end() = 0;
        };

        /*class binary_string_reader : public binary_deserializer_basic
        {
        public:
            std::istringstream stream;

            void load_data(char* dat, uint16_t size) override
            {
                stream.read(dat, size);
            }

            binary_string_reader(const std::string& str) : stream(str) {}
        };*/

        class binary_buffer_reader : public binary_deserializer_basic
        {
        public:
            const char *ptr = nullptr;
            const char *_end = nullptr;

            void load_data(char *dat, uint16_t size) override
            {
                memcpy(dat, ptr, size);
                ptr += size;
            }

            void skip(int size) override { ptr += size; }

            void *pointer() override { return (void *)ptr; }
            const void *end() override { return _end; }

            binary_buffer_reader(const char *str, size_t size)
                : ptr(str), _end(str + size)
            {
            }
            binary_buffer_reader(igris::buffer buf)
                : ptr(buf.data()), _end(buf.data() + buf.size())
            {
            }

            binary_buffer_reader(const binary_buffer_reader&) = default;
            binary_buffer_reader& operator=(const binary_buffer_reader&) = default;

            virtual ~binary_buffer_reader() = default;
        };

        using binreader = binary_buffer_reader;
        using binwriter = binary_buffer_writer;
    }
}

#endif
