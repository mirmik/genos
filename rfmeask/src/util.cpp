
#include <util.h>

// get user directory for unix and windows
std::string get_user_dir()
{
    std::string user_dir;
#if defined(_WIN32)
    char *userprofile = getenv("USERPROFILE");
    if (userprofile != nullptr)
    {
        user_dir = userprofile;
    }
    else
    {
        user_dir = ".";
    }
#else
    char *home = getenv("HOME");
    if (home != nullptr)
    {
        user_dir = home;
    }
    else
    {
        user_dir = ".";
    }
#endif
    return user_dir;
}