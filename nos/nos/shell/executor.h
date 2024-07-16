#ifndef NOS_EXECUTOR_H
#define NOS_EXECUTOR_H

#include <algorithm>
#include <nos/print.h>
#include <nos/shell/argv.h>
#include <nos/shell/command.h>

namespace nos
{
    template <class... AddArgs> class executor_basic_t
    {
        nos::buffer undefined_error_message = "Undefined command:";

    public:
        virtual void print_help_to(nos::ostream &os) = 0;
        virtual nos::command_t<AddArgs...> *find(const nos::buffer &name) = 0;
        virtual ~executor_basic_t() = default;

        int execute(const char *cmd, nos::ostream &os)
        {
            nos::tokens tokens(cmd);
            return execute(tokens, os);
        }

        void print_undefined(nos::ostream &os, const nos::buffer &name)
        {
            nos::println_to(os, undefined_error_message, name);
        }

        int execute(const nos::argv &argv, nos::ostream &os, AddArgs... addargs)
        {
            if (argv.size() == 0)
                return 0;

            auto cmd = find(argv[0]);

            if (cmd)
            {
                cmd->execute(argv, os, addargs...);
                return 0;
            }
            return -1;
        }
    };

    template <class... AddArgs>
    class executor_t : public executor_basic_t<AddArgs...>
    {
        std::vector<nos::command_t<AddArgs...>> commands = {};

    public:
        executor_t() = default;

        executor_t(const std::vector<nos::command_t<AddArgs...>> &vec)
            : commands(vec)
        {
        }

        void add_command(const nos::command_t<AddArgs...> &cmd)
        {
            commands.push_back(cmd);
        }

        void print_help_to(nos::ostream &os)
        {
            for (auto &cmd : commands)
            {
                nos::println_to(os, cmd.name(), "-", cmd.help());
            }
        }

        nos::command_t<AddArgs...> *find(const nos::buffer &name) override
        {
            auto it = std::find_if(commands.begin(),
                                   commands.end(),
                                   [&](const auto &cmd)
                                   { return name == cmd.name(); });

            if (it == commands.end())
            {
                return nullptr;
            }

            return &*it;
        }
    };

    using executor = executor_t<>;
}

#endif