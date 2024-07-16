#include <doctest/doctest.h>
#include <nos/inet/tcpspam_server.h>
#include <nos/print.h>

TEST_CASE("tcpspam_server")
{
    nos::inet::tcpspam_server server;
    server.start(12345);
    nos::println_to(server, "Hello, world!");
}