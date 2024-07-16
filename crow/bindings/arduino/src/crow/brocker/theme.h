/** @file */

#ifndef CROWKER_THEME_H
#define CROWKER_THEME_H

#include <memory>
#include <set>
#include <string>

namespace crowker_implementation
{
    class subscriber;

    class theme
    {
    public:
        std::string name;
        std::set<subscriber *> subs;
        int64_t publish_timestamp;
        int64_t activity_timestamp;

    public:
        size_t count_subscribers()
        {
            return subs.size();
        }

        void link_subscriber(subscriber *sub)
        {
            subs.insert(sub);
        }

        bool has_subscriber(subscriber *sub)
        {
            return subs.count(sub);
        }

        void unlink_subscriber(subscriber *sub)
        {
            subs.erase(sub);
        }

        void publish(const std::string &data);
    };
} // namespace crowker_implementation

#endif