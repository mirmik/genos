/**
    @file
*/

#include <comm/Notify.h>

std::recursive_mutex notifymtx;

/**
 * Поиск темы по номеру и индексам.
 * @param tag - Номер темы. (см. themestags.h)
 * @param vect - Вектор индексов (@example: CHANNEL1:AXIS0 - {1,0})
 * @return Указатель на объект темы.
 */
NotifyTheme *NotifyBrocker::themeFind(int tag, const std::vector<int32_t> &vect)
{
    std::lock_guard<std::recursive_mutex> lock(notifymtx);
    if (themes.find(std::make_pair(tag, vect)) == themes.end())
        return nullptr;
    return themes[std::make_pair(tag, vect)];
}

/**
 * Регистрация темы.
 * @param theme - Указатель на регистрируемую тему.
 * @param tag - Номер темы. (см. themestags.h)
 * @param vect - Вектор индексов (@example: CHANNEL1:AXIS0 - {1,0})
 */
void NotifyBrocker::themeRegistry(NotifyTheme *theme,
                                  int32_t tag,
                                  const std::vector<int32_t> &vect)
{
    std::lock_guard<std::recursive_mutex> lock(notifymtx);
    themes[std::make_pair(tag, vect)] = theme;
}

void NotifyBrocker::themeRegistry(std::unique_ptr<NotifyTheme> &theme,
                                  int32_t tag,
                                  const std::vector<int32_t> &vect)
{
    themeRegistry(theme.get(), tag, vect);
}

NotifyBrocker brocker;
