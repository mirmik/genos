#ifndef nos_TRENT_TRENT_H
#define nos_TRENT_TRENT_H

/**
    @file
*/

#include <cassert>
#include <cstdint>
#include <map>
#include <nos/buffer.h>
#include <nos/print.h>
#include <nos/trent/trent_path.h>
#include <nos/util/ctrdtr.h>
#include <nos/util/error.h>
#include <nos/util/expected.h>
#include <nos/util/flat_map.h>
#include <string>
#include <utility>
#include <vector>

// using namespace nos::nos::expected_type;

namespace nos
{
    enum class trent_type : uint8_t
    {
        string,
        list,
        dict,
        numer,
        boolean,
        nil,
    };
    const char *typestr(trent_type type);

    class trent_path;

    template <template <class Allocator> class TAlloc> class trent_basic
    {
    public:
        static const trent_basic &nil();

        // SPECS
        using type = trent_type;
        using value_type = std::pair<std::string, trent_basic>;

        using valloc_t = std::allocator<trent_basic>;
        using malloc_t =
            std::allocator<std::pair<const std::string, trent_basic>>;

        class wrong_path : public std::exception
        {
            trent_path path = {};
            std::string str = {};

        public:
            wrong_path(const nos::trent_path &path) : path(path)
            {
                str = std::string("trent:wrong_path: ") + path.to_string();
            }

            const char *what() const noexcept override
            {
                return str.c_str();
            }
        };

        class wrong_type : public std::exception
        {
            trent_path path = {};
            type t = {};
            std::string str = {};

        public:
            wrong_type(const trent_path &path, type t, type rt)
                : path(path), t(t)
            {
                str = std::string("trent:wrong_type:") + std::string(" path:") +
                      path.to_string() + std::string(" request:") +
                      nos::typestr(t) + std::string(" realtype:") +
                      nos::typestr(rt);
            }

            const char *what() const noexcept override
            {
                return str.c_str();
            }
        };

        class wrong_index : public std::exception
        {
            std::string str;
            trent_path path;
            type t;

        public:
            wrong_index(const trent_path &path, type t) : path(path), t(t)
            {
                str = std::string("trent:wrong_index: path: ") +
                      path.to_string() + std::string(" index: ") +
                      nos::typestr(t);
            }

            const char *what() const noexcept override
            {
                return str.c_str();
            }
        };

        using numer_type = long double;
        using integer_type = int64_t;
        using list_type = std::vector<trent_basic, valloc_t>;
        // using dict_type = std::map<std::string, trent_basic,
        // std::less<std::string>, malloc_t>;
        using dict_type = nos::flat_map<std::string, trent_basic>;
        using string_type = std::string;

    protected:
        type m_type = type::nil;
        union
        {
            bool m_bool;
            numer_type m_num;
            list_type m_arr;
            dict_type m_dct;
            string_type m_str;
        };

    public:
        const char *typestr()
        {
            return nos::typestr(m_type);
        }

        ~trent_basic();
        trent_basic() : m_type(type::nil) {}
        trent_basic(const trent_basic &other);
        trent_basic(trent_basic &&other);

        void invalidate();

        template <class T> trent_basic(const T &obj)
        {
            init(obj);
        }

    public:
        void init_sint(const int64_t &i)
        {
            m_type = type::numer;
            m_num = static_cast<numer_type>(i);
        }
        void init_uint(const uint64_t &i)
        {
            m_type = type::numer;
            m_num = i;
        }
        void init_flt(const long double &i)
        {
            m_type = type::numer;
            m_num = i;
        }
        void init_str(const char *data, size_t size);

        void init(type t);
        // void init(const trent_basic &oth);

        void init(const char *ptr)
        {
            init_str(ptr, strlen(ptr));
        }
        void init(const std::string &str)
        {
            init_str(str.data(), str.size());
        }
        void init(const nos::buffer &str)
        {
            init_str(str.data(), str.size());
        }

        void init(const int8_t &i)
        {
            init_sint(i);
        }
        void init(const int16_t &i)
        {
            init_sint(i);
        }
        void init(const int32_t &i)
        {
            init_sint(i);
        }
        void init(const int64_t &i)
        {
            init_sint(i);
        }

        void init(const uint8_t &i)
        {
            init_uint(i);
        }
        void init(const uint16_t &i)
        {
            init_uint(i);
        }
        void init(const uint32_t &i)
        {
            init_uint(i);
        }
        void init(const uint64_t &i)
        {
            init_uint(i);
        }

        void init(const float &i)
        {
            init_flt(i);
        }
        void init(const double &i)
        {
            init_flt(i);
        }
        void init(const long double &i)
        {
            init_flt(i);
        }

        void init(const bool &i)
        {
            m_type = type::boolean;
            m_bool = i;
        }

        template <typename T> void reset(const T &obj)
        {
            invalidate();
            init(obj);
        }

    public:
        trent_basic &operator[](int i)
        {
            if (m_type != type::list)
                init(type::list);
            if (m_arr.size() <= (unsigned int)i)
                m_arr.resize(i + 1);
            return m_arr[i];
        }

        trent_basic &operator[](const char *key)
        {
            if (m_type != type::dict)
                init(type::dict);
            return m_dct[std::string(key)];
        }

        trent_basic &operator[](const std::string &key)
        {
            if (m_type != type::dict)
                init(type::dict);
            return m_dct[key];
        }

        trent_basic &operator[](const nos::buffer &key)
        {
            if (m_type != type::dict)
                init(type::dict);
            return m_dct[std::string(key.data(), key.size())];
        }

        const trent_basic &operator[](const std::string &obj) const
        {
            if (m_type != type::dict || !contains(obj))
                return nil();
            return at(obj);
        }

        const trent_basic &operator[](const nos::buffer &obj) const
        {
            if (m_type != type::dict || !contains(obj))
                return nil();
            return at(obj);
        }

        const trent_basic &operator[](const char *obj) const
        {
            if (m_type != type::dict || !contains(obj))
                return nil();
            return at(obj);
        }

        const trent_basic &operator[](int obj) const
        {
            if (m_type != type::list)
                return nil();
            return at(obj);
        }

        const trent_basic &at(const trent_path &path) const
        {
            const trent_basic *tr = this;
            for (const auto &p : path)
            {
                if (p.is_string)
                {
                    tr = &tr->at(p.str);
                }
                else
                {
                    tr = &tr->at(p.i32);
                }
            }
            return *tr;
        }

        trent_basic &at(const trent_path &path)
        {
            trent_basic *tr = this;
            for (auto &p : path)
            {
                if (p.is_string)
                {
                    tr = &tr->at(p.str);
                }
                else
                {
                    tr = &tr->at(p.i32);
                }
            }
            return *tr;
        }

        trent_basic &operator[](const trent_path &path)
        {
            trent_basic *tr = this;
            for (auto &p : path)
            {
                if (p.is_string)
                {
                    tr = &((*tr)[p.str]);
                }
                else
                {
                    tr = &((*tr)[p.i32]);
                }
            }
            return *tr;
        }

        trent_basic &operator[](const trent_path &path) const
        {
            const trent_basic *tr = this;
            for (auto &p : path)
            {
                if (p.is_string)
                {
                    tr = &((*tr)[p.str]);
                }
                else
                {
                    tr = &((*tr)[p.i32]);
                }
            }
            return *tr;
        }

        bool contains(const char *key) const
        {
            if (m_type != type::dict)
                return false;
            return m_dct.find(key) != m_dct.end();
        }

        bool contains(std::string key) const
        {
            if (m_type != type::dict)
                return false;
            return m_dct.find(std::string(key)) != m_dct.end();
        }

        void init_from_list(const std::initializer_list<double> l)
        {
            init(type::list);
            m_arr.reserve(l.size());
            for (auto &i : l)
            {
                m_arr.emplace_back(i);
            }
        }

        void init_from_list(const std::initializer_list<std::string> l)
        {
            init(type::list);
            m_arr.reserve(l.size());
            for (auto &i : l)
            {
                m_arr.emplace_back(i);
            }
        }

        const trent_basic &at(int i) const
        {
            if (m_type != type::list)
                throw std::runtime_error(std::string("trent: is not a list: ") +
                                         nos::typestr(m_type));
            if (m_arr.size() <= (unsigned int)i)
                throw std::runtime_error(std::string("trent:wrong_index: ") +
                                         std::to_string(i));
            return m_arr[i];
        }

        const trent_basic &at(const std::string &key) const
        {
            if (m_type != type::dict)
                throw std::runtime_error(std::string("trent: is not a dict: ") +
                                         nos::typestr(m_type));
            return m_dct.at(key);
        }

        const trent_basic &at(const char *key) const
        {
            if (m_type != type::dict)
                throw std::runtime_error(std::string("trent: is not a dict: ") +
                                         nos::typestr(m_type));
            return m_dct.at(std::string(key));
        }

        void push_back(const trent_basic &tr)
        {
            if (m_type != type::list)
                init(type::list);
            m_arr.push_back(tr);
        }

        const trent_basic *_get(const std::string &str) const
        {
            if (is_dict())
            {
                auto it = m_dct.find(str);
                if (it == m_dct.end())
                    return nullptr;
                return &it->second;
            }

            return nullptr;
        }

        const trent_basic *_get(const char *str) const
        {
            return get(std::string(str));
        }

        const trent_basic *_get(int index) const
        {
            if (is_list())
                return &m_arr[index];

            return nullptr;
        }

        const trent_basic *get(const trent_path &path) const
        {
            const trent_basic *tr = this;
            for (const auto &p : path)
            {
                if (p.is_string)
                {
                    if (!tr->is_dict())
                        return nullptr;
                    tr = tr->_get(p.str);
                }
                else
                {
                    tr = tr->_get(p.i32);
                }

                if (tr == nullptr)
                    return nullptr;
            }
            return tr;
        }

        const trent_basic &get_except(const trent_path &path) const
        {
            const trent_basic *tr = get(path);
            if (tr == nullptr)
            {
                throw wrong_path(path);
            }
            return *tr;
        }

        numer_type get_as_numer_ex(const trent_path &path) const
        {
            const trent_basic &tr = get_except(path);

            if (tr.m_type != trent_type::numer)
            {
                throw wrong_type(path, trent_type::numer, tr.m_type);
            }

            return tr.m_num;
        }

        const string_type &get_as_string_ex(const trent_path &path) const
        {
            const trent_basic &tr = get_except(path);

            if (tr.m_type != trent_type::string)
            {
                throw wrong_type(path, trent_type::string, tr.m_type);
            }

            return tr.m_str;
        }

        bool get_as_boolean_ex(const trent_path &path) const
        {
            const trent_basic &tr = get_except(path);

            if (tr.m_type != trent_type::boolean)
            {
                throw wrong_type(path, trent_type::string, tr.m_type);
            }

            return tr.m_bool;
        }

        numer_type get_as_numer_def(const trent_path &path,
                                    numer_type def) const
        {
            const trent_basic *tr = get(path);
            if (tr == nullptr || tr->m_type != trent_type::numer)
                return def;
            return tr->m_num;
        }

        const string_type &get_as_string_def(const trent_path &path,
                                             const std::string &def) const
        {
            const trent_basic *tr = get(path);
            if (tr == nullptr || tr->m_type != trent_type::string)
                return def;
            return tr->m_str;
        }

        bool get_as_boolean_def(const trent_path &path, bool def) const
        {
            const trent_basic *tr = get(path);
            if (tr == nullptr || tr->m_type != trent_type::boolean)
                return def;
            return tr->m_bool;
        }

        string_type &as_string()
        {
            if (!is_string())
                init(type::string);
            return m_str;
        }

        const string_type &as_string() const
        {
            if (!is_string())
                throw std::runtime_error(
                    std::string("trent: is not a string: ") +
                    nos::typestr(m_type));
            return m_str;
        }

        nos::expected<string_type &, nos::errstring> as_string_critical()
        {
            if (!is_string())
                return errstring("is't string");
            return m_str;
        }

        nos::expected<const string_type &, nos::errstring>
        as_string_critical() const
        {
            if (!is_string())
                return errstring("is't string");
            return m_str;
        }

        string_type &as_string_except();
        const string_type &as_string_except() const;
        const string_type &as_string_default(const string_type &def) const
        {
            if (!is_string())
                return def;
            return m_str;
        }

        dict_type &as_dict()
        {
            if (!is_dict())
                init(type::dict);
            return m_dct;
        }
        const dict_type &as_dict() const
        {
            if (!is_dict())
                throw std::runtime_error("is't dict");
            return m_dct;
        }
        nos::expected<dict_type &, nos::errstring> as_dict_critical()
        {
            if (!is_dict())
                return errstring("is't list");
            return m_dct;
        }
        nos::expected<const dict_type &, nos::errstring>
        as_dict_critical() const
        {
            if (!is_dict())
                return errstring("is't list");
            return m_dct;
        }
        dict_type &as_dict_except()
        {
            if (!is_dict())
                throw std::runtime_error("is't list");
            return m_dct;
        }
        const dict_type &as_dict_except() const
        {
            if (!is_dict())
                throw std::runtime_error("is't list");
            return m_dct;
        }

        list_type &as_list()
        {
            if (!is_list())
                init(type::list);
            return m_arr;
        }

        const list_type &as_list() const
        {
            if (!is_list())
                throw std::runtime_error("is't list");
            return m_arr;
        }

        nos::expected<list_type &, nos::errstring> as_list_critical()
        {
            if (!is_list())
                return errstring("is't list");
            return m_arr;
        }
        nos::expected<const list_type &, nos::errstring>
        as_list_critical() const
        {
            if (!is_list())
                return errstring("is't list");
            return m_arr;
        }
        list_type &as_list_except()
        {
            if (!is_list())
                throw std::runtime_error("is't list");
            return m_arr;
        }
        const list_type &as_list_except() const
        {
            if (!is_list())
                throw std::runtime_error("is't list");
            return m_arr;
        }

        numer_type as_numer() const
        {
            if (is_bool())
                return (int)m_bool;
            if (is_string())
                return std::stod(m_str);
            if (!is_numer())
                throw std::runtime_error("is't numer");
            return m_num;
        }

        integer_type as_integer() const
        {
            if (is_bool())
                return (int)m_bool;
            if (!is_numer())
                throw std::runtime_error("is't numer");
            return m_num;
        }

        nos::expected<numer_type, nos::errstring> as_numer_critical() const
        {
            if (is_bool())
                return (int)m_bool;

            if (!is_numer())
                return errstring("is't numer");
            return m_num;
        }
        numer_type as_numer_except() const
        {
            if (is_bool())
                return (int)m_bool;
            if (!is_numer())
                throw std::runtime_error("is't numer");
            return m_num;
        }
        numer_type as_numer_default(numer_type def) const
        {
            if (is_bool())
                return (int)m_bool;
            if (!is_numer())
                return def;
            return m_num;
        }

        int64_t as_integer_default(int64_t def) const
        {
            return as_numer_default(def);
        }

        // TODO добавить проверку на целое.
        nos::expected<int64_t, nos::errstring> as_integer_critical() const
        {
            if (!is_numer())
                return errstring("is't numer");
            return m_num;
        }
        int64_t as_integer_except() const
        {
            if (!is_numer())
                throw std::runtime_error("is't numer");
            return m_num;
        }

        bool as_bool() const
        {
            return m_bool;
        }
        bool as_bool_default(bool def) const
        {
            if (!is_bool())
                return def;
            return m_bool;
        }

        // TODO добавить проверку на целое.
        nos::expected<bool, nos::errstring> as_bool_critical() const
        {
            if (!is_bool())
                return errstring("is't bool");
            return m_bool;
        }

        bool as_bool_except() const
        {
            if (!is_bool())
                throw std::runtime_error("is't bool");
            return m_bool;
        }

        const nos::buffer as_buffer() const;

        // integer_type& unsafe_integer_const() { return m_int; }
        numer_type &unsafe_numer_const()
        {
            return m_num;
        }
        string_type &unsafe_string_const()
        {
            return m_str;
        }
        list_type &unsafe_list_const()
        {
            return m_arr;
        }
        dict_type &unsafe_dict_const()
        {
            return m_dct;
        }

        //          const integer_type& unsafe_integer_const() const { return
        // m_int;
        //}
        const numer_type &unsafe_numer_const() const
        {
            return m_num;
        }
        const string_type &unsafe_string_const() const
        {
            return m_str;
        }
        const list_type &unsafe_list_const() const
        {
            return m_arr;
        }
        const dict_type &unsafe_dict_const() const
        {
            return m_dct;
        }
        const bool &unsafe_bool_const() const
        {
            return m_bool;
        }

        trent_basic::type get_type() const
        {
            return m_type;
        }

        bool is_nil() const
        {
            return m_type == type::nil;
        }
        bool is_bool() const
        {
            return m_type == type::boolean;
        }
        bool is_numer() const
        {
            return m_type == type::numer;
        }
        bool is_list() const
        {
            return m_type == type::list;
        }
        bool is_dict() const
        {
            return m_type == type::dict;
        }
        bool is_string() const
        {
            return m_type == type::string;
        }

        template <class O>
        nos::expected<size_t, nos::output_error> print_to(O &os) const;

    public:
        trent_basic &operator=(const trent_basic &other)
        {
            invalidate();
            m_type = other.m_type;
            switch (m_type)
            {
            case type::string:
                nos::constructor(&m_str, other.m_str);
                return *this;
            case type::list:
                nos::constructor(&m_arr, other.m_arr);
                return *this;
            case type::dict:
                nos::constructor(&m_dct, other.m_dct);
                return *this;
            case type::numer:
                m_num = other.m_num;
                return *this;
            case type::boolean:
                m_bool = other.m_bool;
                return *this;
            case type::nil:
                return *this;
            }
            return *this;
        }

        trent_basic &operator=(trent_basic &&other)
        {
            invalidate();
            m_type = other.m_type;
            switch (m_type)
            {
            case type::string:
                nos::move_constructor(&m_str, std::move(other.m_str));
                return *this;
            case type::list:
                nos::move_constructor(&m_arr, std::move(other.m_arr));
                return *this;
            case type::dict:
                nos::move_constructor(&m_dct, std::move(other.m_dct));
                return *this;
            case type::numer:
                m_num = other.m_num;
                return *this;
            case type::boolean:
                m_num = other.m_num;
                return *this;
            case type::nil:
                return *this;
            }

            other.reset(type::nil);
            return *this;
        }

        template <class T> trent_basic &operator=(const T &arg)
        {
            reset(arg);
            return *this;
        }

        template <class T> T get() const
        {
            using DT = typename std::decay<T>::type;

            if constexpr (std::is_same_v<DT, trent_basic>)
            {
                return *this;
            }
            else if constexpr (std::is_same_v<DT, std::string>)
            {
                if (is_string())
                    return as_string();
                else
                    return std::to_string(as_numer_except());
            }
            else if constexpr (std::is_same_v<DT, bool>)
            {
                return as_bool();
            }
            else
            {
                if (is_numer())
                    return static_cast<T>(as_numer());
                else
                    return static_cast<T>(std::stod(as_string()));
            }
        }
    };

