/**
    @file
*/

#include <comm/ncpi.h>
#include <comm/scpi-def.h>
#include <csignal>
#include <nos/inet/tcp_client.h>
#include <nos/inet/tcp_server.h>
#include <nos/input.h>
#include <thread>

static volatile bool ncpi_cancel_token = false;
static std::thread ncpi_thread;
static int ncpi_port;
extern std::string scpi_idn[];
#define NCPI_MAX_COMMAND_NUMBERS 5

int ncpi_listenfd = -1;

//Вариант структуры данных для контекста scpi системы ncpi
struct ncpi_user_context
{
    NotifierNCPI ncl = {};
    nos::inet::tcp_client sock = {};
};

size_t NCPI_Write(scpi_t *context, const char *data, size_t len)
{
    ncpi_user_context *ucontext = (ncpi_user_context *)context->user_context;
    ucontext->sock.write(data, len);
    return 0;
}

scpi_result_t NCPI_Flush(scpi_t *context)
{
    ncpi_user_context *ucontext = (ncpi_user_context *)context->user_context;
    (void)ucontext;
    return SCPI_RES_OK;
}

int NCPI_Error(scpi_t *context, int_fast16_t err)
{
    (void)context;
    nos::log::warn("**ERROR {} \"{}\"", (int32_t)err, SCPI_ErrorTranslate(err));

    return 0;
}

scpi_result_t
NCPI_Control(scpi_t *context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val)
{
    (void)context;
    nos::log::warn("SCPI_Control???");
    return SCPI_RES_OK;
}

scpi_result_t NCPI_Reset(scpi_t *context)
{
    (void)context;
    nos::log::warn("SCPI_Reset???");
    return SCPI_RES_OK;
}

void set_realtime_priority();

void NCPI_clientHandler(nos::inet::tcp_client sock)
{
    char input_buffer[1024];

#ifndef __WIN64__
    std::signal(SIGPIPE, SIG_IGN);
#endif
    nos::log::info("open connection (fd:{})", sock.fd());
    sock.nodelay(true);

    scpi_t scpi_context;

    SCPI_Init(&scpi_context,
              ncpi_commands,
              &ncpi_interface,
              scpi_units_def,
              scpi_idn[0].c_str(),
              scpi_idn[1].c_str(),
              scpi_idn[2].c_str(),
              scpi_idn[3].c_str(),
              input_buffer,
              SCPI_INPUT_BUFFER_LENGTH,
              scpi_error_queue_data,
              SCPI_ERROR_QUEUE_SIZE);

    ncpi_user_context *ucontext = new ncpi_user_context;
    ucontext->sock = sock;
    ucontext->ncl.sock = sock;

    scpi_context.user_context = (void *)ucontext;

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

            nos::fprintln("NCPI: {} {}", str.size(), str);
            auto opposite = sock.getaddr();
            nos::fprintln("NCPI: {}:{}", opposite.addr, opposite.port);
            SCPI_Input(&scpi_context, str.c_str(), str.size());
        }
    }
    catch (std::exception &e)
    {
        nos::log::error("NCPI: exception: {}", e.what());
    }

    ucontext->ncl.removeAllThemes();
    sock.close();

    delete ucontext;

    nos::log::info("close ncpi connection (fd:{})", sock.fd());
}

void NCPI_notifyThreadFunc(int ncpi_port)
{
    nos::log::trace("NCPI listener: thread start (file:{})", __FILE__);
    //	char buffer[512];

    nos::inet::tcp_server serv;
    serv.init();
    serv.reusing(true);
    serv.bind("0.0.0.0", ncpi_port);
    serv.listen();
    nos::log::info("NCPI server started on port {}", ncpi_port);

    while (1)
    {
        nos::inet::tcp_client client = serv.accept();
        if (!client.good())
            continue;
        if (ncpi_cancel_token)
            break;
        std::thread thr(NCPI_clientHandler, client);
        thr.detach();
    }
    serv.close();
}

scpi_result_t NCPI_setNotification_without(scpi_t *context)
{
    int32_t arg = -1;
    ncpi_user_context *ucontext = (ncpi_user_context *)context->user_context;

    int tag = SCPI_CmdTag(context);

    if (!SCPI_ParamInt32(context, &arg, true))
        return SCPI_RES_ERR;
    if (arg != 0 && arg != 1)
    {
        SCPI_ErrorPush(context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);
        return SCPI_RES_ERR;
    }

    NotifyTheme *theme = brocker.themeFind(tag, {});
    if (theme == nullptr)
    {
        SCPI_ErrorPush(context, SCPI_ERROR_HEADER_SUFFIX_OUTOFRANGE);
        return SCPI_RES_ERR;
    };

    switch (arg)
    {
    case 1:
        theme->registryClient(&ucontext->ncl);
        break;
    case 0:
        theme->removeClient(&ucontext->ncl);
        break;
    };

    return SCPI_RES_OK;
}

