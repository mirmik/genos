#ifndef NOTIFY_THEME_H
#define NOTIFY_THEME_H

/**
    @file
    Класс темы уведомлений. Используется для сигнализации удаленных подписчиков
   о событиях
*/
#include <igris/util/bug.h>
#include <logging.h>
#include <map>
#include <memory>
#include <mutex>
#include <nos/inet/tcp_client.h>
#include <nos/log/logger.h>
#include <set>
#include <stdint.h>
#include <string>
#include <string_view>
#include <vector>

extern std::recursive_mutex notifymtx;

enum SmoothType : uint8_t
{
    NONE,
    TIMERED,
    SMOOTH
};

class NotifyTheme;
class ThemePrivateData;

/**	NotifyBrocker - Класс, в котором храниться информация о имеющихся
    в системе темах. */
class NotifyBrocker
{
    nos::log::logger logger = {};

private:
    /**Для регистрации тем используеюся тэг темы и поясняющие номера (
    на случай, если один и тот-же тег используют многие объекты.) Уступка NCPI*/
    std::map<std::pair<int, std::vector<int32_t>>, NotifyTheme *> themes = {};

public:
    void themeRegistry(NotifyTheme *theme,
                       int32_t tag,
                       const std::vector<int32_t> &vect);
    void themeRegistry(std::unique_ptr<NotifyTheme> &theme,
                       int32_t tag,
                       const std::vector<int32_t> &vect);

    NotifyTheme *themeFind(int tag, const std::vector<int32_t> &vect);

    NotifyBrocker() : logger("NotifyBrocker") {}
};

/** Базовый подписчик на тему. Предок всех подписчиков */
class NotifierBasic
{
public:
    std::map<NotifyTheme *, ThemePrivateData *> themesPrivateData = {};

public:
    std::set<NotifyTheme *> themes = {};
    // std::mutex notifymtx;

    NotifierBasic() = default;
    virtual ~NotifierBasic() = default;

    //Каждый тип подписчика сам описывает метод уведомления.
    virtual void notify(NotifyTheme *theme) = 0;
    virtual void notify(NotifyTheme *theme, int32_t arg) = 0;
    virtual void notify(NotifyTheme *theme, double arg) = 0;
    virtual void notify(NotifyTheme *theme, std::vector<int32_t> arg) = 0;
    virtual void notify(NotifyTheme *theme, const char *buf, int length) = 0;
    void notify(NotifyTheme *theme, std::string_view str)
    {
        notify(theme, str.data(), str.size());
    }

    virtual std::string getIdentifier() = 0;

    void removeAllThemes();
};

/*Подписччик протокола NCPI*/
class NotifierNCPI : public NotifierBasic
{
public:
    nos::inet::tcp_client sock = {};

public:
    NotifierNCPI() = default;
    virtual ~NotifierNCPI() = default;

    void notify(NotifyTheme *theme) override;
    void notify(NotifyTheme *theme, int32_t arg) override;
    void notify(NotifyTheme *theme, double arg) override;
    void notify(NotifyTheme *theme, std::vector<int32_t> arg) override;
    void notify(NotifyTheme *theme, const char *buf, int length) override;

    std::string getIdentifier() override;
};

/**
    Для некоторых тем может быть необходимо хранить контекстную информацию,
    Специфичную для каждого клиента.
*/
class ThemePrivateData
{
public:
    uint64_t count = 0;
    virtual ~ThemePrivateData() {}
};

//! Контекстная информация плавной темы.
class SmoothThemePrivateData : public ThemePrivateData
{
public:
    SmoothType type = SmoothType::NONE;
    double param = 0;
    double last = 0;

    ~SmoothThemePrivateData(){};
};

/*Тема уведомлений*/
class NotifyTheme
{
public:
    std::set<NotifierBasic *> notifiers = {};

    void setNCPIHeader(const char *str);
    void setNCPIHeader(const std::string &str);

public:
    std::string ncpi_header = {};
    std::string name = {};

    // CONSTREF_GETTER(getName, name)

    void setName(const std::string &str);
    void setName(const char *str);

    void setNCPIHeader(const std::string &__str, std::vector<int32_t> vect);

    void registryClient(NotifierBasic *client);
    void removeClient(NotifierBasic *client);
    bool clientIsRegistry(NotifierBasic *client);

    bool has_subscribers()
    {
        return notifiers.size() != 0;
    }

    /** Функции, отвечающие за рассылку уведомлений клиентам.
        Работа функций ограничивается вызовом соответствующего метода клиента и
        передачи ему указателя темы.
        Клиент в зависимости от своей природы, может запросить у темы
       информацию, необходимую для совершения уведомления.
    */
    template <class... Args> void notify(Args &&... args)
    {
        std::lock_guard<std::recursive_mutex> lock(notifymtx);
        for (auto c : notifiers)
        {
            c->notify(this, std::forward<Args>(args)...);
        }
    }
};

class NotifySmoothTheme : public NotifyTheme
{
public:
    void registryClient(NotifierBasic *client, SmoothType type, double param);

    template <class Arg> void notifySmooth(Arg &&arg, bool force)
    {
        std::lock_guard<std::recursive_mutex> lock(notifymtx);
        for (auto c : notifiers)
        {
            ThemePrivateData *__privdata =
                c->themesPrivateData.find(this)->second;
            SmoothThemePrivateData *privdata =
                (SmoothThemePrivateData *)__privdata;

            double current_time;

            switch (privdata->type)
            {
            case SmoothType::TIMERED:
                current_time =
                    std::chrono::system_clock::now().time_since_epoch().count();

                if (privdata->count == 0 ||
                    privdata->last - current_time >= privdata->param || force)
                {
                    privdata->last = current_time;
                    privdata->count++;
                    c->notify(this, std::forward<Arg>(arg));
                };

                break;
            case SmoothType::SMOOTH:

                if (privdata->count == 0 ||
                    std::fabs(privdata->last - arg) >= privdata->param || force)
                {
                    privdata->last = arg;
                    privdata->count++;
                    c->notify(this, std::forward<Arg>(arg));
                };

                break;

            case SmoothType::NONE:
                BUG();
            }
        }
    }
};

extern std::unique_ptr<NotifyTheme> messageBusNotify;
extern std::unique_ptr<NotifyTheme> messageChannelNotify;

#endif
