#include <chrono>
#include <comm/jsonsocket.h>
#include <doctest/doctest.h>
#include <idn.h>
#include <nos/inet/tcp_client.h>
#include <nos/input.h>
#include <nos/trent/json.h>
#include <thread>

extern std::string system_idn();

TEST_CASE("jsonapi")
{
    JsonApiServer server;
    int port = 10022;
    set_idn_from_string("A,B,C,D");
    server.start(port);
    auto client = nos::inet::tcp_client::dial("127.0.0.1", port);
    client.set_input_timeout(1000ms);
    nos::println_to(client, "{\"cmd\":\"idn\"}");
    auto ans = nos::readline_from(client, 1024, false);
    server.stop();
    auto tr = nos::json::parse(*ans);
    CHECK_EQ(tr["result"].as_string(), "A,B,C,D");
    CHECK_EQ(tr["success"].as_bool(), true);
}
