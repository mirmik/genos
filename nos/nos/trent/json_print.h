#ifndef NOS_JSON_PRINT_H
#define NOS_JSON_PRINT_H

#include <iostream>
#include <nos/io/std_adapter.h>
#include <nos/io/test_width_ostream.h>
#include <nos/trent/json.h>
#include <sstream>
#include <string>

namespace nos
{
    namespace json
    {
        template <template <class Allocator> class TAlloc = std::allocator>
        void print_to(const trent_basic<TAlloc> &tr,
                      std::ostream &os,
                      bool pretty,
                      int tab)
        {
            bool sep = false;
            bool havedict;

            switch (tr.get_type())
            {
            case trent::type::numer:
                os << tr.unsafe_numer_const();
                break;

            case trent::type::boolean:
                os << (tr.unsafe_bool_const() ? "true" : "false");
                break;

            case trent::type::string:
                os.put('"');
                os << tr.unsafe_string_const();
                os.put('"');
                break;

            case trent::type::list:
                havedict = false;

                for (const auto &m : tr.unsafe_list_const())
                {
                    if (m.get_type() == trent::type::dict)
                    {
                        havedict = true;
                        break;
                    }
                }

                os.put('[');

                if (havedict)
                    for (auto &v : tr.unsafe_list_const())
                    {
                        if (sep)
                        {
                            if (pretty)
                                os << ", ";
                            else
                                os.put(',');
                        }

                        json::print_to(v, os, pretty, tab + 1);
                        sep = true;
                    }
                else
                {
                    for (auto &v : tr.unsafe_list_const())
                    {
                        if (sep)
                            os.put(',');

                        if (pretty)
                        {
                            os << "\r\n";
                            for (int i = 0; i < tab + 1; i++)
                                os.put('\t');
                        }

                        if (pretty)
                        {
                            nos::test_width_ostream testwidth;
                            nos::std_ostream_adapter adapter =
                                nos::make_std_adapter(testwidth);
                            json::print_to(v, adapter, false, 0);
                            if (testwidth.result > 20)
                                json::print_to(v, os, true, tab + 1);
                            else
                                json::print_to(v, os, false, tab + 1);
                        }
                        else
                            json::print_to(v, os, pretty, tab + 1);

                        sep = true;
                    }

                    if (pretty)
                    {
                        os << "\r\n";

                        for (int i = 0; i < tab; i++)
                            os.put('\t');
                    }
                }

                os.put(']');
                break;

            case trent::type::dict:
                os.put('{');

                for (auto &p : tr.unsafe_dict_const())
                {
                    if (sep)
                        os.put(',');

                    if (pretty)
                    {
                        os.put('\n');

                        for (int i = 0; i < tab + 1; i++)
                            os.put('\t');
                    }

                    os.put('"');
                    os << p.first;
                    os.put('"');
                    if (pretty)
                        os.write(": ", 2);
                    else
                        os.put(':');

                    if (pretty)
                    {
                        nos::test_width_ostream testwidth;
                        nos::std_ostream_adapter adapter =
                            nos::make_std_adapter(testwidth);
                        json::print_to(p.second, adapter, false, 0);
                        if (testwidth.result > 20)
                            json::print_to(p.second, os, true, tab + 1);
                        else
                            json::print_to(p.second, os, false, tab + 1);
                    }
                    else
                        json::print_to(p.second, os, pretty, tab + 1);
                    sep = true;
                }

                if (pretty)
                {
                    os.put('\n');
                    for (int i = 0; i < tab; i++)
                        os.put('\t');
                }

                os.put('}');
                break;

            case trent::type::nil:
                os.write("nil", 3);
                break;
            }

            if (pretty)
                if (tab == 0)
                    os << "\r\n";
        }

        template <template <class Allocator> class TAlloc = std::allocator>
        std::string to_string(const nos::trent_basic<TAlloc> &tr,
                              bool pretty = false)
        {
            std::stringstream ss;
            print_to(tr, ss, pretty, 0);
            return ss.str();
        }

        template <template <class Allocator> class TAlloc = std::allocator>
        std::string to_pretty_string(const nos::trent_basic<TAlloc> &tr)
        {
            std::stringstream ss;
            print_to(tr, ss, true, 0);
            return ss.str();
        }

        template <template <class Allocator> class TAlloc = std::allocator>
        void pretty_print_to(const trent_basic<TAlloc> &tr, std::ostream &os)
        {
            print_to(tr, os, true, 0);
        }
    }
}

#endif
