#ifndef ONBOARDTEST_TESTENV_H_
#define ONBOARDTEST_TESTENV_H_

#include <cstddef>
#include <string>

namespace onboardtest
{
    extern const char *onboardtest_exit_command;

    class OnBoardTest
    {
    public:
        void (*test)();
        OnBoardTest *next = nullptr;
        const char *name;

    public:
        OnBoardTest(void (*test)(), const char *name);
    };

    class TestEnvBinder
    {
    public:
        OnBoardTest *first = nullptr;
        TestEnvBinder() = default;

    public:
        void add(OnBoardTest *test);
        static TestEnvBinder &instance();
    };

    class TestEnv
    {
        void (*_write)(const char *, size_t);
        void (*_wait_syncronization)();

    public:
        TestEnv(void (*write)(const char *, size_t),
                void (*wait_syncronization)());
        ~TestEnv() = default;
        void start();
    };
} // namespace onboardtest

#endif