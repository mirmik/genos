#ifndef NOS_WEAKED_FUNCTIONS_H
#define NOS_WEAKED_FUNCTIONS_H

#include <concepts>
#include <functional>
#include <nos/trent/json.h>
#include <nos/trent/json_print.h>
#include <nos/trent/trent.h>
#include <nos/util/arglist.h>
#include <stdexcept>
#include <string>

namespace nos
{
    /// @brief Класс именованного аргумента.
    /// Используется вместе с классом cf_abstract, для запуска
    /// weaked_function Непосредственно через trent. Для применения с
    /// механизмами rpc.
    class trent_argument
    {
        std::string name = {};
        nos::trent value = {};

    public:
        trent_argument(const nos::trent &value, const std::string &name = "")
            : name(name), value(value)
        {
        }

        const std::string &get_name() const
        {
            return name;
        }

        const nos::trent &get_value() const
        {
            return value;
        }
    };

    /// @brief Класс для внутреннего использования в weaked_function.
    /// Создаёт временное хранилище для аргументов, в trent виде.
    /// Являясь промежуточным хранилищем, упорядочивает конверсию аргументов,
    /// заменяя N*M на 2*N+M необходимых методов конверсий.
    struct runtime_argument
    {
        bool inited;
        nos::trent value;
    };

    namespace detail
    {
        template <class F> class signature
        {
        };

        template <class R, class... Args> struct signature<R(Args...)>
        {
            constexpr static size_t count = sizeof...(Args);
            using result_type = R;

            template <size_t I>
            using nth_argtype =
                typename std::tuple_element_t<I, std::tuple<Args...>>;
        };
    }

    template <class, template <class...> class>
    inline constexpr bool is_specialization = false;
    template <template <class...> class T, class... Args>
    inline constexpr bool is_specialization<T<Args...>, T> = true;

    /// @brief Является ли тип инстансом nos::argpair?
    template <class T> concept ArgPair = is_specialization<T, nos::argpair>;

    class weaked_function_basic
    {
    public:
        virtual nos::expected<nos::trent, nos::errstring>
        call_with_args(const std::vector<nos::trent_argument> &args) = 0;
        virtual ~weaked_function_basic() = default;
    };

