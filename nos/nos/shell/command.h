#ifndef NOS_SHELL_COMMAND_H
#define NOS_SHELL_COMMAND_H

#include <nos/io/ostream.h>
#include <nos/util/delegate.h>

namespace nos
{
    class argv;

    template <class... AddArgs> class command_t
    {
        nos::buffer _name;
        nos::buffer _help;
        nos::delegate<int, const nos::argv &, nos::ostream &, AddArgs...> _func;

    public:
        command_t(
            nos::buffer name,
            nos::buffer help,
            nos::delegate<int, const nos::argv &, nos::ostream &, AddArgs...>
                func)
            : _name(name), _help(help), _func(func)
        {
        }

        const nos::buffer &name() const
        {
            return _name;
        }
        const nos::buffer &help() const
        {
            return _help;
        }

        int execute(const nos::argv &argv,
                    nos::ostream &os,
                    AddArgs... addargs) const
        {
            return _func(argv, os, addargs...);
        }
    };

    using command = command_t<>;
}

#endif