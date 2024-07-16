#ifndef NOS_CHECK_ENV_H
#define NOS_CHECK_ENV_H

#include <setjmp.h>
#include <string>

extern jmp_buf nos_check_start_point;
extern std::string nos_check_error_msg;

#define NOS_CHECK_ENV_START()                                                  \
    if (nos_check_error_msg = "", setjmp(nos_check_start_point) == 0)

#define NOS_CHECK_ENV_ERROR_HANDLER() else

void NOS_CHECK_ENV_RAISE_SET(std::string &msg);
void NOS_CHECK_ENV_RAISE_IMPL();

#endif