#include <chrono>
#include <comm/scpi-def.h>
#include <doctest/doctest.h>
#include <idn.h>
#include <nos/inet/tcp_client.h>
#include <nos/input.h>
#include <nos/print.h>
#include <thread>

using namespace std::chrono_literals;

TEST_CASE("scpi api test")
{
    set_idn_from_string("A,B,C,D");
    start_scpi_server(10016);
    std::this_thread::sleep_for(100ms);
    auto client = nos::inet::tcp_client::dial("127.0.0.1", 10016);
    client.set_input_timeout(1000ms);
    nos::println_to(client, "*IDN?");
    auto ans = nos::readline_from(client, 1024, false);
    stop_scpi_server();
    CHECK_EQ(*ans, "A,B,C,D");
}