    using trent = trent_basic<std::allocator>;

    template <template <class Allocator> class TAlloc>
    trent_basic<TAlloc>::~trent_basic()
    {
        invalidate();
    }

    template <template <class Allocator> class TAlloc>
    void trent_basic<TAlloc>::invalidate()
    {
        switch (m_type)
        {
        case type::string:
            nos::destructor(&m_str);
            return;

        case type::list:
            nos::destructor(&m_arr);
            return;

        case type::dict:
            nos::destructor(&m_dct);
            return;

        case type::nil:
        case type::numer:
        case type::boolean:
            return;
        }

        m_type = type::nil;
    }

    template <template <class Allocator> class TAlloc>
    void trent_basic<TAlloc>::init_str(const char *data, size_t size)
    {
        m_type = trent_basic::type::string;
        nos::constructor(&m_str, data, size);
    }

    template <template <class Allocator> class TAlloc>
    template <class O>
    nos::expected<size_t, nos::output_error>
    trent_basic<TAlloc>::print_to(O &os) const
    {
        bool sep = false;

        switch (get_type())
        {
        case type::boolean:
            nos::print_to(os, unsafe_bool_const() ? "true" : "false");
            return 0;

        case type::numer:
            nos::print_to(os, unsafe_numer_const());
            return 0;

        case type::string:
            os.putbyte('"');
            nos::print_to(os, unsafe_string_const());
            os.putbyte('"');
            return 0;

        case type::list:
            os.putbyte('[');

            for (auto &v : unsafe_list_const())
            {
                if (sep)
                    os.putbyte(',');

                v.print_to(os);
                sep = true;
            }

            os.putbyte(']');
            return 0;

        case type::dict:
            os.putbyte('{');

            for (auto &p : unsafe_dict_const())
            {
                if (sep)
                    os.putbyte(',');

                os.putbyte('"');
                nos::print_to(os, p.first);
                os.putbyte('"');
                os.putbyte(':');
                p.second.print_to(os);
                sep = true;
            }

            os.putbyte('}');
            return 0;

        case type::nil:
            nos::print_to(os, "nil");
            return 0;
        }
        return 0;
    }

