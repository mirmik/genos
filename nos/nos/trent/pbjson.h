#ifndef NOS_TRENT_PBJSON_H
#define NOS_TRENT_PBJSON_H

#include <nos/trent/trent.h>
#include <nos/util/buffer.h>

namespace nos
{
    namespace pbjson
    {
        class parser
        {
        public:
            nos::buffer buf = {};

        public:
            parser() = default;
            parser(const nos::buffer &buf) : buf(buf) {}

            std::pair<int, int> get_type_and_length()
            {
                // type  located in the first 3 bits
                // length located in the last 5 bits
                uint8_t type_and_length = buf[0];
                uint8_t type = (type_and_length & 0b11100000) >> 4;
                uint8_t length =
                    type_and_length & 0x1F; // todo length more than 15
                buf = {buf.data() + 1, buf.size() - 1};
                return {type, length};
            }

            int64_t parse_int(size_t length, bool is_negative)
            {
                int64_t v = 0;
                for (size_t i = 0; i < length; i++)
                {
                    v = (v << 8) | buf[i];
                }
                if (is_negative)
                    v = -v;
                buf = {buf.data() + length, buf.size() - length};
                return v;
            }

            double parse_double(size_t length)
            {
                union
                {
                    uint8_t bits[8];
                    double value;
                };

                for (size_t i = 0; i < length; i++)
                {
                    bits[8 - length + i] = buf[i];
                }
                return value;
            }

            std::string parse_string(size_t length)
            {
                std::string value(buf.data(), length);
                buf = {buf.data() + length, buf.size() - length};
                return value;
            }

            std::string parse_key()
            {
                uint8_t size = buf[0];
                buf = {buf.data() + 1, buf.size() - 1};
                return parse_string(size);
            }

            nos::trent parse_array(size_t length)
            {
                nos::trent value;
                value.as_list();
                for (size_t i = 0; i < length; ++i)
                {
                    auto tr = parse();
                    value.push_back(tr);
                }
                return value;
            }

            nos::trent parse_object(size_t length)
            {
                nos::trent value;
                value.as_dict();
                for (size_t i = 0; i < length; ++i)
                {
                    auto key = parse_key();
                    auto tr = parse();
                    value[key] = tr;
                }
                return value;
            }

            nos::trent parse()
            {
                nos::trent tr;

                auto [type, length] = get_type_and_length();

                if (type == 2 || type == 4)
                    tr = parse_int(length, type == 4);

                if (type == 6)
                    tr = parse_double(length);

                if (type == 8)
                    tr = parse_string(length);

                if (type == 0xC)
                    tr = parse_array(length);

                if (type == 0xE)
                    tr = parse_object(length);

                return tr;
            }
        };
    }
}

#endif