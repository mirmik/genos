/** @file */

#include "theme.h"
#include "client.h"
#include "crowker.h"
#include <chrono>
#include <igris/dprint.h>
#include <igris/math.h>
#include <nos/log.h>
#include <nos/print.h>

crowker_implementation::theme::theme(size_t queue_size)
{
    _last_messages.resize(queue_size);
}

int64_t crowker_eval_timestamp()
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return millis;
}

std::vector<std::shared_ptr<std::string>>
crowker_implementation::theme::get_latest(uint32_t count)
{
    std::vector<std::shared_ptr<std::string>> arr;
    uint32_t size = __MIN__(count, _last_messages.size());
    for (size_t i = 0; i < size; ++i)
    {
        arr.push_back(_last_messages[i]);
    }
    return arr;
}

void crowker_implementation::theme::publish(
    const std::shared_ptr<std::string> &data)
{
    std::vector<client *> killme_list;

    {
        std::lock_guard<std::mutex> lock(mtx);
        _last_messages.push(data);
    }

    for (auto *sub : _subs)
    {
        crowker_implementation::options opts = sub->get_theme_options(this);

        if (opts.qos == 0 &&
            crowker_eval_timestamp() - sub->activity_timestamp() > 20000)
        {
            killme_list.push_back(sub);
        }
        else
        {
            sub->publish(name(), {data->data(), data->size()}, opts);
        }
    }

    for (auto *sub : killme_list)
    {
        crow::crowker::instance()->unlink_theme_client(this, sub);
    }

    _publish_timestamp = _activity_timestamp = crowker_eval_timestamp();
}

void crowker_implementation::theme::unlink_client(client *sub)
{
    if (_subs.count(sub) == 0)
    {
        nos::log::warn("try unlink unregistred client:{} theme:{}", sub->name(),
                       name());
        return;
    }

    nos::fprintln("unlink client:{} theme:{}", sub->name(), name());
    _subs.erase(sub);
}