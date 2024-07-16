/**
@file Сообщения об ошибках в Device.
*/

#ifndef CONSOLE_MESSAGE_H
#define CONSOLE_MESSAGE_H

#include <devices/Device.h>

void debugErrorRefuse(Device *dev);
void debugErrorAlarm(Device *dev);
void consuccess(Device *dev);
void systemReadyPrint();
void systemUnreadyPrint();

#endif
