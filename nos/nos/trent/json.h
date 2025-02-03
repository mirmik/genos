#ifndef nos_TRENT_JSON_H
#define nos_TRENT_JSON_H

/**
    @file
*/

#include <exception>
#include <nos/trent/trent.h>
#include <nos/util/numconvert.h>
#include <sstream>

using namespace std::literals::string_literals;

namespace nos
{
    namespace json
    {
        class parser
        {
            int symbno = 0;
            int lineno = 1;
            char onebuf = 0;

            class unexpected_symbol : public std::runtime_error
            {
            public:
                unexpected_symbol(char symb)
                    : std::runtime_error(std::string("unexpected:") + symb)
                {
                }
            };

        public:
            virtual ~parser() {}
            virtual char readnext_impl() = 0;

            char readnext()
            {
                symbno++;
                char c = readnext_impl();
                if (c == '\n')
                {
                    lineno++;
                    symbno = 0;
                }
                return c;
            }

            std::string errloc()
            {
                return " lineno:" + std::to_string(lineno) +
                       " symbno:" + std::to_string(symbno);
            }

            char readnext_skipping()
            {
                char c;

            __try__:
                c = readnext();

                if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
                    goto __try__;

                if (c == '/')
                {
                    c = readnext();

                    switch (c)
                    {
                    case '*':
                        while (true)
                        {
                            c = readnext();

                            if (c == '*')
                                if ((c = readnext()) == '/')
                                {
                                    goto __try__;
                                }
                        }

                    case '/':

                        // is.ignore(INT_MAX, '\n');
                        while (c != '\n')
                            c = readnext();

                        goto __try__;

                    default:
                        // is.unget();
                        break;
                    }
                }

                // is.unget();
                return c;
            }

            template <template <class Allocator> class TAlloc = std::allocator>
            trent_basic<TAlloc> parse()
            {
                // TRACE();

                if (onebuf == 0)
                {
                    onebuf = readnext_skipping();
                }

                if (isdigit(onebuf) || onebuf == '-')
                    return parse_numer();

                if (onebuf == '"')
                    return parse_string();

                if (onebuf == '\'')
                    return parse_string();

                if (onebuf == '[')
                    return parse_list();

                if (onebuf == '{')
                    return parse_dict();

                if (isalpha(onebuf))
                    return parse_mnemonic();

                throw std::runtime_error("unexpected_symbol "s + onebuf +
                                         errloc());
            }

            template <template <class Allocator> class TAlloc = std::allocator>
            trent_basic<TAlloc> parse_mnemonic()
            {
                char buf[32];
                char *ptr = &buf[1];

                buf[0] = onebuf;

                while (isalpha(onebuf = readnext_skipping()))
                {
                    *ptr++ = onebuf;
                }

                *ptr = 0;

                if (strcmp("true", buf) == 0)
                {
                    return true;
                }
                if (strcmp("false", buf) == 0)
                {
                    return false;
                }
                if (strcmp("nil", buf) == 0 || strcmp("null", buf) == 0)
                {
                    return trent_basic<TAlloc>();
                }

                throw std::runtime_error("unexpected_mnemonic "s + errloc());
            }

            template <template <class Allocator> class TAlloc = std::allocator>
            trent_basic<TAlloc> parse_numer()
            {
                char buf[32];
                char *ptr = &buf[1];

                buf[0] = onebuf;

                while (isdigit(onebuf = readnext()) || onebuf == '-' ||
                       onebuf == 'e' || onebuf == '.')
                {
                    *ptr++ = onebuf;
                }

                *ptr = 0;

                if (isspace(onebuf))
                    onebuf = 0;

                return strtod(buf, nullptr);
            }

            template <template <class Allocator> class TAlloc = std::allocator>
            trent_basic<TAlloc> parse_string()
            {
                // TRACE();
                trent::string_type str;

                char delim = onebuf;

                while ((onebuf = readnext()) != delim)
                    str += onebuf;

                onebuf = 0;
                return str;
            }

            template <template <class Allocator> class TAlloc = std::allocator>
            trent_basic<TAlloc> parse_list()
            {
                // TRACE();
                trent_basic<TAlloc> js(trent::type::list);

                int counter = 0;
                onebuf = readnext_skipping();

                if (onebuf == ']')
                {
                    onebuf = 0;
                    return js;
                }

                while (true)
                {
                    trent_basic<TAlloc> r = parse();
                    js.as_list().push_back(r);

                    if (onebuf == 0)
                        onebuf = readnext_skipping();

                    if (onebuf == ']')
                    {
                        onebuf = 0;
                        return js;
                    }

                    if (onebuf != ',')
                    {
                        throw std::runtime_error(
                            "json::parse_list: wait_, expect_"s + onebuf +
                            errloc());
                    }

                    counter++;
                    onebuf = readnext_skipping();
                }
            }

            template <template <class Allocator> class TAlloc = std::allocator>
            trent_basic<TAlloc> parse_dict()
            {
                // TRACE();
                trent_basic<TAlloc> js(trent::type::dict);

                int counter = 0;
                onebuf = readnext_skipping();

                if (onebuf == '}')
                {
                    return js;
                }

                while (true)
                {
                    // Изменить на строку
                    trent_basic<TAlloc> key = parse();

                    onebuf = readnext_skipping();

                    if (onebuf != ':')
                        throw std::runtime_error(
                            "json:parse_dict_0 wait_: expect_"s + onebuf +
                            " lineno:" + std::to_string(lineno) +
                            " symbno:" + std::to_string(symbno));

                    onebuf = 0;

                    trent_basic<TAlloc> value = parse();

                    js.as_dict().emplace(std::move(key.as_string()),
                                         std::move(value));

                    if (onebuf == 0)
                        onebuf = readnext_skipping();

                    if (onebuf == '}')
                    {
                        onebuf = 0;
                        return js;
                    }

                    if (onebuf != ',')
                    {
                        throw std::runtime_error(
                            "json:parse_dict_1 wait_, expect_"s + onebuf +
                            " lineno:" + std::to_string(lineno) +
                            " symbno:" + std::to_string(symbno));
                    }

                    counter++;
                    onebuf = readnext_skipping();

                    if (onebuf == '}')
                    {
                        onebuf = 0;
                        return js;
                    }
                }
            }
        };

        class parser_cstr : public parser
        {
            const char *ptr = nullptr;

        public:
            parser_cstr(const char *str) : ptr(str) {}
            parser_cstr(const parser_cstr &) = default;
            parser_cstr &operator=(const parser_cstr &) = default;

            char readnext_impl()
            {
                return *ptr++;
            }
        };

        class parser_str : public parser
        {
            const char *ptr = nullptr;

        public:
            parser_str(const std::string &str) : ptr(str.data()) {}
            parser_str(const parser_str &) = default;
            parser_str &operator=(const parser_str &) = default;

            char readnext_impl()
            {
                return *ptr++;
            }
        };

        class parser_input_stream : public parser
        {
            std::istream &is;

        public:
            parser_input_stream(std::istream &is) : is(is) {}

            char readnext_impl()
            {
                return is.get();
            }
        };

        nos::trent parse(const char *str);
        nos::trent parse(const std::string &str);
        nos::trent parse_file(const std::string &str);
    }
}

#endif
