#include <control.h>
#include <crow-service.h>
#include <crow/addons/logger.h>
#include <crow/address.h>
#include <crow/gates/self_driven_gstuff.h>
#include <crow/hexer.h>
#include <crow/nodes/nospublisher.h>
#include <crow/nodes/publisher_node.h>
#include <crow/nodes/service_node.h>
#include <crow/proto/node.h>
#include <crow/tower.h>
#include <fcntl.h>
#include <genos/autom_schedee.h>
#include <genos/schedee_api.h>
#include <igris/container/ring.h>
#include <igris/defs/io.h>
#include <igris/shell/readlinexx.h>
#include <igris/shell/rshell_executor.h>
#include <igris/shell/vtermxx.h>
#include <nos/fprint.h>
#include <nos/io/sstream.h>
#include <nos/io/string_writer.h>
#include <ralgo/cnc/themes.h>
#include <ralgo/log.h>
#include <serial.h>
#include <unistd.h>
#include <zillot/common/uartring.h>
#include <sizes.h>

bool crow_services_inited = false;

class publish_logger : public nos::log::logger
{
public:
    // crow::publisher_node *pubnode;

public:
    publish_logger() = default;

    void log(nos::log::level lvl,
             const std::string_view &fmt,
             const nos::visitable_arglist &arglist) override
    {
    }

    using nos::log::logger::log;
};

publish_logger logger;
igris::vtermxx vterm_2;

igris::ring<std::string> execution_ring(EXECUTION_RING_SIZE);

void execute_callback_2(const char *str, size_t len)
{
    execution_ring.emplace(str, len);
    // nos::stringstream output;
    // auto tokens = nos::tokens(str);
    // int code = system_executor.execute(tokens, output);
    // if (code < 0)
    //     nos::println("Undefined command");
    // write(STDOUT_FILENO, output.str().data(), output.str().size());
}

void late_execution(void *_priv, int *state)
{
    if (execution_ring.empty())
        return;

    if (execution_ring.avail() > 10)
        abort();

    std::string str = execution_ring.tail();
    execution_ring.pop();

    nos::stringstream output;
    auto tokens = nos::tokens(str.c_str());
    int code = system_executor.execute(tokens, output);
    uring_rs232.write(output.str().data(), output.str().size());
}

void read_console_2(void *_priv, int *state)
{
    const char *path = (const char *)_priv;

    switch (*state)
    {
    case 0:
        open(path, O_RDONLY);
        open(path, O_WRONLY);

        vterm_2.init(96, 5);
        vterm_2.set_prompt("");
        vterm_2.set_echo(false);
        vterm_2.set_write_callback(+[](const char *str, size_t len) {
            write(STDOUT_FILENO, str, len);
        });
        vterm_2.set_execute_callback(execute_callback_2);
        vterm_2.set_signal_callback(+[](int sig) { nos::println("Signal"); });
        vterm_2.init_step();

        *state = 1;
        break;

    case 1:
        vterm_2.newdata(VTERMXX_INIT_STEP);
        genos::wait_for_avail(STDIN_FILENO);
        *state = 2;
        break;

    case 2:
        char buf[48];
        int len = read(STDIN_FILENO, buf, sizeof(buf));
        for (int i = 0; i < len; ++i)
        {
            vterm_2.newdata(buf[i]);
        }
        *state = 1;
        break;
    }
}

void init_debug_console_2()
{
    auto *proc =
        genos::create_autom_schedee(read_console_2, (void *)"/dev/ttyS1");
    proc->set_mnemo("terminal");
    proc->set_priority(0);
    proc->start();

    auto *execproc = genos::create_autom_schedee(late_execution, nullptr);
    execproc->set_mnemo("executor");
    execproc->set_priority(1);
    execproc->start();
}

void init_crow_part()
{
    init_debug_console_2();

    nos::log::set_default_logger(&logger);
    crow_services_inited = true;
}

void send_final_status_handle()
{
    uring_rs232.write("stop\r\n", 6);
}

void send_start_status_handle()
{
    uring_rs232.write("start\r\n", 7);
}

void telemetry_publisher_publish(const int64_t *poses, int axes_count)
{
    uring_rs232.write("pulse\r\n", 7);
}

void feedback_telemetry_publisher_publish(const int64_t *feedback_poses,
                                          int axes_count)
{
}