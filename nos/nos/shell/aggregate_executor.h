#ifndef NOS_AGGREGATE_EXECUTOR_H
#define NOS_AGGREGATE_EXECUTOR_H

#include <nos/shell/executor.h>

namespace nos
{
    template <class... AddArgs>
    class aggregate_executor_t : public nos::executor_basic_t<AddArgs...>
    {
        std::vector<nos::executor_basic_t<AddArgs...> *> executors;

    public:
        aggregate_executor_t(
            const std::vector<nos::executor_basic_t<AddArgs...> *> &executors)
            : executors(executors)
        {
        }

        void print_help_to(nos::ostream &os)
        {
            for (auto *ex : executors)
            {
                ex->print_help_to(os);
            }
        }

        nos::command_t<AddArgs...> *find(const nos::buffer &name) override
        {
            nos::command_t<AddArgs...> *ptr = nullptr;
            for (auto it = executors.begin(); it != executors.end() && !ptr;
                 ++it)
            {
                ptr = (*it)->find(name);
            }

            return ptr;
        }
    };

    using aggregate_executor = aggregate_executor_t<>;
}

#endif