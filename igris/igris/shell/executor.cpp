#include <igris/shell/executor.h>

__attribute__((weak)) void executor_make_process(int (*func)(int, char **),
                                                 int argc, char **argv)
{
    BUG();
}
