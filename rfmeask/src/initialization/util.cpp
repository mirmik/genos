#include <initialization/util.h>
#include <string>

std::string port_converter(std::string port)
{
    if (port == "COM1")
    {
        return "/dev/ttyS0";
    }
    else if (port == "COM2")
    {
        return "/dev/ttyS1";
    }
    else if (port == "COM3")
    {
        return "/dev/ttyS2";
    }
    else if (port == "COM4")
    {
        return "/dev/ttyS3";
    }
    else
    {
        return port;
    }
    return port;
}