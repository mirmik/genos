#include <doctest/doctest.h>
#include <nos/io/sstream.h>
#include <nos/shell/aggregate_executor.h>
#include <nos/shell/argv.h>
#include <nos/shell/executor.h>
#include <nos/util/osutil.h>

TEST_CASE("shell")
{
    char *cmd = nos::osutil::strdup("hello world shell  \t\n jigly");
    nos::tokens argv(cmd);

    CHECK_EQ(argv[0], "hello");
    CHECK_EQ(argv[1], "world");
    CHECK_EQ(argv[2], "shell");
    CHECK_EQ(argv[3], "jigly");

    free(cmd);
}

int hello_t_void(const nos::argv &argv, nos::ostream &os, void *)
{
    nos::print_to(os, "lalala");
    CHECK_EQ(argv[1], "a");
    CHECK_EQ(argv.size(), 2);
    return 0;
}

int hello(const nos::argv &argv, nos::ostream &os)
{
    nos::print_to(os, "lalala");
    CHECK_EQ(argv[1], "a");
    CHECK_EQ(argv.size(), 2);
    return 0;
}

int af(const nos::argv &, nos::ostream &os)
{
    nos::print_to(os, "a");
    return 0;
}

int bf(const nos::argv &, nos::ostream &os)
{
    nos::print_to(os, "b");
    return 0;
}

int cf(const nos::argv &, nos::ostream &os)
{
    nos::print_to(os, "c");
    return 0;
}

int df(const nos::argv &, nos::ostream &os)
{
    nos::print_to(os, "d");
    return 0;
}

TEST_CASE("tokens")
{
    char *cmd = nos::osutil::strdup("hello a");
    auto tokens = nos::tokens(cmd);
    CHECK_EQ(tokens[0], "hello");
    CHECK_EQ(tokens[1], "a");
    free(cmd);
}

TEST_CASE("executor")
{
    char *cmd = nos::osutil::strdup("hello a");
    nos::stringstream answer;
    nos::executor executor({{"hello", "help example", hello}});

    auto tokens = nos::tokens(cmd);
    auto argv = nos::argv(tokens);
    CHECK_EQ(tokens.size(), 2);
    CHECK_EQ(argv.size(), 2);

    executor.execute(tokens, answer);
    CHECK_EQ(answer.str(), "lalala");
    free(cmd);
}

TEST_CASE("executor_t")
{
    char *cmd = nos::osutil::strdup("hello a");
    nos::stringstream answer;
    nos::executor_t<void *> executor({{"hello", "help example", hello_t_void}});

    auto tokens = nos::tokens(cmd);
    auto argv = nos::argv(tokens);
    CHECK_EQ(tokens.size(), 2);
    CHECK_EQ(argv.size(), 2);

    executor.execute(tokens, answer, nullptr);
    CHECK_EQ(answer.str(), "lalala");
    free(cmd);
}

TEST_CASE("aggregate_executor")
{
    nos::stringstream answer;

    nos::executor abexecutor({
        {"af", "", af},
        {"bf", "", bf},
    });

    nos::executor cdexecutor({
        {"cf", "", cf},
        {"df", "", df},
    });

    nos::aggregate_executor executor({&abexecutor, &cdexecutor});

    executor.execute("af", answer);
    CHECK_EQ(answer.str(), "a");

    executor.execute("bf", answer);
    CHECK_EQ(answer.str(), "ab");

    executor.execute("cf", answer);
    CHECK_EQ(answer.str(), "abc");

    executor.execute("df", answer);
    CHECK_EQ(answer.str(), "abcd");
}