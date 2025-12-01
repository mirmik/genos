#ifndef IGRIS_UTIL_MACRO_H
#define IGRIS_UTIL_MACRO_H

#define ARGS_INVOKE_FOR_EACH_10(I, a, b, c, d, e, f, g, h, i)                  \
    do                                                                         \
    {                                                                          \
        I(a);                                                                  \
        I(b);                                                                  \
        I(c);                                                                  \
        I(d);                                                                  \
        I(e);                                                                  \
        I(f);                                                                  \
        I(g);                                                                  \
        I(h);                                                                  \
        I(i);                                                                  \
    } while (0)
#define ARGS_INVOKE_FOR_EACH_9(I, a, b, c, d, e, f, g, h)                      \
    do                                                                         \
    {                                                                          \
        I(a);                                                                  \
        I(b);                                                                  \
        I(c);                                                                  \
        I(d);                                                                  \
        I(e);                                                                  \
        I(f);                                                                  \
        I(g);                                                                  \
        I(h);                                                                  \
    } while (0)
#define ARGS_INVOKE_FOR_EACH_8(I, a, b, c, d, e, f, g)                         \
    do                                                                         \
    {                                                                          \
        I(a);                                                                  \
        I(b);                                                                  \
        I(c);                                                                  \
        I(d);                                                                  \
        I(e);                                                                  \
        I(f);                                                                  \
        I(g);                                                                  \
    } while (0)
#define ARGS_INVOKE_FOR_EACH_7(I, a, b, c, d, e, f)                            \
    do                                                                         \
    {                                                                          \
        I(a);                                                                  \
        I(b);                                                                  \
        I(c);                                                                  \
        I(d);                                                                  \
        I(e);                                                                  \
        I(f);                                                                  \
    } while (0)
#define ARGS_INVOKE_FOR_EACH_6(I, a, b, c, d, e)                               \
    do                                                                         \
    {                                                                          \
        I(a);                                                                  \
        I(b);                                                                  \
        I(c);                                                                  \
        I(d);                                                                  \
        I(e);                                                                  \
    } while (0)
#define ARGS_INVOKE_FOR_EACH_5(I, a, b, c, d)                                  \
    do                                                                         \
    {                                                                          \
        I(a);                                                                  \
        I(b);                                                                  \
        I(c);                                                                  \
        I(d);                                                                  \
    } while (0)
#define ARGS_INVOKE_FOR_EACH_4(I, a, b, c)                                     \
    do                                                                         \
    {                                                                          \
        I(a);                                                                  \
        I(b);                                                                  \
        I(c);                                                                  \
    } while (0)
#define ARGS_INVOKE_FOR_EACH_3(I, a, b)                                        \
    do                                                                         \
    {                                                                          \
        I(a);                                                                  \
        I(b);                                                                  \
    } while (0)
#define ARGS_INVOKE_FOR_EACH_2(I, a)                                           \
    do                                                                         \
    {                                                                          \
        I(a);                                                                  \
    } while (0)

#define ARGS_INVOKE_FOR_EACH(...)                                              \
    CONCAT2(ARGS_INVOKE_FOR_EACH_, COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

#define STRINGIFY(...) #__VA_ARGS__
#define STRINGIFY2(...) STRINGIFY(__VA_ARGS__)

#define CONCAT(a, b) a##b
#define CONCAT2(a, b) CONCAT(a, b)

#define TWELVE_ARGUMENT(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12,     \
                        ...)                                                   \
    a12

#define ELEVEN_ARGUMENT(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, ...) a11

#define COUNT_ARGS(...)                                                        \
    ELEVEN_ARGUMENT(dummy, ##__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define TEMPLATE_INSTANCE(func, ...)                                           \
    constexpr const auto CONCAT2(privptr, __LINE__) = &func<__VA_ARGS__>;

#endif
