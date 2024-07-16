#ifndef NOS_TRENT_YAML_H
#define NOS_TRENT_YAML_H

namespace nos
{
    namespace yaml
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

                /*if (c == '/')
                {
                    c = readnext();

                    switch (c)
                    {
                    case '*':
                        while (true)
                        {
                            c = readnext();
                            if (c == '*')
                            {
                                c = readnext();
                                if (c == '/')
                                    goto __try__;
                            }
                        }
                        break;
                    case '/':
                        while (true)
                        {
                            c = readnext();
                            if (c == '\n')
                                goto __try__;
                        }
                        break;
                    default:
                        throw unexpected_symbol(c);
                    }
                }*/

                return c;
            }

            trent parse()
            {
                int whitespace_prefix = 0;
                return parse_object_started_on_newline(
                    whitespace_prefix_length);
            }

            trent
            parse_object_started_on_newline(int previous_whitespace_prefix)
            {
                int ws = get_whitespace_prefix_and_skip_it();

                char c = readnext_skipping();
                if (c == '-')
                {
                    // it's an array
                    trent arr = parse_array_multiline(ws);
                    return arr;
                }

                if (isalpha(c))
                {
                    // it's an object
                    trent obj = parse_object_multiline(ws);
                    return obj;
                }
            }
        };

        trent parse_array_multiline_from_first_line(int whitespace_prefix)
        {
            trent obj;
            while (true)
            {
                auto o = parse(whitespace_prefix);
                obj.push_back(o);
                auto ws = get_whitespace_prefix_and_skip_it();
                if (ws <= whitespace_prefix)
                    break;
            }
            return obj;
        }

        trent parse_object_multiline_from_first_line(int whitespace_prefix)
        {
            trent obj;
            while (true)
            {
                std::string key = parse_key();
                char c = readnext_skipping();
                if (c != ':')
                    throw unexpected_symbol(c);

                trent value = parse(whitespace_prefix);
                obj[key] = value;
            }
            return obj;
        }

        class string_parser : public parser
        {
            const char *it;
            const char *eit;

        public:
            string_parser(const char *it, const char *eit) : it(it), eit(eit) {}

            char readnext_impl()
            {
                if (it == eit)
                    return 0;
                return *it++;
            }
        };

        class file_parser : public parser
        {
            FILE *f;

        public:
            file_parser(FILE *f) : f(f) {}

            char readnext_impl()
            {
                if (onebuf)
                {
                    char c = onebuf;
                    onebuf = 0;
                    return c;
                }
                return fgetc(f);
            }

            void unread(char c)
            {
                onebuf = c;
            }
        };
    }
}

#endif