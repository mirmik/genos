/**
    @file
*/

#include <comm/scpi-def.h>
#include <igris/osutil/signal.h>
#include <logging.h>
#include <nos/inet/tcp_client.h>
#include <nos/inet/tcp_server.h>
#include <nos/input.h>
#include <scpi/scpi.h>
#include <servos/AbstractServo.h>
#include <signal.h>
#include <sys/resource.h>
#include <thread>
#include <utilxx/simple_readline.h>

static std::thread scpi_thread;
static int scpi_port;
static volatile bool scpi_cancel_token = false;
extern std::string scpi_idn[];
protected_set<scpi_t *> scpi_context_set;

#define BUFSIZE 100000000UL

struct scpi_user_context
{
    nos::inet::tcp_client sock = {};
    char inbuf[BUFSIZE] = {};
    char outbuf[128] = {};
    simple_readline inrl = {};
    simple_readline outrl = {};

    void in_line_handler(const char *str, size_t sz)
    {
        nos::log::debug("Input: {}", str);
    }

    void out_line_handler(const char *str, size_t sz)
    {
        nos::log::debug("Output: {}", str);
    }

    int full_handler(simple_readline &rl)
    {
        rl.reset();
        return 0;
    }

    scpi_user_context() :
        inrl(std::string_view(inbuf, 128)), outrl(std::string_view(outbuf, 128))
    {
        outrl.set_line_handler(
            igris::make_delegate(&scpi_user_context::out_line_handler, this));
        inrl.set_line_handler(
            igris::make_delegate(&scpi_user_context::in_line_handler, this));
        outrl.set_full_handler(
            igris::make_delegate(&scpi_user_context::full_handler, this));
        inrl.set_full_handler(
            igris::make_delegate(&scpi_user_context::full_handler, this));
    }
};

size_t SCPI_Write(scpi_t *context, const char *data, size_t len)
{
    scpi_user_context *ucontext = (scpi_user_context *)context->user_context;
    ucontext->outrl.write(data, len);
    ucontext->sock.write(data, len);
    return 0;
}

scpi_result_t SCPI_Flush(scpi_t *context)
{
    (void)context;
    return SCPI_RES_OK;
}

int SCPI_Error(scpi_t *context, int_fast16_t err)
{
    (void)context;
    nos::fprintln("**ERROR {} \"{}\"", (int32_t)err, SCPI_ErrorTranslate(err));
    nos::log::warn("**ERROR {} \"{}\"", (int32_t)err, SCPI_ErrorTranslate(err));
    return 0;
}

scpi_result_t
SCPI_Control(scpi_t *context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val)
{
    (void)context;
    nos::log::warn("SCPI_Control???");
    return SCPI_RES_OK;
}

scpi_result_t SCPI_Reset(scpi_t *context)
{
    (void)context;
    nos::log::warn("SCPI_Reset???");
    return SCPI_RES_OK;
}

int scpi_listenfd;

void set_realtime_priority()
{
    int ret;

    // We'll operate on the currently running thread.
    pthread_t this_thread = pthread_self();

    // struct sched_param is used to store the scheduling priority
    struct sched_param params;

    // We'll set the priority to the maximum.
    params.sched_priority = 30; // sched_get_priority_max(SCHED_FIFO);
    nos::log::info("Trying to set thread realtime prio = {}",
                   params.sched_priority);

    // Attempt to set thread real-time priority to the SCHED_FIFO policy
    ret = pthread_setschedparam(this_thread, SCHED_FIFO, &params);
    if (ret != 0)
    {
        nos::log::info("Unsuccessful in setting thread realtime prio");
        return;
    }

    // Now verify the change in thread priority
    int policy = 0;
    ret = pthread_getschedparam(this_thread, &policy, &params);
    if (ret != 0)
    {
        nos::log::info("Couldn't retrieve real-time scheduling paramers");
        return;
    }

    // Check the correct policy was applied
    if (policy != SCHED_FIFO)
    {
        std::cout << "Scheduling is NOT SCHED_FIFO!" << std::endl;
    }
    else
    {
        std::cout << "SCHED_FIFO OK" << std::endl;
    }

    // Print thread scheduling priority
    std::cout << "Thread priority is " << params.sched_priority << std::endl;
}

void SCPI_clientHandler(nos::inet::tcp_client sock)
{
#ifndef __WIN64__
    igris::osutil::signal(SIGPIPE, SIG_IGN);
#endif

    // char *buffer = (char *)malloc(BUFSIZE);
    char *input_buffer = (char *)malloc(BUFSIZE);

    nos::log::info("open connection (fd:{})", sock.fd());
    (void)sock.nodelay(true);

    scpi_t scpi_context;

    SCPI_Init(&scpi_context,
              scpi_commands,
              &scpi_interface,
              scpi_units_def,
              scpi_idn[0].c_str(),
              scpi_idn[1].c_str(),
              scpi_idn[2].c_str(),
              scpi_idn[3].c_str(),
              input_buffer,
              SCPI_INPUT_BUFFER_LENGTH,
              scpi_error_queue_data,
              SCPI_ERROR_QUEUE_SIZE);

    scpi_context_set.insert(&scpi_context);

    scpi_user_context *ucontext = new scpi_user_context;
    ucontext->sock = sock;
    scpi_context.user_context = (void *)ucontext;

    // set_realtime_priority();

    try
    {
        while (1)
        {
            auto ret = nos::readline_from(sock, 1024, true);

            if (ret.is_error())
            {
                break;
            }
            auto &str = *ret;

            if (str.size() == 0)
                break;

            //  nos::fprintln("SCPI: {} {}", str.size(), str);
            //  auto opposite = sock.getaddr();
            // nos::fprintln("SCPI: {}:{}", opposite.addr, opposite.port);
            ucontext->inrl.write(str.c_str(), str.size());
            prevent_updates = true;
            SCPI_Input(&scpi_context, str.c_str(), str.size());
            prevent_updates = false;
        }
    }
    catch (std::exception &e)
    {
        nos::println("SCPI: exception: {}", e.what());
    }

    scpi_context_set.erase(&scpi_context);
    sock.close();

    free(input_buffer);
    delete ucontext;

    nos::log::info("close connection (fd:{})", sock.fd());
}

void SCPI_threadFunc(int scpi_port)
{
    int ret;
    nos::inet::tcp_server serv;
    nos::log::info("SCPI server started on port {}", scpi_port);

    ret = serv.init();
    if (ret < 0)
    {

        perror("scpi init fault");
        exit(-1);
    }

    ret = serv.reusing(true);
    if (ret < 0)
    {

        perror("scpi set reusing option fault");
        exit(-1);
    }

    ret = serv.bind("0.0.0.0", scpi_port);
    if (ret < 0)
    {

        perror("scpi bind fault");
        exit(-1);
    }

    serv.listen();

    if (!serv.good())
    {
        perror("scpi listen fault");
        exit(-1);
    }

    nos::log::info("SCPI server created (port:{})", scpi_port);

    while (1)
    {
        nos::inet::tcp_client client = serv.accept();
        nos::log::info("accept client");
        if (!client.good())
            continue;

        if (scpi_cancel_token)
            return;

        std::thread thr(SCPI_clientHandler, client);
        thr.detach();
    }

    serv.close();
}

void start_scpi_server(int port)
{
    scpi_port = port;
    scpi_thread = std::thread(SCPI_threadFunc, port);
}

void stop_scpi_server()
{
    scpi_cancel_token = true;
    nos::inet::tcp_client::dial("127.0.0.1", scpi_port);
    scpi_thread.join();
}

void scpi_server_join()
{
    scpi_thread.join();
}