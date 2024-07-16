#ifndef WEAKED_FUNCTION_INTERPRETER_H
#define WEAKED_FUNCTION_INTERPRETER_H

#include <nos/shell/argv.h>
#include <nos/shell/weaked_function.h>

namespace nos
{
    class weaked_function_interpreter
    {
        nos::wf_collection functions = {};

    public:
        weaked_function_interpreter() = default;

        template <class R, class... Args>
        void add(const std::string &name,
                 std::function<R(Args...)> f,
                 const std::array<std::string, sizeof...(Args)> &argnames = {})
        {
            functions.add(name, f, argnames);
        }

        /// Выполняет комманды типа
        /// cmd ARG1 ARG2
        nos::expected<nos::trent, nos::errstring>
        execute_console_command_protocol(std::string str)
        {
            auto tokens = nos::tokens(str);
            auto argv = nos::argv(tokens);
            std::string name = argv[0];
            auto argw = argv.without(1);

            std::vector<nos::trent_argument> targs;
            auto it = argw.begin();
            auto eit = argw.end();
            for (; it != eit; ++it)
            {
                if ((*it)[0] == '-' && (*it)[1] == '-')
                {
                    auto key = (*it).substr(2);
                    ++it;
                    if (it == eit)
                        break;
                    targs.emplace_back(*it, key);
                    continue;
                }

                targs.emplace_back(*it);
            }

            return functions.execute(argv[0], targs);
        }

        nos::expected<nos::trent, nos::errstring>
        execute_json_protocol(nos::trent tr,
                              std::string cmdfield = "cmd",
                              std::string argsfield = "args",
                              std::string kwargsfield = "kwargs")
        {
            if (!tr.contains(cmdfield) || !tr[cmdfield].is_string())
                return nos::trent();

            std::string name = tr[cmdfield].as_string();
            std::vector<nos::trent_argument> targs;

            if (tr.contains(argsfield) && tr[argsfield].is_list())
                for (auto &arg : tr[argsfield].as_list())
                {
                    targs.emplace_back(arg);
                }

            if (tr.contains(kwargsfield) && tr[kwargsfield].is_dict())
                for (auto &[key, value] : tr[kwargsfield].as_dict())
                {
                    targs.emplace_back(value, key);
                }

            return functions.execute(name, targs);
        }
    };
}

#endif