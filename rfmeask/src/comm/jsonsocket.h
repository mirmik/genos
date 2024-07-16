#ifndef JSON_API_SERVER_H
#define JSON_API_SERVER_H

#include <nos/inet/tcp_client.h>
#include <nos/inet/tcp_server.h>
#include <nos/shell/weaked_function.h>
#include <nos/shell/weaked_function_interpreter.h>
#include <string>
#include <thread>
#include <vector>

extern std::string system_idn();

class JsonApiClient
{
    nos::inet::tcp_client sock = {};
    std::thread _thread;
    bool status_notification_enabled = false;
    nos::weaked_function_interpreter &interpreter;

public:
    JsonApiClient(nos::inet::tcp_client &&sock,
                  nos::weaked_function_interpreter &wfi) :
        sock(std::move(sock)), interpreter(wfi)
    {
        _thread = std::thread([this]() { this->run(); });
    }

    std::thread &thread()
    {
        return _thread;
    }

    nos::inet::tcp_client &socket()
    {
        return sock;
    }

    void set_status_notification_enabled(bool enabled)
    {
        status_notification_enabled = enabled;
    }

    void run();
    void json_handler(const std::string &msg);
    void console_handler(const std::string &msg);
};

class JsonApiServer
{
    std::vector<JsonApiClient> clients;
    nos::weaked_function_interpreter interpreter;
    volatile bool cancel_thread = false;
    std::thread server_thread = {};
    nos::inet::tcp_server server = {};
    int port;

public:
    void start(int port);
    void stop();
    void init_wf_collection();

    ~JsonApiServer()
    {
        server.close_socket();
        cancel_thread = true;

        if (server_thread.joinable())
            server_thread.join();

        for (auto &client : clients)
        {
            if (client.socket().good())
                client.socket().disconnect();
            if (client.thread().joinable())
                client.thread().join();
        }
    }

    static JsonApiServer &instance()
    {
        static JsonApiServer instance;
        return instance;
    }
};

#endif
