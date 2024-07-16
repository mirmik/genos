#include <doctest/doctest.h>
#include <igris/shell/mshell.h>

#include <cstddef>
#include <cstring>
#include <stdlib.h>

#include <string>

static int add(int argc, char **argv)
{
    return atoi(argv[1]) + atoi(argv[2]);
}
static int sub(int argc, char **argv)
{
    return atoi(argv[1]) - atoi(argv[2]);
}
static int mul(int argc, char **argv)
{
    return atoi(argv[1]) * atoi(argv[2]);
}
static int div(int argc, char **argv)
{
    return atoi(argv[1]) / atoi(argv[2]);
}

struct mshell_command commands_A[] = {
    {"add", add, "Add"}, {"sub", sub, "Sub"}, {NULL, NULL, NULL}};

struct mshell_command commands_B[] = {
    {"mul", mul, "Mul"}, {"div", div, "Div"}, {NULL, NULL, NULL}};

struct mshell_command *command_tables[] = {commands_A, commands_B, NULL};

static std::string output;

static void write(void *, const char *data, size_t size)
{
    output += std::string(data, size);
}

TEST_CASE("mshell")
{
    int ret;
    char cmd[48];
    memset(cmd, 0, 48);

    strcpy(cmd, "add 13 23");
    mshell_execute(cmd, commands_A, &ret);
    CHECK_EQ(36, ret);

    strcpy(cmd, "sub 13 23");
    mshell_execute(cmd, commands_A, &ret);
    CHECK_EQ(-10, ret);

    strcpy(cmd, "mul 4 3");
    mshell_tables_execute(cmd, command_tables, &ret);
    CHECK_EQ(12, ret);

    strcpy(cmd, "div 8 2");
    mshell_tables_execute(cmd, command_tables, &ret);
    CHECK_EQ(4, ret);

    mshell_help(commands_A, write, NULL);
    CHECK_EQ(output, "add - Add\r\nsub - Sub\r\n");
    output.clear();

    mshell_tables_help(command_tables, write, NULL);
    CHECK_EQ(output, "add - Add\r\nsub - Sub\r\nmul - Mul\r\ndiv - Div\r\n");
    output.clear();
}
