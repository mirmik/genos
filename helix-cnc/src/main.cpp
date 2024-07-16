#include <api2.h>
#include <asm/irq.h>
#include <control.h>
#include <crow-service.h>
#include <crow/tower.h>
#include <executor.h>
#include <genos/autom_schedee.h>
#include <genos/coop_schedee.h>
#include <genos/fork.h>
#include <genos/invoke.h>
#include <genos/ktimer.h>
#include <genos/schedee.h>
#include <genos/schedee_api.h>
#include <igris/datastruct/dlist.h>
#include <igris/time/systime.h>
#include <igris/util/cpu_delay.h>
#include <main.h>
#include <nos/io/null_ostream.h>
#include <nos_serial_ostream.h>
#include <ralgo/disctime.h>
#include <serial.h>
#include <servo.h>
#include <trigger/eventctr.h>
#include <vector>
#include <zillot/common/uart.h>
#include <zillot/common/uartring.h>
#include <zillot/irq.h>
#include <zillot/irqtable/irqtable.h>

static_assert(sizeof(float64_t) == 8);

#define PROCESS_STACK_SIZE 3072

extern int isrtime(int argc, char **argv);
int set_control_mode(int argc, char **argv);
int crow_diagnostic(int argc, char **argv);

void board_initialization();
void configure();
void start_blink_schedee();
void init_debug_console();

void use_script()
{
    system_executor.execute("cmd setprotect 0\r\n", *nos::default_ostream());
}

int testproc(int argc, char **argv)
{
    genos::current_schedee()->set_mnemo("child");
    for (int i = 0; i < 10; ++i)
    {
        nos::println("I am child process");
        genos::current_schedee_msleep(1000);
        genos::current_schedee_displace();
    }
    return 0;
}

int mainthread(void *)
{
    genos::register_system_utility("testproc", testproc);
    return 0;
}

int main()
{
    genos::schedee_manager_init();

    board_initialization();
    serial_interfaces_init();
    irqs_enable();

    nos::println();
    nos::println();
    nos::println();
    nos::println("Board initialization success");

    nos::println("Init operation system tables");

    nos::println("Start blink schedee");
    start_blink_schedee();

    configure();

    init_crow_part();
    nos::println("Start Crow module ... OK");

    // К этому моменту дефолтный логгер должен быть установлен
    ralgo::set_logger(nos::log::default_logger());

    init_cnc();
    nos::println("Start CNC module ... OK");

    irqs_enable();

    init_top_level();
    init_bottom_level();

    nos::println("Start operation system");
    init_debug_console();

    // use_script();

    auto *mainproc = genos::create_process(mainthread, nullptr, nullptr, 1024);
    mainproc->set_mnemo("main");
    mainproc->set_priority(1);
    mainproc->start();

    frequency_test_start();
    igris::delay(1000);
    frequency_test_stop();

    while (1)
    {
        __schedule__();
    }
}

void __schedule__()
{
    while (1)
    {
        crow::onestep();
        genos::ktimer_manager_step();
        genos::schedee_manager_step();
    }
}
