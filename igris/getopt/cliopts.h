#ifndef IGRIS_CLIOPTS_H
#define IGRIS_CLIOPTS_H

#include <igris/result.h>
#include <igris/util/ctrdtr.h>
#include <string>
#include <vector>

// Переписать по аналогии с питоном. Пусть трент возвращает.

namespace igris
{
    class cliopts
    {
        using error = igris::result_type::error;
        template <class T> using result = igris::result_type::result<T, error>;

        enum class Type
        {
            Bool,
            Integer,
            String,
            Option
        };

        struct opt
        {
            std::string long_name;
            char short_name;
            Type type;

            union
            {
                bool b = false;
                int32_t i32;
                std::string str;
            };

        public:
            opt(const char *l, char s, bool def)
                : long_name(l), short_name(s), type(Type::Bool), b(def)
            {
            }
            opt(const char *l, char s, int def)
                : long_name(l), short_name(s), type(Type::Integer), i32(def)
            {
            }
            opt(const char *l, char s, std::string def)
                : long_name(l), short_name(s), type(Type::String), str(def)
            {
            }
            opt(const char *l, char s)
                : long_name(l), short_name(s), type(Type::Option), b(false)
            {
            }

            opt(const opt &other)
                : long_name(other.long_name), short_name(other.short_name),
                  type(other.type)
            {
                switch (type)
                {
                case Type::String:
                    igris::constructor(&str, other.str);
                    break;
                case Type::Integer:
                    i32 = other.i32;
                    break;
                case Type::Bool:
                case Type::Option:
                    b = other.b;
                }
            }

            ~opt()
            {
                if (type == Type::String)
                    igris::destructor(&str);
            }
        };

    public:
        std::vector<opt> opts = {};
        std::vector<std::string> args = {};

        void add_bool(const char *l, char s, bool def)
        {
            opts.emplace_back(l, s, def);
        }
        void add_integer(const char *l, char s, int32_t def)
        {
            opts.emplace_back(l, s, def);
        }
        void add_string(const char *l, char s, std::string def)
        {
            opts.emplace_back(l, s, def);
        }
        void add_string(const char *l, char s, const char *def)
        {
            opts.emplace_back(l, s, std::string(def));
        }
        void add_option(const char *l, char s)
        {
            opts.emplace_back(l, s);
        }

        result<opt *> get_opt(const std::string &name)
        {
            std::string message;

            for (auto &o : opts)
            {
                if (o.long_name == name)
                {
                    return &o;
                }
            }

            message = "wrong opt name " + name;
            return error(message);
        }

        result<opt *> get_opt(char c)
        {
            std::string message;

            for (auto &o : opts)
            {
                if (c == o.short_name)
                {
                    return &o;
                }
            }

            message = "wrong opt short_name " + std::string(&c, 1);
            return error(message);
        }

        result<opt *> get_opt(const std::string &name, Type type)
        {
            std::string message;

            for (auto &o : opts)
            {
                if (o.long_name == name)
                {
                    if (o.type != type)
                        return error("wrong opt type");
                    return &o;
                }
            }

            message = "wrong opt name " + name;
            return error(message);
        }

        result<std::string> get_string(const std::string &name)
        {
            auto &&r = get_opt(name, Type::String);
            if (r.is_error())
                return r.error();
            return r.value()->str;
        }

        result<int32_t> get_integer(const std::string &name)
        {
            auto &&r = get_opt(name, Type::Integer);
            if (r.is_error())
                return r.error();
            return r.value()->i32;
        }

        result<bool> get_bool(const std::string &name)
        {
            auto &&r = get_opt(name, Type::Option);
            if (r.is_error())
                return r.error();
            return r.value()->b;
        }

        result<bool> get_option(const std::string &name)
        {
            auto &&r = get_opt(name, Type::Option);
            if (r.is_error())
                return r.error();
            return r.value()->b;
        }

        std::vector<std::string> get_args()
        {
            return args;
        }
        const std::vector<std::string> &arguments()
        {
            return args;
        }

        enum class AutomState
        {
            WaitValue,
            Normal
        };

        result<void> set_value(opt &o, const char *val)
        {
            if (*val == 0)
                return error("wrong option syntax");
            switch (o.type)
            {
            case Type::String:
                o.str = std::string(val);
                break;
            case Type::Integer:
                o.i32 = atoi(val);
                break;
            default:
                BUG();
            }
            return result<void>();
        }

        static int parse_minus(const char *str, AutomState state)
        {
            if (*str != '-')
                return 0;
            if (state == AutomState::WaitValue)
                return -1;
            if (*(str + 1) == 0)
                return -1;
            if (*(str + 1) == '-')
            {
                if (*(str + 2) == 0)
                    return -1;
                return 2;
            }
            return 1;
        }

        result<opt *> parse_long_opt(const char *l, AutomState &state)
        {
            char buf[64];
            char *ptr = buf;
            while (*l != '=' && *l != 0)
            {
                *ptr++ = *l++;
            }
            *ptr = 0;

            auto &&r = get_opt(buf);
            if (r.is_error())
                return r.error();

            opt *o = r.value();

            if (*l == '=')
            {
                if (o->type == Type::Option || o->type == Type::Bool)
                    return error("wrong option syntax");
                set_value(*o, l + 1);
                return o;
            }
            else
            {
                if (o->type != Type::Option && o->type != Type::Bool)
                    state = AutomState::WaitValue;
                if (o->type == Type::Bool || o->type == Type::Option)
                    o->b = true;
                return o;
            }
        }

        result<void> parse(int argc, char *argv[])
        {
            int i;

            auto state = AutomState::Normal;
            opt *curopt = nullptr;
            // result<opt&> res;

            for (auto it = argv; argc--; it++)
            {
                switch (parse_minus(*it, state))
                {
                case 0:
                    // dprln(0);
                    if (state == AutomState::Normal)
                    {
                        args.push_back(std::string(*it));
                    }
                    else
                    {
                        auto &&r = set_value(*curopt, *it);
                        if (r.is_error())
                            return r.error();

                        state = AutomState::Normal;
                    }
                    continue;
                case 1:
                    i = 1;
                    while (*(*it + i) != 0)
                    {
                        auto &&r = get_opt(*(*it + i));
                        if (r.is_error())
                            return std::move(r.error());

                        curopt = r.value();
                        if (curopt->type != Type::Option &&
                            curopt->type != Type::Bool)
                        {
                            if (*(*it + i + 1) == 0)
                            {
                                state = AutomState::WaitValue;
                                break;
                            }
                            auto &&r = set_value(*curopt, *it + i + 1);
                            if (r.is_error())
                                return std::move(r.error());
                            break;
                        }
                        else
                        {
                            curopt->b = true;
                        }
                        i++;
                    }
                    break;
                case 2:
                {
                    auto &&r = parse_long_opt(*it + 2, state);
                    if (r.is_error())
                        return std::move(r.error());

                    curopt = r.value();
                    break;
                }
                case -1:
                    return error("wrong option syntax");
                }
            }
            if (state == AutomState::WaitValue)
                return error("wrong option syntax");
            return result<void>();
        }

        result<void> parse(int strt, int argc, char *argv[])
        {
            return parse(argc - strt, argv + strt);
        }
    };
}

#endif