    template <template <class Allocator> class TAlloc>
    void trent_basic<TAlloc>::init(trent_basic::type t)
    {
        if (m_type != trent_basic::type::nil)
            invalidate();

        m_type = t;

        switch (m_type)
        {
        case trent_basic::type::string:
            nos::constructor(&m_str);
            return;

        case trent_basic::type::list:
            nos::constructor(&m_arr);
            return;

        case trent_basic::type::dict:
            nos::constructor(&m_dct);
            return;

        case trent_basic::type::boolean:
        case trent_basic::type::numer:
        case trent_basic::type::nil:
            return;
        }
    }

    template <template <class Allocator> class TAlloc>
    trent_basic<TAlloc>::trent_basic(const trent_basic<TAlloc> &other)
    {
        m_type = other.m_type;

        switch (m_type)
        {
        case trent_basic::type::string:
            nos::constructor(&m_str, other.m_str);
            return;

        case trent_basic::type::list:
            nos::constructor(&m_arr, other.m_arr);
            return;

        case trent_basic::type::dict:
            nos::constructor(&m_dct, other.m_dct);
            return;

        case trent_basic::type::numer:
            m_num = other.m_num;
            return;

        case trent_basic::type::boolean:
            m_bool = other.m_bool;
            return;

        case trent_basic::type::nil:
            return;
        }
    }

