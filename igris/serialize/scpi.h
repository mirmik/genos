#ifndef igris_SERIALIZE_SCPI_H
#define igris_SERIALIZE_SCPI_H

/**
@file
*/

#include <cctype>
#include <string>
#include <vector>

#include <igris/deprecated/creader.h>

namespace igris
{
    class scpi_string_parser
    {
    public:
        struct header
        {
            std::string str;
            int num;
            header(const std::string &str, int num) : str(str), num(num) {}
        };

        std::vector<header> headers;
        std::vector<std::string> arguments;
        bool is_question = false;
        bool is_error = false;

    public:
        scpi_string_parser(const std::string &str)
        {
            igris::creader reader(str.c_str());

            while (reader.next_is<int(int)>(std::isalpha))
            {
                std::string tempstr = reader.string_while<int(int)>(isalpha);
                int tempnum =
                    reader.next_is<int(int)>(isdigit) ? reader.integer() : -1;
                headers.emplace_back(tempstr, tempnum);
                if (!reader.next_is(':'))
                    break;
                else
                    reader.skip();
            }

            reader.skip_while(" ,\n");
            while (!reader.next_is(igris::buffer("\0?", 2)))
            {
                if (reader.next_is('\"'))
                {
                    reader.skip();
                    arguments.emplace_back(
                        reader.string_while(igris::chars_set_checker(
                            igris::buffer("\"\0", 2), false)));
                    if (reader.next_is('\0'))
                        return;
                    else
                        reader.skip();
                }
                else
                {
                    arguments.emplace_back(
                        reader.string_while(igris::chars_set_checker(
                            igris::buffer(" ,\0\n?", 4), false)));
                }
                reader.skip_while(" ,\n");
            }
            if (reader.next_is('?'))
                is_question = true;
        }
    };
}

#endif
