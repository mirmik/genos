#include <nos/check.h>
#include <nos/inet/tcp_client.h>
#include <nos/input.h>
#include <onboardtest/test.h>
#include <onboardtest/testenv.h>

nos::inet::tcp_client client;

void client_write(const char *data, size_t size)
{
    client.write(data, size);
}

void client_wait_syncronization()
{
    nos::readline_from(client);
}

int main(int argc, char **argv)
{
    client.connect("127.0.0.1", 10001);

    onboardtest::TestEnv testenv(client_write, client_wait_syncronization);
    testenv.start();

    client.close();
    return 0;
}

TEST_CASE("hello")
{
    NOS_CHECK_EQ(1, 1);
}