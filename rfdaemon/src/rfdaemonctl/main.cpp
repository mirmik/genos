#include <chrono>
#include <defs.h>
#include <igris/getopt/cliopts.h>
#include <igris/util/base64.h>
#include <nos/inet/tcp_client.h>
#include <nos/input.h>

using namespace std::chrono_literals;

int main(int argc, char **argv)
{
    igris::cliopts parser;
    parser.add_string("host", 'H', "127.0.0.1");
    parser.add_integer("port", 'p', RFDAEMON_DEFAULT_API_PORT);

    auto opts = parser.parse(argc - 1, argv + 1);
    auto host = parser.get_string("host");
    auto port = parser.get_integer("port");
    auto args = parser.get_args();

    nos::inet::tcp_client client;
    try
    {
        client = nos::inet::tcp_client::dial(host.value(), port);
    }
    catch (const std::exception &ex)
    {
        std::cout << "Can't connect to " << host.value() << ":" << port.value()
                  << std::endl;
        return -1;
    };

    std::string command;
    if (args.size() == 0)
    {
        std::cout << "No command specified" << std::endl;
        return -1;
    }
    else
    {
        command = args[0];
    }

    if (command == "list")
    {
        nos::print_to(client, "b64out list\n");
        std::string b64str = *nos::readline_from(client);
        std::string text = igris::base64_decode(b64str);
        std::cout << text << std::endl;
        return 0;
    }
    else if (command == "start")
    {
        if (args.size() < 2)
        {
            std::cout << "No name specified" << std::endl;
            return -1;
        }

        nos::print_to(client, nos::format("b64out start {}\n", args[1]));
        std::string b64str = *nos::readline_from(client);
        std::string text = igris::base64_decode(b64str);
        std::cout << text << std::endl;
        return 0;
    }
    else if (command == "stop")
    {
        if (args.size() < 2)
        {
            std::cout << "No name specified" << std::endl;
            return -1;
        }

        nos::print_to(client, nos::format("b64out stop {}\n", args[1]));
        std::string b64str = *nos::readline_from(client);
        std::string text = igris::base64_decode(b64str);
        std::cout << text << std::endl;
        return 0;
    }
    else if (command == "restart")
    {
        if (args.size() < 2)
        {
            std::cout << "No name specified" << std::endl;
            return -1;
        }

        nos::print_to(client, nos::format("b64out restart {}\n", args[1]));
        std::string b64str = *nos::readline_from(client);
        std::string text = igris::base64_decode(b64str);
        std::cout << text << std::endl;
        return 0;
    }
    else if (command == "status")
    {
        if (args.size() < 2)
        {
            std::cout << "No name specified" << std::endl;
            return -1;
        }

        nos::print_to(client, nos::format("b64out status {}\n", args[1]));
        std::string b64str = *nos::readline_from(client);
        std::string text = igris::base64_decode(b64str);
        std::cout << text << std::endl;
        return 0;
    }
    else if (command == "log")
    {
        if (args.size() < 2)
        {
            std::cout << "No name specified" << std::endl;
            return -1;
        }

        nos::print_to(client, nos::format("b64out log {}\n", args[1]));
        std::string b64str = *nos::readline_from(client);
        std::string text = igris::base64_decode(b64str);
        std::cout << text << std::endl;
        return 0;
    }
    else if (command == "logstream")
    {
        if (args.size() < 2)
        {
            std::cout << "No name specified" << std::endl;
            return -1;
        }

        nos::print_to(client, nos::format("logstream {}\n", args[1]));

        while (1)
        {
            auto estr = nos::readline_from(client);
            if (estr.is_error())
                return 0;
            nos::print(*estr);
        }

        return 0;
    }

    nos::println("Unknown command");
    return -1;
}