/**
    @file
*/

#include "DevAlarmLogger.h"
#include <chrono>
#include <comm/Notify.h>
#include <ctime>
#include <devices/Device.h>

void DevAlarmLogger::init(const char *filename)
{
    char str[256];
    char timeStr[25];
    std::time_t currTime =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::memcpy(timeStr, std::ctime(&currTime), 24);
    timeStr[24] = 0;
    logFile.open(filename,
                 std::ios_base::app | std::ios_base::out | std::ios_base::ate);
    snprintf(str, sizeof(str), "[%s] Session started.\r\n", timeStr);
    logFile.write(str, strlen(str));
    logFile.flush();
}

void DevAlarmLogger::logAlarm(const std::string &alarmStr, Device *dev)
{
    char str[256];
    char timeStr[25];
    std::time_t currTime =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::memcpy(timeStr, std::ctime(&currTime), 24);
    timeStr[24] = 0;
    snprintf(str,
             sizeof(str),
             "[%s] Device: %s %s\r\n",
             timeStr,
             dev->name().c_str(),
             alarmStr.c_str());
    int length = strlen(str);

    if (logFile)
    {
        if (logFile.is_open())
        {
            logFile.write(str, length);
            logFile.flush();
        }
    }
    messageBusNotify->notify(str, length);
}
