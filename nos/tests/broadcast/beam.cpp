#include <chrono>
#include <nos/inet/udp_socket.h>
#include <nos/fprint.h>
#include <thread>

int main(int argc, char *argv[])
{

    std::vector<std::string> args(argv + 1, argv + argc);
    if (args.size() != 1 && args.size() != 2)
    {
        nos::fprintln("Usage: {} <port> or {} <ip> <port>", argv[0], argv[0]);
        return 1;
    }


    if (argc == 2) 
    {
        int sendport = std::stoi(args[0]);

        nos::inet::udp_broadcast_socket sock;
	    sock.init();
	    //sock.bind(sendport+1);
        sock.allow_broadcast();

        while (true)
        {
            sock.send_broadcast("hello\n", 6, sendport);
            nos::println("sended");
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    if (argc == 3) 
    {
        int sendport = std::stoi(args[1]);

        nos::inet::udp_broadcast_socket sock;
        sock.init();
        sock.allow_broadcast();
        //sock.bind(sendport+1);
        
        while (true)
        {
            int sts = sock.sendto("hello\n", 6, args[0], sendport);
            nos::println("sended2", sts, args[0], sendport);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
