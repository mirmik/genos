#include <arpa/inet.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <signal.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

extern std::string ircc_string(const std::string &key);

std::vector<std::string> split(const std::string &str, char delim)
{
    std::vector<std::string> outvec;

    char *strt;
    char *ptr = (char *)str.data();
    char *end = (char *)str.data() + str.size();

    while (true)
    {
        while (*ptr == delim)
            ptr++;

        if (ptr == end)
            break;

        strt = ptr;

        while (ptr != end && *ptr != delim)
            ptr++;

        outvec.emplace_back(strt, ptr - strt);
    }

    return outvec;
}

int make_server(std::string host, int port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(-1);
    }

    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(host.c_str());
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind");
        exit(-1);
    }
    listen(sockfd, 5);
    return sockfd;
}

void compile_http_response(std::string &response,
                           std::string &content,
                           std::string &content_type)
{
    response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: " + content_type + "\r\n";
    response += "Content-Length: " + std::to_string(content.size()) + "\r\n";
    response += "\r\n";
    response += content;
}

int read_line(int socket_fd, std::string &line)
{
    char c;
    int count = 0;
    while (read(socket_fd, &c, 1) > 0)
    {
        if (c == '\n')
        {
            break;
        }
        count++;
        line += c;
    }
    return count;
}

void client_thread_fn(int client_fd)
{
    try
    {
        std::string line;
        // open fd as fstream

        while (true)
        {
            std::string line;
            if (read_line(client_fd, line) > 0)
            {
                std::cout << "READ: " << line << std::endl;

                if (line.find("GET") == 0)
                {
                    std::string resource = split(line, ' ')[1];
                    if (resource == "/")
                    {
                        resource = "/index.html";
                    }
                    std::cout << "SEND Resource: " << resource << std::endl;
                    std::string content = ircc_string(resource);
                    std::string content_type = "text/html";
                    std::string response;
                    compile_http_response(response, content, content_type);
                    send(client_fd, response.c_str(), response.size(), 0);
                }
            }
        }
        close(client_fd);
    }
    catch (const std::exception &e)
    {
        std::cerr << "std::cin error: " << e.what() << std::endl;
    }
}

int server;

int main(int argc, const char * argv[])
{
    int port;
    if (argc >= 2) {
        port = atoi(argv[1]);
    } else {
        port = 8080;
    }

    /// simple http server
    server = make_server("0.0.0.0", port);
    std::cout << "Server started: port:" << port << std::endl;

    while (true)
    {
        struct sockaddr_in serv_addr;
        socklen_t serv_addr_len = sizeof(serv_addr);
        auto client =
            accept(server, (struct sockaddr *)&serv_addr, &serv_addr_len);
        if (client < 0)
        {
            perror("accept");
            break;
        }
        std::thread(client_thread_fn, client).detach();
    }
}
