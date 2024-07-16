#include <board_defs.h>
#include <executor.h>
#include <fcntl.h>
#include <genos/autom_schedee.h>
#include <genos/fork.h>
#include <genos/invoke.h>
#include <genos/schedee_api.h>
#include <igris/shell/vtermxx.h>
#include <nos/io/sstream.h>
#include <nos/print.h>
#include <nos/shell/executor.h>
#include <unistd.h>
#include <zillot/common/uartring.h>

igris::vtermxx vterm;

void execute_callback(const char *str, size_t len)
{
    nos::stringstream output;
    auto tokens = nos::tokens(str);
    int code = system_executor.execute(tokens, output);
    if (code < 0)
        nos::println("Undefined command");
    write(STDOUT_FILENO, output.str().data(), output.str().size());
}

void read_console(void *_priv, int *state)
{
    const char *path = (const char *)_priv;

    switch (*state)
    {
    case 0:
        open(path, O_RDONLY);
        open(path, O_WRONLY);

        vterm.init(96, 5);
        vterm.set_prompt("$ ");
        vterm.set_write_callback(+[](const char *str, size_t len)
                                 { write(STDOUT_FILENO, str, len); });
        vterm.set_execute_callback(execute_callback);
        vterm.set_signal_callback(+[](int sig) { nos::println("Signal"); });
        vterm.init_step();

        *state = 1;
        break;

    case 1:
        vterm.newdata(VTERMXX_INIT_STEP);
        genos::wait_for_avail(STDIN_FILENO);
        *state = 2;
        break;
    case 2:
        char buf[48];
        int len = read(STDIN_FILENO, buf, sizeof(buf));
        for (int i = 0; i < len; ++i)
            vterm.newdata(buf[i]);
        *state = 1;
        break;
    }
}

void init_debug_console()
{
    auto *proc =
        genos::create_autom_schedee(read_console, (void *)"/dev/ttyS0");
    proc->set_mnemo("debug_console");
    proc->set_priority(0);
    proc->start();
}
