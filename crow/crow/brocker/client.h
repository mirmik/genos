/** @file */

#ifndef CROWKER_SUBSCRIBER_H
#define CROWKER_SUBSCRIBER_H

#include "options.h"
#include "theme.h"
#include <igris/buffer.h>
#include <mutex>
#include <nos/log.h>
#include <unordered_map>

namespace crow
{
    class crowker;
}

namespace crowker_implementation
{
    class client
    {
    private:
        std::string _name = {};
        std::unordered_map<theme *, options> _thms = {};
        int64_t _activity_timestamp = {};
        bool _confirmation = false;
        mutable std::recursive_mutex mtx = {};

    public:
        client();
        client(const client &) = delete;
        client &operator=(const client &) = delete;

        // bool is_disconnected();
        virtual ~client();
        virtual void publish(const std::string &theme,
                             const std::string &data,
                             options opts) = 0;

        void set_confirmed(bool en)
        {
            _confirmation = en;
        }

        bool is_confirmed() const
        {
            return _confirmation;
        }

        void set_name(nos::buffer name)
        {
            _name = std::string(name.data(), name.size());
        }

        const std::string &name() const
        {
            return _name;
        }

        void detach_from_themes();
        void forgot_theme(theme *thm);
        options get_theme_options(theme *thm);
        void set_activity_timestamp(int64_t ts)
        {
            _activity_timestamp = ts;
        }

        int64_t activity_timestamp() const
        {
            return _activity_timestamp;
        }

        void set_theme_options(theme *thm, options opts)
        {
            std::lock_guard<std::recursive_mutex> guard(mtx);
            _thms[thm] = opts;
        }

        void set_theme_options(const std::string &thm, options opts)
        {
            std::lock_guard<std::recursive_mutex> guard(mtx);
            for (auto &it : _thms)
            {
                if (it.first->name() == thm)
                {
                    it.second = opts;
                    return;
                }
            }
        }

        options get_theme_options(theme *thm) const
        {
            std::lock_guard<std::recursive_mutex> guard(mtx);
            if (_thms.count(thm) == 0)
                throw std::runtime_error("Theme not found");
            return _thms.at(thm);
        }

        options get_theme_options(const std::string &thm) const
        {
            std::lock_guard<std::recursive_mutex> guard(mtx);
            for (auto &it : _thms)
            {
                if (it.first->name() == thm)
                    return it.second;
            }
            throw std::runtime_error("Theme not found");
        }
    };
}

#endif