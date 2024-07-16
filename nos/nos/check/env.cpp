#include <cstring>
#include <nos/check/env.h>

jmp_buf nos_check_start_point;
std::string nos_check_error_msg;

void NOS_CHECK_ENV_RAISE_SET(std::string &msg)
{
    nos_check_error_msg = msg;
}

void NOS_CHECK_ENV_RAISE_IMPL()
{
    longjmp(nos_check_start_point, 1);
}