scpi_result_t NCPI_setNotification(scpi_t *context)
{
    int32_t arg = -1;
    ncpi_user_context *ucontext = (ncpi_user_context *)context->user_context;

    std::vector<int32_t> numbers;
    int32_t __numbers[NCPI_MAX_COMMAND_NUMBERS];

    for (int i = 0; i < NCPI_MAX_COMMAND_NUMBERS; i++)
    {
        __numbers[i] = -1;
    }

    SCPI_CommandNumbers(context, __numbers, NCPI_MAX_COMMAND_NUMBERS, -1);

    for (int i = 0; i < NCPI_MAX_COMMAND_NUMBERS; i++)
    {
        if (__numbers[i] == -1)
            break;
        numbers.push_back(__numbers[i]);
    }

    int tag = SCPI_CmdTag(context);

    if (!SCPI_ParamInt32(context, &arg, true))
        return SCPI_RES_ERR;
    if (arg != 0 && arg != 1)
    {
        SCPI_ErrorPush(context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);
        return SCPI_RES_ERR;
    }

    NotifyTheme *theme = brocker.themeFind(tag, numbers);
    if (theme == nullptr)
    {
        SCPI_ErrorPush(context, SCPI_ERROR_HEADER_SUFFIX_OUTOFRANGE);
        return SCPI_RES_ERR;
    };

    switch (arg)
    {
    case 1:
        theme->registryClient(&ucontext->ncl);
        break;
    case 0:
        theme->removeClient(&ucontext->ncl);
        break;
    };

    return SCPI_RES_OK;
}

scpi_result_t NCPI_setSmoothNotification(scpi_t *context)
{
    //	int32_t arg0 = -1;
    int32_t arg1 = -1;
    float arg2 = 0;

    ncpi_user_context *ucontext = (ncpi_user_context *)context->user_context;

    std::vector<int32_t> numbers;
    int32_t __numbers[NCPI_MAX_COMMAND_NUMBERS];

    const scpi_choice_def_t options[] = {{"OFF", -1000},
                                         {"SMOOTH", SmoothType::SMOOTH},
                                         {"TIMERED", SmoothType::TIMERED},
                                         SCPI_CHOICE_LIST_END};

    //Считывание номеров идентификаторов.
    for (int i = 0; i < NCPI_MAX_COMMAND_NUMBERS; i++)
        __numbers[i] = -1;
    SCPI_CommandNumbers(context, __numbers, NCPI_MAX_COMMAND_NUMBERS, -1);
    for (int i = 0; i < NCPI_MAX_COMMAND_NUMBERS; i++)
    {
        if (__numbers[i] == -1)
            break;
        numbers.push_back(__numbers[i]);
    }

    //Получение тэга паттерна команды.
    int tag = SCPI_CmdTag(context);

    //Получение параметров плавной подписки.

    if (!SCPI_ParamChoice(context, options, &arg1, false))
        return SCPI_RES_ERR;

    if (arg1 != -1000)
        if (!SCPI_ParamFloat(context, &arg2, false))
            return SCPI_RES_ERR;

    //Поиск темы производится по тэгк паттерна и номерам идентификаторов.
    NotifyTheme *theme = brocker.themeFind(tag, numbers);
    if (theme == nullptr)
    {
        SCPI_ErrorPush(context, SCPI_ERROR_HEADER_SUFFIX_OUTOFRANGE);
        return SCPI_RES_ERR;
    };

    NotifySmoothTheme *__theme = reinterpret_cast<NotifySmoothTheme *>(theme);

    //Подписаться на тему, используя полученные параметры или отписаться от
    //темы.
    if (arg1 == -1000)
        __theme->removeClient(&ucontext->ncl);
    else
        __theme->registryClient(&ucontext->ncl, (SmoothType)arg1, arg2);

    return SCPI_RES_OK;
}

/**	Запрос о наличии подписки клиента на тему.
    @out:
        1 при подписке.
        0 при отсутствии подписки.
*/
scpi_result_t NCPI_notificationQ(scpi_t *context)
{
    // int32_t arg = -1;
    ncpi_user_context *ucontext = (ncpi_user_context *)context->user_context;
    std::vector<int32_t> numbers;
    int32_t __numbers[NCPI_MAX_COMMAND_NUMBERS];

    //Считывание номеров в идентификаторах.
    for (int i = 0; i < NCPI_MAX_COMMAND_NUMBERS; i++)
        __numbers[i] = -1;
    SCPI_CommandNumbers(context, __numbers, NCPI_MAX_COMMAND_NUMBERS, -1);
    for (int i = 0; i < NCPI_MAX_COMMAND_NUMBERS; i++)
    {
        if (__numbers[i] == -1)
            break;
        numbers.push_back(__numbers[i]);
    }

    //Получение тэга паттерна команды.
    int tag = SCPI_CmdTag(context);

    //Поиск темы производится по тэгк паттерна и номерам идентификаторов.
    NotifyTheme *theme = brocker.themeFind(tag, numbers);
    if (theme == nullptr)
    {
        SCPI_ErrorPush(context, SCPI_ERROR_HEADER_SUFFIX_OUTOFRANGE);
        return SCPI_RES_ERR;
    };

    //Если клиент зарегистрирован, возвращаем соответственный результат.
    SCPI_ResultInt32(context, theme->clientIsRegistry(&ucontext->ncl) ? 1 : 0);

    return SCPI_RES_OK;
}

scpi_result_t NCPI_unsubscribe(scpi_t *context)
{
    ncpi_user_context *ucontext = (ncpi_user_context *)context->user_context;
    ucontext->ncl.removeAllThemes();
    return SCPI_RES_OK;
}

void start_ncpi_server(int port)
{
    ncpi_port = port;
    ncpi_thread = std::thread(NCPI_notifyThreadFunc, port);
}

void stop_ncpi_server()
{
    ncpi_cancel_token = true;
    nos::inet::tcp_client::dial("127.0.0.1", ncpi_port);
    ncpi_thread.join();
}

void ncpi_server_join()
{
    ncpi_thread.join();
}