/** @file */

#ifndef NOS_CONVERT_COLOR
#define NOS_CONVERT_COLOR

#include <string>
#include <nos/print.h>
#include <nos/fprint.h>

namespace nos
{
    static inline std::string black         (const std::string str) { return nos::format("\x1B[30;1m{}\x1B[0m", str); } 
    static inline std::string red           (const std::string str) { return nos::format("\x1B[31;1m{}\x1B[0m", str); } 
    static inline std::string green         (const std::string str) { return nos::format("\x1B[32;1m{}\x1B[0m", str); } 
    static inline std::string yellow        (const std::string str) { return nos::format("\x1B[33;1m{}\x1B[0m", str); } 
    static inline std::string blue          (const std::string str) { return nos::format("\x1B[34;1m{}\x1B[0m", str); } 
    static inline std::string magenta       (const std::string str) { return nos::format("\x1B[35;1m{}\x1B[0m", str); } 
    static inline std::string cyan          (const std::string str) { return nos::format("\x1B[36;1m{}\x1B[0m", str); } 
    static inline std::string white         (const std::string str) { return nos::format("\x1B[37;1m{}\x1B[0m", str); } 
    static inline std::string dark_black    (const std::string str) { return nos::format("\x1B[30m{}\x1B[0m", str); }   
    static inline std::string dark_red      (const std::string str) { return nos::format("\x1B[31m{}\x1B[0m", str); }   
    static inline std::string dark_green    (const std::string str) { return nos::format("\x1B[32m{}\x1B[0m", str); }   
    static inline std::string dark_yellow   (const std::string str) { return nos::format("\x1B[33m{}\x1B[0m", str); }   
    static inline std::string dark_blue     (const std::string str) { return nos::format("\x1B[34m{}\x1B[0m", str); }   
    static inline std::string dark_magenta  (const std::string str) { return nos::format("\x1B[35m{}\x1B[0m", str); }   
    static inline std::string dark_cyan     (const std::string str) { return nos::format("\x1B[36m{}\x1B[0m", str); }   
    static inline std::string dark_white    (const std::string str) { return nos::format("\x1B[37m{}\x1B[0m", str); }
}

#define COLOR_BLACK(str)                "\x1B[30;1m" str "\x1B[0m"
#define COLOR_RED(str)                  "\x1B[31;1m" str "\x1B[0m"
#define COLOR_GREEN(str)                "\x1B[32;1m" str "\x1B[0m"
#define COLOR_YELLOW(str)               "\x1B[33;1m" str "\x1B[0m"
#define COLOR_BLUE(str)                 "\x1B[34;1m" str "\x1B[0m"
#define COLOR_MAGENTA(str)              "\x1B[35;1m" str "\x1B[0m"
#define COLOR_CYAN(str)                 "\x1B[36;1m" str "\x1B[0m"
#define COLOR_WHITE(str)                "\x1B[37;1m" str "\x1B[0m"
#define COLOR_DARK_BLACK(str)           "\x1B[30m" str "\x1B[0m"
#define COLOR_DARK_RED(str)             "\x1B[31m" str "\x1B[0m"
#define COLOR_DARK_GREEN(str)           "\x1B[32m" str "\x1B[0m"
#define COLOR_DARK_YELLOW(str)          "\x1B[33m" str "\x1B[0m"
#define COLOR_DARK_BLUE(str)            "\x1B[34m" str "\x1B[0m"
#define COLOR_DARK_MAGENTA(str)         "\x1B[35m" str "\x1B[0m"
#define COLOR_DARK_CYAN(str)            "\x1B[36m" str "\x1B[0m"
#define COLOR_DARK_WHITE(str)           "\x1B[37m" str "\x1B[0m"

namespace nos 
{
    static void reset_terminal() 
    {
        nos::print("\x1B[0m");
    }
}

#endif
