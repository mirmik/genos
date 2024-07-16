#include <doctest/doctest.h>
#include <nos/io/sstream.h>
#include <nos/log.h>
#include <nos/log/ostream_target.h>
#include <nos/log/target_logger.h>

TEST_CASE("nos.log")
{
    nos::stringstream ss;
    nos::current_ostream = &ss;
    nos::log::info("Hello, world!");
    CHECK_EQ(ss.str(), "[ info] Hello, world!\r\n");
    nos::current_ostream = nullptr;
}

TEST_CASE("logging level")
{
    nos::stringstream ss;
    nos::current_ostream = &ss;
    auto logger = nos::log::logger("logger").set_level(nos::log::level::Info);
    logger.log(nos::log::level::Debug, "Hello, world!");
    CHECK_EQ(ss.str(), "");
    logger.set_level(nos::log::level::Debug);
    logger.log(nos::log::level::Debug, "Hello, world!");
    CHECK_EQ(ss.str(), "[debug] Hello, world!\r\n");
    nos::current_ostream = nullptr;
}

TEST_CASE("logger copy test")
{
    auto logger = nos::log::logger("logger")
                      .set_level(nos::log::level::Info)
                      .set_pattern("[{level}] {msg}\r\n");
    auto logger2 = logger;

    CHECK_EQ(logger2.name(), "logger");
    CHECK_EQ(logger2.minlevel(), nos::log::level::Info);
    CHECK_EQ(logger2.pattern(), "[{level}] {msg}\r\n");
}

TEST_CASE("levels comparison")
{
    CHECK_EQ(nos::log::level::Trace, nos::log::level::Trace);
    CHECK_EQ(nos::log::level::Debug, nos::log::level::Debug);
    CHECK_EQ(nos::log::level::Info, nos::log::level::Info);
    CHECK_EQ(nos::log::level::Warn, nos::log::level::Warn);
    CHECK_EQ(nos::log::level::Error, nos::log::level::Error);
    CHECK_EQ(nos::log::level::Fault, nos::log::level::Fault);

    CHECK_GT(nos::log::level::Debug, nos::log::level::Trace);
    CHECK_GT(nos::log::level::Info, nos::log::level::Debug);
    CHECK_GT(nos::log::level::Warn, nos::log::level::Info);
    CHECK_GT(nos::log::level::Error, nos::log::level::Warn);
    CHECK_GT(nos::log::level::Fault, nos::log::level::Error);
    CHECK_GT(nos::log::level::Fault, nos::log::level::Trace);

    CHECK_LT(nos::log::level::Trace, nos::log::level::Debug);
    CHECK_LT(nos::log::level::Debug, nos::log::level::Info);
    CHECK_LT(nos::log::level::Info, nos::log::level::Warn);
    CHECK_LT(nos::log::level::Warn, nos::log::level::Error);
    CHECK_LT(nos::log::level::Error, nos::log::level::Fault);
    CHECK_LT(nos::log::level::Trace, nos::log::level::Fault);
}

class AAA
{
public:
    nos::log::logger logger{"KollmorgenServoControl"};

    AAA()
    {
        logger.set_level(nos::log::level::Info);
    }
};

class BBB
{
public:
    nos::log::logger logger = nos::log::logger("KollmorgenServoControl");

    BBB()
    {
        logger.set_level(nos::log::level::Info);
    }
};

class CCC
{
public:
    nos::log::logger logger = nos::log::logger("KollmorgenServoControl")
                                  .set_level(nos::log::level::Info);
};

TEST_CASE("class initialization")
{
    nos::log::logger logger{"KollmorgenServoControl"};
    CHECK_EQ(logger.name(), "KollmorgenServoControl");
    {
        AAA a;
        CHECK_EQ(a.logger._name, "KollmorgenServoControl");
        CHECK_EQ(a.logger.name(), "KollmorgenServoControl");
        CHECK_EQ(a.logger.minlevel(), nos::log::level::Info);
        CHECK_EQ(a.logger.pattern(), "[{level}] {msg}\r\n");
    }
    {
        BBB b;
        CHECK_EQ(b.logger._name, "KollmorgenServoControl");
        CHECK_EQ(b.logger.name(), "KollmorgenServoControl");
        CHECK_EQ(b.logger.minlevel(), nos::log::level::Info);
        CHECK_EQ(b.logger.pattern(), "[{level}] {msg}\r\n");
    }
    {
        CCC c;
        CHECK_EQ(c.logger._name, "KollmorgenServoControl");
        CHECK_EQ(c.logger.name(), "KollmorgenServoControl");
        CHECK_EQ(c.logger.minlevel(), nos::log::level::Info);
        CHECK_EQ(c.logger.pattern(), "[{level}] {msg}\r\n");
    }

    {
        nos::newline_string = "\n";
        CCC c;
        CHECK_EQ(c.logger._name, "KollmorgenServoControl");
        CHECK_EQ(c.logger.name(), "KollmorgenServoControl");
        CHECK_EQ(c.logger.minlevel(), nos::log::level::Info);
        CHECK_EQ(c.logger.pattern(), "[{level}] {msg}\n");
        nos::newline_string = "\r\n";
    }
}

TEST_CASE("target log")
{
    nos::stringstream ss;
    nos::log::ostream_target target(ss);
    nos::log::target_logger logger({target});
    logger.log(nos::log::level::Info, "Hello, world!");

    CHECK_EQ(ss.str(), "[ info] Hello, world!\r\n");
}