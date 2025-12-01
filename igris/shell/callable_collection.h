#ifndef CALLABLE_COLLECTION_H
#define CALLABLE_COLLECTION_H

#include <functional>
#include <igris/container/static_vector.h>
#include <initializer_list>
#include <string>
#include <vector>

namespace igris
{
    template <class F> struct callable_collection_record
    {
        const char *key;
        const char *help;
        std::function<F> func;
    };

    template <class F, size_t N> class static_callable_collection
    {
        using Container =
            igris::static_vector<callable_collection_record<F>, N>;

        Container records = {};

    public:
        static_callable_collection() = default;
        static_callable_collection(const static_callable_collection &) =
            default;
        static_callable_collection(static_callable_collection &&) = default;
        static_callable_collection &
        operator=(const static_callable_collection &) = default;
        static_callable_collection &
        operator=(static_callable_collection &&) = default;

        static_callable_collection(Container &&recs)
        {
            records = std::move(recs);
        }

        static_callable_collection(const Container &recs)
        {
            records = recs;
        }

        static_callable_collection(
            const std::initializer_list<callable_collection_record<F>> &recs)
        {
            records = recs;
        }

        template <class U>
        void add(const std::string &key, const std::string &help, U func)
        {
            records.push_back({key, help, std::function<F>(func)});
        }

        template <class U> void add(const std::string &key, U func)
        {
            records.push_back({key, "", std::function<F>(func)});
        }

        template <class U> void add(const char *key, const char *help, U func)
        {
            records.push_back({key, help, std::function<F>(func)});
        }

        template <class U> void add(const char *key, U func)
        {
            records.push_back({key, "", std::function<F>(func)});
        }

        void add(callable_collection_record<F> &&rec)
        {
            records.push_back(std::move(rec));
        }

        void add(callable_collection_record<F> &rec)
        {
            records.push_back(rec);
        }

        void add(Container &&recs)
        {
            records.insert(records.end(), recs.begin(), recs.end());
        }

        void add(Container &recs)
        {
            records.insert(records.end(), recs.begin(), recs.end());
        }

        void add(static_callable_collection &&recs)
        {
            records.insert(records.end(), recs.begin(), recs.end());
        }

        void add(static_callable_collection &recs)
        {
            records.insert(records.end(), recs.begin(), recs.end());
        }

        auto begin()
        {
            return records.begin();
        }
        auto end()
        {
            return records.end();
        }
        auto begin() const
        {
            return records.begin();
        }
        auto end() const
        {
            return records.end();
        }

        std::function<F> *find(const char *key)
        {
            for (auto &rec : records)
            {
                if (strcmp(rec.key, key) == 0)
                    return &rec.func;
            }
            return nullptr;
        }
    };
}

#endif