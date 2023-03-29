#include <asm/irq.h>
#include <genos/coop_schedee.h>
#include <genos/fork.h>
#include <genos/invoke.h>
#include <genos/schedee.h>
#include <genos/schedee_api.h>
#include <igris/osinter/ctrobj.h>
#include <igris/util/string.h>

#define GENOS_DEFAULT_HEAPSTACK_SIZE 1024

int genos::clone(int (*fn)(void *), void *arg, void *stack, size_t stack_size)
{
    auto *sch = genos::current_schedee();
    auto *ncsch = genos::create_coop_schedee(fn, arg, stack, stack_size);
    ncsch->set_priority(sch->priority());
    ncsch->start();
    return ncsch->pid;
}

struct environment_vars
{
    const char *cmd;
    std::vector<const char *> args;
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

int genos::command_process_v(const char **argv)
{
    auto *env = new environment_vars;
    for (const char **sstr = argv; *sstr != NULL; ++sstr)
    {
        env->args.push_back(strdup(*sstr));
    }
    env->cmd = env->args[0];
    auto *sch = genos::current_schedee();
    genos::coop_schedee *csch = (genos::coop_schedee *)sch;
    csch->reset_context(__execute_starter, env);
    csch->execute();

    // unreachable code
    return -1;
}

int genos::execute(const std::vector<std::string> &vec)
{
    std::vector<const char *> cvec;
    for (const auto &str : vec)
        cvec.push_back(str.c_str());
    cvec.push_back(nullptr);
    return genos::command_process_v(cvec.data());
}

int genos::execute(const char *cmd)
{
    auto splited = igris::split(cmd);
    return genos::execute(splited);
}

int __system_v_second_half(void *arg)
{
    genos::current_schedee()->set_mnemo("sechalf");
    auto *argv = (std::vector<const char *> *)arg;
    genos::command_process_v(argv->data());
    for (const char *str : *argv)
        free((void *)str);
    delete argv;
    return 0;
}

/*void genos::start_command_process_v(const char **argv, size_t stacksize)
{
    genos::clone(__start_command_process_v, (void *)argv, NULL, stacksize);
}*/

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

int genos::waitpid(intptr_t pid)
{
    genos::schedee *sch;
    sch = genos::current_schedee();

    sch->sch_state = genos::schedee_state::wait_schedee;

    system_lock();
    dlist_del_init(&sch->ctr.lnk);
    system_unlock();

    ctrobj_set_future(&sch->ctr, pid);
    genos::current_schedee_displace();

    return 0;
}

int genos::waitpid_without_displace(intptr_t pid)
{
    genos::schedee *sch;
    sch = genos::current_schedee();

    sch->sch_state = genos::schedee_state::wait_schedee;

    system_lock();
    dlist_del_init(&sch->ctr.lnk);
    system_unlock();

    ctrobj_set_future(&sch->ctr, pid);

    return 0;
}