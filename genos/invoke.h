#ifndef GENOS_INVOKE_H
#define GENOS_INVOKE_H

#include <string_view>

#define GENOS_DEFAULT_HEAPSTACK_SIZE 2048

namespace genos
{
    using system_utility_func_t = int (*)(int, char **);

    class system_utility
    {
        std::string _name = {};
        system_utility_func_t _func = nullptr;

    public:
        system_utility(const std::string &name, system_utility_func_t func)
            : _name(name), _func(func)
        {
        }

        const std::string &name() const
        {
            return _name;
        }

        int invoke(int argc, char **argv) const
        {
            return _func(argc, argv);
        }
    };

    class system_utility_aggregator
    {
        std::vector<system_utility> _utilities = {};

    public:
        system_utility_aggregator() = default;
        std::vector<system_utility> &utilities();
        void add_utility(const char *name, system_utility_func_t util);
        int invoke(const char *cmd, int argc, char **argv);
    };

    int invoke_program(const char *cmd, int argc, const char **argv);
    int system(const char *cmd);
    int system_v(const char **cmds);
    int system_v_without_displace(const char **argv);
    void register_system_utility(const char *cmd, system_utility_func_t util);
}

#endif