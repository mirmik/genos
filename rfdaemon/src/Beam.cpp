#include <Beam.h>
#include <nos/trent/json.h>
#include <nos/trent/json_print.h>

std::map<std::string, nos::trent (Beam::*)(const nos::trent &)>
    Beam::method_map = {
        {"getsettings", &Beam::getsettings},
};

Beam::Beam()
{
    recv_socket.init();
    recv_socket.bind(RECVPORT);

    send_socket.init();
    send_socket.bind(SENDPORT);
    send_socket.allow_broadcast();
}

void Beam::start()
{
    beam_thread = std::thread(&Beam::beam_thread_func, this);
}

void Beam::beam_thread_func()
{
    nos::println("starting beam thread");
    for (;;)
    {
        auto [data, ip, port] = recv_socket.recvfrom();
        nos::println("NewBeamMessage:", data);

        try
        {
            nos::trent tr = nos::json::parse(data);
            on_new_command(tr, ip, port);
        }
        catch (const std::exception &e)
        {
            nos::println("Beam error:", e.what());
        }
    }
}

nos::trent Beam::getsettings(const nos::trent &tr)
{
    return tr;
}

void Beam::on_new_command(const nos::trent &tr, std::string ip, int port)
{
    if (!tr.contains("cmd"))
    {
        throw std::runtime_error("cmd field is required");
    }
    auto cmd = tr["cmd"].as_string();

    auto rettr = (this->*method_map[cmd])(tr);
    auto json = nos::json::to_string(rettr, false);
    send_socket.sendto(json.data(), json.size(), ip, port);
}