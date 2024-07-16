/** @file */

#ifndef NOS_TRACE_H
#define NOS_TRACE_H

#include <nos/print.h>
#include <nos/util/macro.h>

namespace nos
{
    extern thread_local unsigned int trace_level;

    class tracer
    {
    public:
        const char* func=nullptr;
        void* retptr=nullptr;
        void* visit=nullptr;

        tracer(const char* func)
        {
            this->func = func;
            ++trace_level;
        }

        tracer(const tracer&) = default;
        tracer& operator=(const tracer&) = default;       

        void print_out()
        {
            nos::print("TRACE: ");
            nos::print(trace_level);
            nos::print(": <- ");
            nos::print(func);
            func = nullptr;
        }

        ~tracer()
        {
            if (func) {
                print_out();
                nos::println();
            }
            --trace_level;
        }
    };
}

#define PRARG(x)                    \
    nos::print(STRINGIFY(x));       \
    nos::putbyte(':');              \
    nos::print(x);                  \
    nos::putbyte(' ');

#define TRPRE_S()                                       \
    nos::tracer __nos_tracer(__PRETTY_FUNCTION__);      \
    do {                                                \
        nos::print("TRACE: ");                          \
        nos::print(nos::trace_level);                   \
        nos::print(": -> ");                            \
        nos::print(__PRETTY_FUNCTION__);

#define TRPRE()                             \
    TRPRE_S()                               \
    nos::print(" args: ");

#define TRPOS() nos::println(); } while(0);

#define TRACE_ARGS_9(a,b,c,d,e,f,g,h,i)    TRPRE() ARGS_INVOKE_FOR_EACH(PRARG,a,b,c,d,e,f,g,h,i); TRPOS()
#define TRACE_ARGS_8(a,b,c,d,e,f,g,h)      TRPRE() ARGS_INVOKE_FOR_EACH(PRARG,a,b,c,d,e,f,g,h); TRPOS()
#define TRACE_ARGS_7(a,b,c,d,e,f,g)        TRPRE() ARGS_INVOKE_FOR_EACH(PRARG,a,b,c,d,e,f,g); TRPOS()
#define TRACE_ARGS_6(a,b,c,d,e,f)          TRPRE() ARGS_INVOKE_FOR_EACH(PRARG,a,b,c,d,e,f); TRPOS()
#define TRACE_ARGS_5(a,b,c,d,e)            TRPRE() ARGS_INVOKE_FOR_EACH(PRARG,a,b,c,d,e); TRPOS()
#define TRACE_ARGS_4(a,b,c,d)              TRPRE() ARGS_INVOKE_FOR_EACH(PRARG,a,b,c,d); TRPOS()
#define TRACE_ARGS_3(a,b,c)                TRPRE() ARGS_INVOKE_FOR_EACH(PRARG,a,b,c); TRPOS()
#define TRACE_ARGS_2(a,b)                  TRPRE() ARGS_INVOKE_FOR_EACH(PRARG,a,b); TRPOS()
#define TRACE_ARGS_1(a)                    TRPRE() PRARG(a); TRPOS()

#define TRACE_ARGS_PR(...) CONCAT2(TRACE_ARGS_,COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

#if defined(NOTRACE) && NOTRACE!=0
#define TRACE()
#define TRACE_ARGS(...)
#define TRRET(ex) ex
#else
#define TRACE() TRPRE_S() TRPOS();
#define TRACE_ARGS(...) CONCAT2(TRACE_ARGS_,COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)
#define TRRET(ex) [&__nos_tracer](auto ret){ __nos_tracer.print_out(); nos::println(" return:", ret); return ret; }(ex)
#endif

#endif
