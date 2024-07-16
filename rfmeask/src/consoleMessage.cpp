/**
    @file
*/

#include <consoleMessage.h>
#include <logging.h>

void debugErrorRefuse(Device *dev)
{
    nos::log::info("Error::ConnectionRefuse: {}", dev->name());
}

void debugErrorAlarm(Device *dev)
{
    // TODO: Это сообщение спамится в терминал без остановки. Вероятно, оно
    // вообще не нужно.
    // nos::log::info("Error::ConnectionAlarm");
}

void consuccess(Device *dev)
{
    nos::log::info("ConnectionSuccess: {}", dev->name());
}
