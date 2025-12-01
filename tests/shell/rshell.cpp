#include <doctest/doctest.h>
#include <igris/shell/rshell.h>

#include <cstddef>
#include <cstring>
#include <stdlib.h>

#include <igris/buffer.h>
#include <string>

static int add(int argc, char **argv, char* ans, int ansmax) { return atoi(argv[1]) + atoi(argv[2]); }
static int sub(int argc, char **argv, char* ans, int ansmax) { return atoi(argv[1]) - atoi(argv[2]); }
static int mul(int argc, char **argv, char* ans, int ansmax) { return atoi(argv[1]) * atoi(argv[2]); }
static int div(int argc, char **argv, char* ans, int ansmax) { return atoi(argv[1]) / atoi(argv[2]); }

static struct rshell_command commands_A[] = {
    {"add", add, "Add"}, 
    {"sub", sub, "Sub"}, 
    {NULL, NULL, NULL}};

static struct rshell_command commands_B[] = {
    {"mul", mul, "Mul"}, 
    {"div", div, "Div"}, 
    {NULL, NULL, NULL}};

static struct rshell_command_table command_tables[] =
{
    { commands_A, 0 },
    { commands_B, 0 },
    { nullptr, 0 }
};

TEST_CASE("rshell")
{
    int ret;
    char cmd[48];
    char ans[48];
    memset(cmd, 0, 48);

    strcpy(cmd, "add 13 23");
    rshell_execute(cmd, commands_A, &ret, 0, ans, 48);
    CHECK_EQ(36, ret);

    strcpy(cmd, "sub 13 23");
    rshell_execute(cmd, commands_A, &ret, 0, ans, 48);
    CHECK_EQ(-10, ret);

    strcpy(cmd, "mul 4 3");
    rshell_tables_execute(cmd, command_tables, &ret, ans, 48);
    CHECK_EQ(12, ret);

    strcpy(cmd, "div 8 2");
    rshell_tables_execute(cmd, command_tables, &ret, ans, 48);
    CHECK_EQ(4, ret);

    rshell_help(commands_A, ans, 48);
    CHECK_EQ(std::string(ans), "add - Add\r\nsub - Sub\r\n");

    rshell_tables_help(command_tables, ans, 48);
    CHECK_EQ(std::string(ans), "add - Add\r\nsub - Sub\r\nmul - Mul\r\ndiv - Div\r\n");
}
