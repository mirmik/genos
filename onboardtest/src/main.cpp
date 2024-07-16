#include <getopt.h>
#include <memory>
#include <nos/fprint.h>
#include <nos/inet/tcp_server.h>
#include <nos/input.h>
#include <nos/io/serial_port.h>
#include <nos/util/string.h>
#include <onboardtest/testenv.h>
#include <string>

const char *VERSION = "0.1.0";
std::string filename = {};

static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                       {"version", no_argument, 0, 'v'},
                                       {0, 0, 0, 0}};

void parse_args(int argc, char **argv)
{
    int c;
    while (1)
    {
        int option_index = 0;
        c = getopt_long(argc, argv, "hvf:", long_options, &option_index);
        if (c == -1)
            break;
        switch (c)
        {
        case 'h':
            nos::fprint("Usage: {} FILE [options]\n"
                        "Options:\n"
                        "  -h, --help\t\tShow this help message\n"
                        "  -v, --version\t\tShow version\n",
                        argv[0]);
            exit(0);

        case 'v':
            nos::fprintln("Version: {}", VERSION);
            break;

        case '?':
            break;
        }
    }

    if (argc - optind == 0)
    {
        nos::fprint("Usage: {} FILE [options]\n"
                    "Try '{} --help' for more information.\n",
                    argv[0],
                    argv[0]);
        exit(1);
    }

    if (argc - optind > 1)
    {
        nos::fprint("Usage: {} FILE [options]\n"
                    "Try '{} --help' for more information.\n",
                    argv[0],
                    argv[0]);
        exit(1);
    }

    filename = argv[optind];
}

void start_with_iostream(nos::iostream &stream)
{
    nos::fprintln("Starting with iostream");

    // send start command
    nos::println_to(stream, "STARTTESTS");

    while (1)
    {
        auto exstr = nos::readline_from(stream);
        if (exstr.is_error())
        {
            nos::fprintln("Connection closed");
            break;
        }

        if (nos::trim(exstr.value()) == onboardtest::onboardtest_exit_command)
        {
            auto exstatus = nos::readline_from(stream);

            nos::fprintln("Exit command received");

            if (nos::trim(*exstatus) == "success")
            {
                nos::fprintln("Status: Success");
                exit(0);
            }
            else if (nos::trim(*exstatus) == "error")
            {
                nos::fprintln("Status: Failure");
                exit(-1);
            }
            else
            {
                nos::fprintln("Unknown exit status");
                exit(-1);
            }
            break;
        }

        nos::fprintln("Received: {}", exstr.value());
    }
}

void start(const std::string &filename)
{
    if (filename[0] == '/')
    {
        nos::serial_port port(filename.c_str(), 115200, 'n', 8, 1);
        if (!port.good())
        {
            nos::println("Failed to open port");
            exit(1);
        }

        start_with_iostream(port);
        port.close();
    }
    else if (filename[0] == ':')
    {
        auto portno = std::stoi(filename.substr(1));
        nos::inet::tcp_server server("0.0.0.0", portno);
        server.reusing(true);
        if (!server.good())
        {
            nos::println("Failed to open port");
            exit(1);
        }

        nos::fprintln("Waiting for connection on port {}...", portno);
        auto client = server.accept();
        if (!client.good())
        {
            nos::println("Failed to accept client");
            exit(1);
        }

        nos::println("Accepted tcp client");
        start_with_iostream(client);

        client.close();
        server.close();
    }
    else
    {
        nos::println("Invalid port name");
        exit(1);
    }
}

int main(int argc, char **argv)
{
    parse_args(argc, argv);

    if (filename.empty())
    {
        nos::println("No file specified");
        return 1;
    }

    start(filename);
}