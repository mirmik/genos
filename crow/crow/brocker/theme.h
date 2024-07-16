/** @file */

#ifndef CROWKER_THEME_H
#define CROWKER_THEME_H

#include <crow/defs.h>
#include <deque>
#include <igris/container/cyclic_buffer.h>
#include <memory>
#include <mutex>
#include <nos/fprint.h>
#include <set>
#include <string>

namespace crowker_implementation
{
    class client;

    class theme
    {
    private:
        std::string _name = {};
        std::set<client *> _subs = {};
        int64_t _publish_timestamp = 0;
        int64_t _activity_timestamp = 0;

        std::mutex mtx = {};
        igris::cyclic_buffer<std::shared_ptr<std::string>> _last_messages{
            CROW_DEFAULT_QUEUE_SIZE};

    public:
        theme(size_t queue_size);
        theme(const theme &) = delete;

        std::mutex &local_lock()
        {
            return mtx;
        }

        size_t count_clients()
        {
            return _subs.size();
        }

        std::vector<std::shared_ptr<std::string>> get_latest(uint32_t count);

        bool link_client(client *sub)
        {
            if (_subs.count(sub) == 0)
            {
                _subs.insert(sub);
                return true;
            }

            return false;
        }

        bool has_client(client *sub)
        {
            return _subs.count(sub);
        }

        void unlink_client(client *sub);
        void publish(const std::shared_ptr<std::string> &data);

        void set_name(const std::string &name)
        {
            _name = name;
        }

        const std::string &name() const
        {
            return _name;
        }

        int64_t publish_timestamp() const
        {
            return _publish_timestamp;
        }

        int64_t activity_timestamp() const
        {
            return _activity_timestamp;
        }

        void set_activity_timestamp(int64_t ts)
        {
            _activity_timestamp = ts;
        }

        void set_publish_timestamp(int64_t ts)
        {
            _publish_timestamp = ts;
        }

        void set_queue_size(size_t sz)
        {
            _last_messages.resize(sz);
        }

        size_t queue_size() const
        {
            return _last_messages.size();
        }

        void resize_queue(size_t sz)
        {
            _last_messages.resize(sz);
        }

        const igris::cyclic_buffer<std::shared_ptr<std::string>> &queue() const
        {
            return _last_messages;
        }
    };
}

#endif