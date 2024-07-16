#ifndef IGRIS_DTRACE_H
#define IGRIS_DTRACE_H

#include <igris/dprint.h>
#include <igris/util/macro.h>

#if NODTRACE
#define DTRACE()
#define DTRRET()
#define DTRRET_ARG(arg)
#define DTRACE_RET(arg)
#else
#define DTRACE()                                                               \
    do                                                                         \
    {                                                                          \
        dpr("DTRACE: ");                                                       \
        dpr(__PRETTY_FUNCTION__);                                              \
        dln();                                                                 \
    } while (0);
#define DTRRET()                                                               \
    do                                                                         \
    {                                                                          \
        dpr("DTRRET<-");                                                       \
        dpr(__PRETTY_FUNCTION__);                                              \
        dln();                                                                 \
    } while (0);
#define DTRRET_ARG(arg)                                                        \
    do                                                                         \
    {                                                                          \
        dpr("DTRRET<-");                                                       \
        dpr(__PRETTY_FUNCTION__);                                              \
        dpr(" arg: ");                                                         \
        dpr(arg);                                                              \
        dln();                                                                 \
    } while (0);
#define DTRACE_RET(arg) (({DTRRET_ARG(arg)}), arg)
#endif

#define DPRARG(x)                                                              \
    dpr(STRINGIFY(x));                                                         \
    dprchar(':');                                                              \
    dpr(x);                                                                    \
    dprchar(' ');
#define DTRPRE()                                                               \
    do                                                                         \
    {                                                                          \
        dpr("DTRACE: ");                                                       \
        dpr(__PRETTY_FUNCTION__);                                              \
        dpr(" args: ");
#define DTRPOS()                                                               \
    dln();                                                                     \
    }                                                                          \
    while (0)                                                                  \
        ;

#define DTRACE_ARGS_9(a, b, c, d, e, f, g, h, i)                               \
    DTRPRE() ARGS_INVOKE_FOR_EACH(DPRARG, a, b, c, d, e, f, g, h, i);          \
    DTRPOS()
#define DTRACE_ARGS_8(a, b, c, d, e, f, g, h)                                  \
    DTRPRE() ARGS_INVOKE_FOR_EACH(DPRARG, a, b, c, d, e, f, g, h);             \
    DTRPOS()
#define DTRACE_ARGS_7(a, b, c, d, e, f, g)                                     \
    DTRPRE() ARGS_INVOKE_FOR_EACH(DPRARG, a, b, c, d, e, f, g);                \
    DTRPOS()
#define DTRACE_ARGS_6(a, b, c, d, e, f)                                        \
    DTRPRE() ARGS_INVOKE_FOR_EACH(DPRARG, a, b, c, d, e, f);                   \
    DTRPOS()
#define DTRACE_ARGS_5(a, b, c, d, e)                                           \
    DTRPRE() ARGS_INVOKE_FOR_EACH(DPRARG, a, b, c, d, e);                      \
    DTRPOS()
#define DTRACE_ARGS_4(a, b, c, d)                                              \
    DTRPRE() ARGS_INVOKE_FOR_EACH(DPRARG, a, b, c, d);                         \
    DTRPOS()
#define DTRACE_ARGS_3(a, b, c)                                                 \
    DTRPRE() ARGS_INVOKE_FOR_EACH(DPRARG, a, b, c);                            \
    DTRPOS()
#define DTRACE_ARGS_2(a, b)                                                    \
    DTRPRE() DPRARG(a);                                                        \
    DPRARG(b);                                                                 \
    DTRPOS()
#define DTRACE_ARGS_1(a)                                                       \
    DTRPRE() DPRARG(a);                                                        \
    DTRPOS()

#if NODTRACE
#define DTRACE_ARGS(...)
#else
#define DTRACE_ARGS(...)                                                       \
    CONCAT2(DTRACE_ARGS_, COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)
#endif

#define DTRACE_CONDITION(en)                                                   \
    if (en)                                                                    \
        DTRACE();
#define DTRACE_ARGS_CONDITION(en, ...)                                         \
    if (en)                                                                    \
    CONCAT2(DTRACE_ARGS_, COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

#endif
