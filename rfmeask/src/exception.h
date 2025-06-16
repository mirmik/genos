/**
    @file
*/

#ifndef RFMEAS_EXCEPTION_H
#define RFMEAS_EXCEPTION_H

#include <cstdio>
#include <exception>
#include <igris/util/location.h>
#include <string>

struct ProjectException : public std::exception
{
    std::string str = {};

    ProjectException(const char *str) : std::exception(), str(str){};
    ProjectException(const std::string &str) : std::exception(), str(str){};
    ProjectException() : std::exception(){};

    const char *what() const noexcept override
    {
        return str.c_str();
    }
};

struct FunctionIsNotReleased : std::exception
{
    struct location loc = {};
    FunctionIsNotReleased(const struct location &loc) : loc(loc) {}
    const char *what() const noexcept override
    {
        static char exception_buffer[128];
        sprintf(exception_buffer,
                "\r\nfunc: %s\r\nfile: %s\r\nline: %d\r\n",
                loc.func,
                loc.file,
                loc.line);
        return exception_buffer;
    }
};

struct FunctionIsNotSupported : public ProjectException
{
    FunctionIsNotSupported(const char *str) : ProjectException(str) {}
    FunctionIsNotSupported() : ProjectException() {}
};

struct AxisException : public ProjectException
{
};

struct AxisUnreadyException : public AxisException
{
};

struct AxisOperationTimeException : public AxisException
{
};

struct AxisLimitException : public AxisException
{
};

struct DeviceException : public ProjectException
{
    DeviceException(const char *str) : ProjectException(str) {}
    DeviceException(const std::string &str) : ProjectException(str) {}
    DeviceException() : ProjectException() {}
};

struct DeviceAlarmException : public DeviceException
{
};

struct DeviceRefuseException : public DeviceException
{
    DeviceRefuseException() : DeviceException("DeviceRefuseException") {}
    DeviceRefuseException(const std::string &a) : DeviceException(a){};
};

struct CommunicationException : public ProjectException
{
};

struct AxisHookPreventionException : public ProjectException
{
};

struct GlobalHookPreventionException : public ProjectException
{
};

struct WrongValueException : public CommunicationException
{
};

#define FUNCTION_IS_NOT_RELEASED                                               \
    {                                                                          \
        CURRENT_LOCATION(loc);                                                 \
        throw FunctionIsNotReleased(loc);                                      \
    }

static constexpr int UPDATE_PARAMETR_ERROR_NOCONNECTED_DEVICE = -2;
static constexpr int UPDATE_PARAMETR_ERROR_STATION_CODE = -1;
static constexpr int UPDATE_PARAMETR_NEED_RESTART = 1;
static constexpr int UPDATE_PARAMETR_DONTNEEDED_RESTART = 0;

#endif
