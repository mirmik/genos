#ifndef CANOPEN_H
#define CANOPEN_H

#include <chrono>
#include <nos/io/file.h>
#include <nos/log.h>

class CanKollm;

class Can
{
public:
    CanKollm *driver = nullptr;
    nos::file file = {};
    const char *portname = nullptr;
    bool echo = false;

    nos::log::logger logger =
        nos::log::logger("Can").set_level(nos::log::level::Info);

    Can() = default;
    Can(const Can &) = delete;
    Can &operator=(const Can &) = delete;
    Can(Can &&) = delete;
    Can &operator=(Can &&) = delete;
    ~Can() = default;

public:
    void open(const char *str);
    void reconnect();
    void close();
    void reset();
    void send_test();
    void send(const char *str, size_t sz);
    void recv_func();
    void parse(char *str);
};

#endif
