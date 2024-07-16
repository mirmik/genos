/**
    @file
*/

#ifndef DEV_ALARM_LOGGER_H
#define DEV_ALARM_LOGGER_H

#include <fstream>

class Device;

/**
    @brief Класс для записи ошибок полученных от устройств
    в процессе их работы в отдельный файл.
*/
class DevAlarmLogger
{
public:
    DevAlarmLogger(){};
    void init(const char *filename);
    void logAlarm(const std::string &alarmStr, Device *dev);

private:
    std::ofstream logFile = {};
};

#endif
