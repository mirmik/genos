#include <cstring>
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
    return system_utilities.invoke(cmd, argc, (char **)argv);
}

void genos::system_utility_aggregator::add_utility(const char *cmd,
                                                   system_utility_func_t util)
{
    utilities.push_back({cmd, util});
}

int genos::system_utility_aggregator::invoke(const char *cmd,
                                             int argc,
                                             char **argv)
{
    for (auto &util : utilities)
    {
        if (strcmp(util.name(), cmd) == 0)
        {
            return util.invoke(argc, argv);
        }
    }
    return -1;
}

void genos::register_system_utility(const char *cmd, system_utility_func_t util)
{
    system_utilities.add_utility(cmd, util);
}

int __system_v_second_half(void *arg)
{
    auto *argv = (std::vector<const char *> *)arg;
    genos::current_schedee()->set_mnemo("sechalf");
    genos::command_process_v(argv->data());
    for (const char *str : *argv)
        free((void *)str);
    delete argv;
    return 0;
}

int genos::system(const char *cmd)
{
    auto vec = igris::split(cmd);
    std::vector<const char *> cvec;
    for (const auto &str : vec)
        cvec.push_back(str.c_str());
    cvec.push_back(nullptr);
    return genos::system_v(cvec.data());
}

int genos::system_v(const char **cmds)
{
    std::vector<const char *> *cvec = new std::vector<const char *>;
    for (const char **sstr = cmds; *sstr != NULL; ++sstr)
        cvec->push_back(strdup(*sstr));
    cvec->push_back(nullptr);
    int pid = genos::clone(__system_v_second_half,
                           (void *)cvec,
                           NULL,
                           GENOS_DEFAULT_HEAPSTACK_SIZE);
    genos::waitpid(pid);
    return 0;
}

int genos::system_v_without_displace(const char **cmds)
{
    std::vector<const char *> *cvec = new std::vector<const char *>;
    for (const char **sstr = cmds; *sstr != NULL; ++sstr)
        cvec->push_back(strdup(*sstr));
    cvec->push_back(nullptr);
    int pid = genos::clone(__system_v_second_half,
                           (void *)cvec,
                           NULL,
                           GENOS_DEFAULT_HEAPSTACK_SIZE);
    genos::waitpid_without_displace(pid);
    return 0;
}