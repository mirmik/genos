#include <doctest/doctest.h>
#include <igris/defs/vt100.h>
#include <igris/shell/vtermxx.h>

#include <string>

static std::string output;

static int a = 0;
static int b = 0;

static void signal_callback(int sig)
{
    b++;
}

static void execute_callback(const char *data, unsigned int size)
{
    a++;
    CHECK_EQ(std::string(data, size), "hello");
}

static void write_callback(const char *data, unsigned int size)
{
    output += std::string(data, size);
}

TEST_CASE("vterm")
{
    igris::vtermxx vterm;
    vterm.init(48, 5);

    vterm.set_write_callback(write_callback);
    vterm.set_execute_callback(execute_callback);

    output.clear();
    vterm.init_step();

    CHECK_EQ(output, "$ ");
    output.clear();

    for (char c : std::string("hello"))
    {
        vterm.newdata(c);
    }

    CHECK_EQ(output.size(), 5);
    CHECK_EQ(output, "hello");

    vterm.newdata('\r');
    vterm.newdata('\n');
    CHECK_EQ(a, 1);

    for (char c : std::string("hello\r\n"))
    {
        vterm.newdata(c);
    }
    CHECK_EQ(a, 2);

    for (char c : std::string("e" VT100_LEFT "h" VT100_RIGHT "llo\r\n"))
    {
        vterm.newdata(c);
    }
    CHECK_EQ(a, 3);

    for (char c : std::string("hello\n"))
    {
        vterm.newdata(c);
    }
    CHECK_EQ(a, 4);

    vterm.newdata(3);
    CHECK_EQ(b, 0);

    vterm.set_signal_callback(signal_callback);

    vterm.newdata(3);
    CHECK_EQ(b, 1);
}