    /// @brief Обёртка для std::function, которая позволяет вызывать функцию,
    /// используя аргументы в trent виде. Позволяет использовать функции с
    /// именованными аргументами. При прямом использовании осуществляет
    /// конверсию аргументов в trent, в соответствии с типами аргументов
    template <class F> class weaked_function : public weaked_function_basic
    {
        std::function<F> func;
        constexpr static size_t count = detail::signature<F>::count;
        std::array<std::string, count> argument_names;

    public:
        weaked_function(auto &&f,
                        const std::array<std::string, count> &names = {})
            : func(f), argument_names(names){};

        ~weaked_function() override = default;

        template <class... Args2> auto operator()(Args2 &&... args) const
        {
            std::tuple<Args2...> args_tuple{(args)...};
            std::array<runtime_argument, count> rarguments = {};

            // parse each argument
            parse_arguments(rarguments,
                            args_tuple,
                            std::make_index_sequence<sizeof...(Args2)>{});

            return call(rarguments, std::make_index_sequence<count>{});
        }

        nos::expected<nos::trent, nos::errstring>
        call_with_args(const std::vector<nos::trent_argument> &args)
        {
            std::array<runtime_argument, count> rarguments = {};
            parse_arguments(rarguments, args);

            if constexpr (std::is_same_v<
                              void,
                              typename detail::signature<F>::result_type>)
            {
                auto ret = call(rarguments, std::make_index_sequence<count>{});
                if (ret.is_error())
                    return ret.error();
                return nos::trent();
            }
            else
            {
                auto ret = call(rarguments, std::make_index_sequence<count>{});
                if (ret.is_error())
                    return ret.error();
                return static_cast<nos::trent>(ret.value());
            }
        }

        template <size_t... I>
        nos::expected<typename nos::detail::signature<F>::result_type,
                      nos::errstring>
        call(const std::array<runtime_argument, count> &arr,
             std::index_sequence<I...>) const
        {
            for (size_t i = 0; i < count; i++)
            {
                if (!arr[i].inited)
                {
                    return nos::errstring("Argument " + argument_names[i] +
                                          " is not inited");
                }
            }

            if constexpr (std::is_same_v<
                              void,
                              typename detail::signature<F>::result_type>)
            {
                func(trent_to_type<typename nos::detail::signature<
                         F>::template nth_argtype<I>>(arr[I].value)...);
                return {};
            }
            else
            {
                auto ret =
                    func(trent_to_type<typename nos::detail::signature<
                             F>::template nth_argtype<I>>(arr[I].value)...);
                return static_cast<
                    typename nos::detail::signature<F>::result_type>(ret);
            }
        }

        template <class T> static T trent_to_type(const nos::trent &t)
        {
            return t.get<T>();
        }

        template <class... Args2, size_t... I>
        void parse_arguments(std::array<runtime_argument, count> &rarguments,
                             const std::tuple<Args2...> &args_tuple,
                             std::index_sequence<I...>) const
        {
            (parse_argument(I, rarguments, std::get<I>(args_tuple)), ...);
        }

        template <ArgPair Arg>
        void parse_argument(size_t,
                            std::array<runtime_argument, count> &rarguments,
                            const Arg &arg) const
        {
            int index2 = -1;
            for (int i = 0; i < (int)count; i++)
            {
                if (argument_names[i] == arg.name)
                {
                    index2 = i;
                    break;
                }
            }

            if (index2 == -1)
            {
                throw std::runtime_error("Argument is not found");
            }

            rarguments[index2].inited = true;
            rarguments[index2].value = arg.value();
        }

        template <typename Arg>
        void parse_argument(size_t index,
                            std::array<runtime_argument, count> &rarguments,
                            const Arg &arg) const
        {
            rarguments[index].inited = true;
            rarguments[index].value = arg;
        }

        void parse_arguments(std::array<runtime_argument, count> &rarguments,
                             const std::vector<nos::trent_argument> &args)
        {
            size_t len = std::min(args.size(), count);
            for (size_t index = 0; index < len; index++)
            {
                if (args[index].get_name().empty())
                {
                    rarguments[index].inited = true;
                    rarguments[index].value = args[index].get_value();
                }
                else
                {
                    bool found = false;
                    for (size_t index2 = 0; index2 < count; index2++)
                    {
                        if (argument_names[index2] == args[index].get_name())
                        {
                            rarguments[index2].inited = true;
                            rarguments[index2].value = args[index].get_value();
                            found = true;
                            break;
                        }
                    }

                    if (found == false)
                    {
                        throw std::runtime_error("Argument is not found");
                    }
                }
            }
        }
    };

    template <class R, class... Args>
    std::unique_ptr<weaked_function_basic>
    make_wf_unique(std::function<R(Args...)> f,
                   std::array<std::string, sizeof...(Args)> names = {})
    {
        return std::make_unique<weaked_function<R(Args...)>>(f, names);
    }

    class wf_collection
    {
        std::map<std::string, std::unique_ptr<weaked_function_basic>>
            collection = {};

    public:
        wf_collection() = default;
        wf_collection(const wf_collection &) = delete;
        wf_collection(wf_collection &&) = delete;

        template <class R, class... Args>
        void add(const std::string &name,
                 std::function<R(Args...)> f,
                 const std::array<std::string, sizeof...(Args)> &names = {})
        {
            collection.emplace(name, make_wf_unique(f, names));
        }

        nos::expected<nos::trent, nos::errstring>
        execute_json(const std::string &json)
        {
            auto trent = nos::json::parse(json);
            return execute_trent(trent);
        }

        nos::expected<nos::trent, nos::errstring>
        execute_trent(const nos::trent &trent)
        {
            std::vector<nos::trent_argument> targs;
            auto name = trent["cmd"].as_string();

            if (trent.contains("args") && trent["args"].is_list())
            {
                auto &args = trent["args"].as_list();
                for (auto &arg : args)
                {
                    targs.emplace_back(arg);
                }
            }

            if (trent.contains("kwargs") && trent["kwargs"].is_dict())
            {
                auto &kwargs = trent["kwargs"].as_dict();
                for (auto &arg : kwargs)
                {
                    targs.emplace_back(arg.second, arg.first);
                }
            }
            return execute(name, targs);
        }

        nos::expected<nos::trent, nos::errstring>
        execute(const std::string &name,
                const std::vector<nos::trent_argument> &args)
        {
            auto it = collection.find(name);
            if (it == collection.end())
                return nos::errstring("Undefined function: " + name);
            auto ret = it->second->call_with_args(args);
            if (ret.is_error())
                return ret.error();
            else
                return ret.value();
        }
    };
}

#endif