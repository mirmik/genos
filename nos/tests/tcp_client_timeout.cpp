#include <doctest/doctest.h>
#include <nos/inet/tcp_client.h>
#include <nos/inet/tcp_server.h>
#include <stdio.h>

TEST_CASE("tcp client connect without timeout")
{
    srand((unsigned int)time(NULL));
    nos::inet::tcp_server server;
    server.init();

    int port;
    for (int i = 0; i < 10; i++)
    {
        port = 30000 + (rand() % 10000);
        if (server.bind(nos::inet::hostaddr::any(), port) == 0)
        {
            break;
        }
    }
    server.listen();

    nos::inet::tcp_client client;
    client.connect(nos::inet::hostaddr::loopback(), port);
    nos::inet::tcp_client sock = server.accept();
    sock.write("hello", 5);
    char buf[6];
    buf[5] = 0;
    client.recv(buf, 5, 0);
    CHECK_EQ(std::string(buf), std::string("hello"));
}

TEST_CASE("tcp client connect with timeout")
{
    srand((unsigned int)time(NULL) + 132123);
    nos::inet::tcp_server server;
    server.init();

    int port;
    for (int i = 0; i < 10; i++)
    {
        port = 30000 + (rand() % 10000);
        if (server.bind(nos::inet::hostaddr::any(), port) == 0)
        {
            break;
        }
    }
    server.listen();

    nos::inet::tcp_client client;
    client.connect(
        nos::inet::hostaddr::loopback(), port, std::chrono::milliseconds(100));
    nos::inet::tcp_client sock = server.accept();
    sock.write("hello", 5);
    char buf[6];
    buf[5] = 0;
    auto ans = client.read(buf, 5);
    CHECK_EQ(*ans, 5);
    CHECK_EQ(std::string(buf), std::string("hello"));
}

TEST_CASE("tcp client connect miss port!")
{
    srand((unsigned int)time(NULL) + 132343);
    nos::inet::tcp_server server;
    server.init();

    int port;
    for (int i = 0; i < 10; i++)
    {
        port = 30000 + (rand() % 10000);
        if (server.bind(nos::inet::hostaddr::any(), port) == 0)
        {
            break;
        }
    }

    server.listen();
    server.close();

    nos::inet::tcp_client client;
    CHECK_THROWS_AS(client.connect(nos::inet::hostaddr::loopback(),
                                   port,
                                   std::chrono::milliseconds(100)),
                    nos::inet::tcp_connect_error);
}

std::string UNKNOWN_HOST = "10.127.0.253";
TEST_CASE("tcp client connect without accept")
{
    srand((unsigned int)time(NULL) + 444);
    nos::inet::tcp_server server;
    server.init();

    int port;
    for (int i = 0; i < 10; i++)
    {
        port = 30000 + (rand() % 10000);
        if (server.bind(nos::inet::hostaddr::any(), port) == 0)
        {
            break;
        }
    }
    server.listen();

    nos::inet::tcp_client client;
    // TODO:
    // CHECK_THROWS_AS(
    //    client.connect(UNKNOWN_HOST, port, std::chrono::milliseconds(100)),
    //    nos::inet::tcp_timeout_error);
}
