#include <nos-shell/standart.h>
#include <nos/fprint.h>
#include <nos/inet/tcp_client.h>
#include <nos/inet/tcp_server.h>
#include <nos/input.h>
#include <nos/print.h>
#include <nos/shell/argv.h>
#include <nos/shell/executor.h>

#include <string>
#include <thread>

std::string execute_tokens(const nos::argv &args, nos::executor &executor)
{
    nos::string_buffer sb;
    if (args.size() == 0)
        return "";
    executor.execute(args, sb);
    return sb.str();
}

void client_spin(nos::inet::tcp_client client,
                 nos::executor &executor,
                 nos::cancel_token cancel)
{
    try
    {
        while (true)
        {
            if (cancel)
                return;
            auto expline = nos::readline_from(client, 1024, false);
            if (expline.is_error() || expline->size() == 0)
                return;

            nos::tokens tokens(*expline);
            auto sb = execute_tokens(nos::argv(tokens), executor);
            if (sb.size() == 0)
                continue;
            client.write(sb.data(), sb.size());
        }
    }
    catch (const std::exception &ex)
    {
        nos::println("client finished by exception", ex.what());
    }
}

void nos::shell_tcp_thread_function(
    int port,
    std::reference_wrapper<nos::executor> executor,
    nos::cancel_token cancel)
{
    nos::inet::tcp_server server;
    server.init();
    server.reusing(true);
    server.bind("0.0.0.0", port);
    server.listen();

    while (true)
    {
        if (cancel)
            return;
        nos::println("Waiting for client...");
        nos::inet::tcp_client client = server.accept();
        nos::println("Client connected");
        std::thread client_thread(client_spin, client, executor, cancel);
        client_thread.detach();
    }
}

void nos::shell_io_thread_function(
    std::reference_wrapper<nos::istream> is,
    std::reference_wrapper<nos::ostream> os,
    std::reference_wrapper<nos::executor> executor,
    nos::cancel_token cancel)
{
    std::string str;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    while (1)
    {
        if (cancel)
            return;
        nos::print_to(os, "$ ");
        str = *nos::readline_from(is, 1024, false);
        nos::tokens tokens(str);
        auto sb = execute_tokens(nos::argv(tokens), executor);
        if (sb.size() == 0)
            continue;
        nos::print_to(os, sb);
    }
}
