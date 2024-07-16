#include <chrono>
#include <nos/inet/udp_socket.h>
#include <thread>

int main(int argc, char *argv[])
{
    std::vector<std::string> args(argv + 1, argv + argc);
    if (args.size() != 1)
    {
        std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int recvport = std::stoi(args[0]);
    nos::inet::udp_broadcast_socket sock(recvport);

    while (true)
    {
        auto [data, addr, port] = sock.recvfrom();
        nos::println(addr, port, data);
    }
}