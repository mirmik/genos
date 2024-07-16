#ifndef TRENT_PATH_H
#define TRENT_PATH_H

/**
    @file
*/

#include <iostream>
#include <nos/util/ctrdtr.h>
#include <nos/util/string.h>
#include <vector>

namespace nos
{
    class trent_path_node
    {
    public:
        bool is_string = false;
        union
        {
            std::string str;
            int32_t i32;
        };

        trent_path_node() = delete;

        trent_path_node(const std::string &str_)
        {
            if (isdigit(str_[0]))
            {
                is_string = false;
                nos::constructor(&this->i32, std::stoi(str_));
            }
            else
            {
                is_string = true;
                nos::constructor(&this->str, str_);
            }
        }

        trent_path_node(const trent_path_node &oth)
        {
            if (oth.is_string == true)
            {
                is_string = true;
                nos::constructor(&str, oth.str);
            }
            else
            {
                is_string = false;
                nos::constructor(&i32, oth.i32);
            }
        }

        ~trent_path_node()
        {
            if (is_string)
                nos::destructor(&str);
        }

        nos::trent_path_node &operator=(const nos::trent_path_node &oth)
        {
            this->is_string = oth.is_string;
            if (this->is_string)
                this->str = oth.str;
            else
                this->i32 = oth.i32;

            return *this;
        }
    };

    class trent_path : public std::vector<trent_path_node>
    {
    public:
        trent_path() {}
        trent_path(const std::string &path) : trent_path(path.c_str()) {}

        trent_path(const char *path)
        {
            auto svec = nos::split(path, '/');

            for (const auto &s : svec)
            {
                std::vector<trent_path_node>::emplace_back(s);
            }
        }

        std::string to_string() const
        {
            std::vector<std::string> svec;
            svec.resize(size());

            for (unsigned int i = 0; i < size(); ++i)
            {
                auto &cur = operator[](i);
                if (cur.is_string)
                {
                    svec[i] = cur.str;
                }
                else
                {
                    svec[i] = std::to_string(cur.i32);
                }
            }

            return nos::join(svec, '/');
        }
    };
}

#endif
