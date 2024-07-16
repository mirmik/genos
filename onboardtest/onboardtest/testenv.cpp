#include <cstring>
#include <nos/check/env.h>
#include <onboardtest/testenv.h>

const char *onboardtest::onboardtest_exit_command = "***ONBOARD_TEST_EXIT***";

onboardtest::OnBoardTest::OnBoardTest(void (*test)(), const char *name)
    : test(test), name(name)
{
    TestEnvBinder::instance().add(this);
}

void onboardtest::TestEnvBinder::add(onboardtest::OnBoardTest *test)
{
    test->next = first;
    first = test;
}

onboardtest::TestEnvBinder &onboardtest::TestEnvBinder::instance()
{
    static onboardtest::TestEnvBinder binder;
    return binder;
}

onboardtest::TestEnv::TestEnv(void (*write)(const char *, size_t),
                              void (*wait_syncronization)())
    : _write(write), _wait_syncronization(wait_syncronization)
{
}

void onboardtest::TestEnv::start()
{
    _wait_syncronization();
    int status;

    nos_check_error_msg = "";
    int setjmp_result = setjmp(nos_check_start_point);

    if (setjmp_result == 0)
    {
        for (auto test = TestEnvBinder::instance().first; test;
             test = test->next)
        {
            _write("Start test case '", 17);
            _write(test->name, strlen(test->name));
            _write("'\r\n", 3);
            test->test();
        }
        status = 0;
    }
    NOS_CHECK_ENV_ERROR_HANDLER()
    {
        status = 1;
    }

    _write(onboardtest_exit_command, strlen(onboardtest_exit_command));
    _write("\r\n", 2);
    if (status == 0)
    {
        _write("success\r\n", 9);
    }
    else
    {
        _write("error\r\n", 7);
    }
}