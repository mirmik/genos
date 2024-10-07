#ifndef BEAM_H
#define BEAM_H

#include <map>
#include <nos/inet/udp_socket.h>
#include <nos/trent/trent.h>
#include <thread>

class Beam
{
    static std::map<std::string, nos::trent (Beam::*)(const nos::trent &)>
        method_map;
    nos::inet::udp_broadcast_socket recv_socket = {};
    nos::inet::udp_broadcast_socket send_socket = {};
    std::thread beam_thread = {};

    static constexpr int RECVPORT = 9835;
    static constexpr int SENDPORT = 9836;

public:
    Beam();
    void start();
    void on_new_command(const nos::trent &tr, std::string ip, int port);

    nos::trent getsettings(const nos::trent &tr);

private:
    void beam_thread_func();
};

#endif