    template <template <class Allocator> class TAlloc>
    trent_basic<TAlloc>::trent_basic(trent_basic<TAlloc> &&other)
    {
        m_type = other.m_type;

        switch (m_type)
        {
        case trent_basic::type::string:
            nos::move_constructor(&m_str, std::move(other.m_str));
            return;

        case trent_basic::type::list:
            nos::move_constructor(&m_arr, std::move(other.m_arr));
            return;

        case trent_basic::type::dict:
            nos::move_constructor(&m_dct, std::move(other.m_dct));
            return;

        case trent_basic::type::numer:
            m_num = other.m_num;
            return;

        case trent_basic::type::boolean:
            m_bool = other.m_bool;
            return;

        case trent_basic::type::nil:
            return;
        }

        other.invalidate();
    }

    template <template <class Allocator> class TAlloc>
    std::string &trent_basic<TAlloc>::as_string_except()
    {
        if (!is_string())
            throw std::runtime_error("isn't string");

        return m_str;
    }

    template <template <class Allocator> class TAlloc>
    const std::string &trent_basic<TAlloc>::as_string_except() const
    {
        if (!is_string())
            throw std::runtime_error("isn't string");

        return m_str;
    }

    template <template <class Allocator> class TAlloc>
    const trent_basic<TAlloc> &trent_basic<TAlloc>::nil()
    {
        static const trent_basic _nil;
        return _nil;
    }

    /*template <template <class Allocator> class TAlloc>
    void trent_basic<TAlloc>::init(const trent_basic<TAlloc> &other)
    {
        if (m_type != trent_basic::type::nil)
            invalidate();

        m_type = other.m_type;

        switch (m_type)
        {
        case trent_basic::type::string:
            nos::constructor(&m_str, other.m_str);
            return;

        case trent_basic::type::list:
            nos::constructor(&m_arr, other.m_arr);
            return;

        case trent_basic::type::dict:
            nos::constructor(&m_dct, other.m_dct);
            return;

        case trent_basic::type::numer:
            m_num = other.m_num;
            return;

        case trent_basic::type::boolean:
            m_bool = other.m_bool;
            return;

        case trent_basic::type::nil:
            return;
        }
    }*/
}

#endif
