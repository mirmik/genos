#ifndef VERSION_H
#define VERSION_H

const int API_VERSION = 250;
const std::string VERSION = "1.1.1";


// 200 - добавлена поддержка systemd
// 250 / 1.0.0 - рефактор легаси сервера
// 250 / 1.1.0 - фикс бага с некорректным обновлением статуса проектов в systemd
// 250 / 1.1.1 - фикс утечки файловых дескрипторов

#endif