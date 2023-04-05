#include <cstring>
#include <genos/coop_schedee.h>
#include <genos/fork.h>
#include <genos/invoke.h>
#include <genos/schedee_api.h>
#include <igris/time/systime.h>
#include <igris/util/bug.h>
#include <igris/util/string.h>
#include <nos/print.h>
#include <unistd.h>

__attribute__((init_priority(101)))
genos::system_utility_aggregator system_utilities;

int genos::invoke_program(const char *cmd, int argc, const char **argv)
{
    dprln("invoke_program:", cmd);
    dprln("size:", system_utilities.utilities().size());
    for (auto &util : system_utilities.utilities())
    {
        dprln("util name:", util.name());
    }
    return system_utilities.invoke(cmd, argc, (char **)argv);
}

void genos::system_utility_aggregator::add_utility(const char *cmd,
                                                   system_utility_func_t util)
{
    _utilities.push_back({cmd, util});
}

int genos::system_utility_aggregator::invoke(const char *cmd,
                                             int argc,
                                             char **argv)
{
    for (auto &util : _utilities)
    {
        if (strcmp(util.name().c_str(), cmd) == 0)
        {
            return util.invoke(argc, argv);
        }
    }
    return -1;
}

std::vector<genos::system_utility> &
genos::system_utility_aggregator::utilities()
{
    return _utilities;
}

void genos::register_system_utility(const char *cmd, system_utility_func_t util)
{
    system_utilities.add_utility(cmd, util);
}

struct environment_vars
{
    const char *cmd = nullptr;
    std::vector<const char *> args = {};
};

int __execute_starter(void *arg)
{
    auto *env = (environment_vars *)arg;
    genos::invoke_program(env->cmd, env->args.size(), env->args.data());
    for (const char *str : env->args)
        free((void *)str);
    delete env;
    return 0;
}

int __system_v_second_half(void *arg)
{
    dprln("sechalf id:", genos::current_schedee()->pid);

    auto *env = (environment_vars *)arg;
    auto *sch = genos::current_schedee();
    genos::coop_schedee *csch = (genos::coop_schedee *)sch;

    csch->set_mnemo("sechalf");
    csch->reset_context(__execute_starter, env);
    csch->execute();

    // unreachable code
    return 0;
}

int genos::system(const char *cmd)
{
    environment_vars *cvec = new environment_vars;
    auto splitted = igris::split(cmd, ' ');

    for (auto &str : splitted)
        cvec->args.push_back(strdup(str.c_str()));
    cvec->args.push_back(nullptr);
    cvec->cmd = cvec->args[0];

    int pid = genos::clone(__system_v_second_half,
                           (void *)cvec,
                           NULL,
                           GENOS_DEFAULT_HEAPSTACK_SIZE);
    genos::waitpid(pid);
    return 0;
}

int genos::system_v(const char **cmds)
{
    abort();
    /*environment_vars *cvec = new environment_vars;

    for (const char **sstr = cmds; *sstr != NULL; ++sstr)
        cvec->args.push_back(strdup(*sstr));
    cvec->args.push_back(nullptr);
    cvec->cmd = cvec->args[0];

    int pid = genos::clone(__system_v_second_half,
                           (void *)cvec,
                           NULL,
                           GENOS_DEFAULT_HEAPSTACK_SIZE);
    genos::waitpid(pid);*/
    return 0;
}

int genos::system_v_without_displace(const char **argv)
{
    environment_vars *cvec = new environment_vars;

    for (const char **sstr = argv; *sstr != NULL; ++sstr)
        cvec->args.push_back(strdup(*sstr));
    cvec->args.push_back(nullptr);
    cvec->cmd = cvec->args[0];

    int pid = genos::clone(__system_v_second_half,
                           (void *)cvec,
                           NULL,
                           GENOS_DEFAULT_HEAPSTACK_SIZE);
    genos::waitpid_without_displace(pid);
    return 0;
}