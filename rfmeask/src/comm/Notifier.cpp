/**
    @file
*/

#include <comm/Notify.h>
#include <igris/sync/syslock.h>
#include <nos/inet/socket.h>
#include <nos/print.h>
#include <string.h>

void NotifierBasic::removeAllThemes()
{
    std::lock_guard<std::recursive_mutex> lock(notifymtx);
    while (!themes.empty())
        (*themes.begin())->removeClient(const_cast<NotifierBasic *>(this));
    themes.clear();
}

std::string NotifierNCPI::getIdentifier()
{
    char buf[48];
    sprintf(buf, "NotifierNCPI(fd:%d)", (int)sock.fd());
    return std::string(buf);
}

void NotifierNCPI::notify(NotifyTheme *theme)
{
    std::lock_guard<std::recursive_mutex> lock(notifymtx);
    char buf[48];
    sprintf(buf,
            "%.*s\n",
            (int)theme->ncpi_header.size(),
            theme->ncpi_header.c_str());
    sock.write(buf, strlen(buf));
}

void NotifierNCPI::notify(NotifyTheme *theme, int32_t arg)
{
    std::lock_guard<std::recursive_mutex> lock(notifymtx);
    char buf[48];
    sprintf(buf,
            "%.*s %d\n",
            (int)theme->ncpi_header.size(),
            theme->ncpi_header.c_str(),
            arg);
    sock.write(buf, strlen(buf));
}

void NotifierNCPI::notify(NotifyTheme *theme, double arg)
{
    std::lock_guard<std::recursive_mutex> lock(notifymtx);
    char buf[48];
    sprintf(buf,
            "%.*s %e\n",
            (int)theme->ncpi_header.size(),
            theme->ncpi_header.c_str(),
            arg);
    sock.write(buf, strlen(buf));
}

void NotifierNCPI::notify(NotifyTheme *theme, std::vector<int32_t> vec)
{
    std::lock_guard<std::recursive_mutex> lock(notifymtx);
    std::string msg = nos::format("{} {}\n", theme->ncpi_header, vec);
    sock.write(msg.data(), msg.size());
}

void NotifierNCPI::notify(NotifyTheme *theme, const char *str, int length)
{
    std::lock_guard<std::recursive_mutex> lock(notifymtx);
    char buf[512];
    sprintf(buf,
            "%.*s %.*s\n",
            (int)theme->ncpi_header.size(),
            theme->ncpi_header.c_str(),
            length,
            str);
    sock.write(buf, strlen(buf));
}
