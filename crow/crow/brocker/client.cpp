/** @file */

#include "client.h"
#include "crowker.h"
#include <nos/log.h>

crowker_implementation::client::~client()
{
    detach_from_themes();
}

crowker_implementation::client::client()
{
    // name is random string
    _name = std::to_string((uint64_t)this);
}

void crowker_implementation::client::detach_from_themes()
{
    std::lock_guard<std::recursive_mutex> guard(mtx);
    while (!_thms.empty())
    {
        auto it = _thms.begin();
        crow::crowker::instance()->unlink_theme_client(it->first, this);
    }
    _thms.clear();
}

void crowker_implementation::client::forgot_theme(theme *thm)
{
    std::lock_guard<std::recursive_mutex> guard(mtx);
    _thms.erase(thm);
}

crowker_implementation::options
crowker_implementation::client::get_theme_options(theme *thm)
{
    std::lock_guard<std::recursive_mutex> guard(mtx);
    if (_thms.count(thm) == 0)
    {
        throw std::runtime_error("Theme not found");
    }
    return _thms[thm];
}