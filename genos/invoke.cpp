#include <cstring>
#include <genos/fork.h>
#include <genos/invoke.h>
#include <genos/schedee_api.h>
#include <igris/time/systime.h>
#include <igris/util/bug.h>
#include <igris/util/string.h>
#include <unistd.h>

int genos::invoke_program(const char *cmd, int argc, const char **argv)
{
    write(STDOUT_FILENO, "HelloWorld\r\n", 12);

    /*    nos::println("invoke_program");
        nos::println("cmd:", cmd);
        nos::println("argc:", argc);

        for (int i = 0; i < argc; i++)
            nos::println("argv[", i, "]:", argv[i]);

        if (strcmp("testproc", cmd) == 0)
        {
            nos::println("testproc");

            genos::current_schedee_msleep(3000);
            nos::println("I am testproc process");

            return 0;
        }

        nos::println("Not found");*/
    return -1;
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