/**
    @file
*/

#include <cmath>
#include <comm/Notify.h>
#include <comm/notifymap.h>

std::unique_ptr<NotifyTheme> messageBusNotify(new NotifyTheme);
std::unique_ptr<NotifyTheme> messageChannelNotify(new NotifyTheme);

/**	Хеадер NCPI будет передан при уведомлении через протокол NCPI.
    В общем случае хеадер может отличаться от комманды установки.
*/
void NotifyTheme::setNCPIHeader(const char *str)
{
    ncpi_header = str;
}

void NotifyTheme::setNCPIHeader(const std::string &str)
{
    ncpi_header = str;
}

/**	Вспомогательная функция, востанавливающая, исходя из номеров подсистем,
    заголовок по паттерну NCPI парсера.
*/
void NotifyTheme::setNCPIHeader(const std::string &__str,
                                std::vector<int32_t> vect)
{
    std::string str = __str;
    size_t sharp;

    //Вставляем вхождения вектора на места вилдкарт.
    for (auto i : vect)
    {
        sharp = str.find("#");
        str.replace(sharp, 1, std::to_string(i));
    }

    setNCPIHeader(str);
}

/** Подписка нового клиента на тему.
    При простой подписке информацию о конфигурации не сохраняется.
    @client новый клиент темы.
*/
void NotifyTheme::registryClient(NotifierBasic *client)
{
    std::lock_guard<std::recursive_mutex> lock(notifymtx);
    notifiers.insert(client);
    client->themes.insert(this);
}

/** Прекращение подписки клиента.
 */
void NotifyTheme::removeClient(NotifierBasic *client)
{
    std::lock_guard<std::recursive_mutex> lock(notifymtx);
    notifiers.erase(client);
    client->themes.erase(this);

    // Поиск и удаление (если есть) конфигурации подписки, сохраняемой темой в
    // теле клиента.
    if (client->themesPrivateData.find(this) != client->themesPrivateData.end())
    {
        delete client->themesPrivateData[this];
        client->themesPrivateData.erase(this);
    }
}

/*Проверка, зарегистрирован ли клиент в данной теме*/
bool NotifyTheme::clientIsRegistry(NotifierBasic *client)
{
    return notifiers.find(client) != notifiers.end();
}

/** Имя темы используется в основном в качестве отладочной информации */
void NotifyTheme::setName(const std::string &str)
{
    name = str;
}

void NotifyTheme::setName(const char *str)
{
    name = str;
}

/** Регистрация клиента плавной темы.
    В плавной теме сохраняется конфмгурация подписки, т.к.
    необходимость уведомления зависит от типа и истории уведомлений.
*/
void NotifySmoothTheme::registryClient(NotifierBasic *client,
                                       SmoothType type,
                                       double param)
{
    std::lock_guard<std::recursive_mutex> lock(notifymtx);
    notifiers.insert(client);
    client->themes.insert(this);

    SmoothThemePrivateData *data = new SmoothThemePrivateData;
    data->type = type;
    data->param = param;
    data->count = 0;

    client->themesPrivateData[this] = data;
}
