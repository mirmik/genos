#ifndef nos_TRENT_BINDER_H
#define nos_TRENT_BINDER_H

/**
    @file
*/

#include <nos/trent/settings.h>
#include <nos/trent/trent.h>

namespace nos
{
    template <typename T> struct trent_binder_ops
    {
    };

    template <typename Integer> struct trent_binder_ops_integer
    {
        static Integer from_trent(const nos::trent &tr)
        {
            return tr.as_integer_except();
        }
        static void set_trent(nos::trent &tr, const Integer &val)
        {
            tr = val;
        }
    };

    template <typename Float> struct trent_binder_ops_float
    {
        static Float from_trent(const nos::trent &tr)
        {
            return tr.as_numer_except();
        }
        static void set_trent(nos::trent &tr, const Float &val)
        {
            tr = val;
        }
    };

    template <>
    struct trent_binder_ops<int32_t> : public trent_binder_ops_integer<int32_t>
    {
    };
    template <>
    struct trent_binder_ops<int64_t> : public trent_binder_ops_integer<int64_t>
    {
    };

    template <>
    struct trent_binder_ops<float> : public trent_binder_ops_float<float>
    {
    };
    template <>
    struct trent_binder_ops<double> : public trent_binder_ops_float<double>
    {
    };

    template <> struct trent_binder_ops<std::string>
    {
        static std::string from_trent(const nos::trent &tr)
        {
            return tr.as_string_except();
        }
        static void set_trent(nos::trent &tr, const std::string &val)
        {
            tr = val;
        }
    };

    template <typename T> class trent_binder
    {
        mutable trent_syncer_slice saver = {};
        trent_binder_ops<T> ops = {};

        mutable bool synced = false;
        mutable T _local = {};

    public:
        trent_binder() = default;

        template <typename... Args>
        trent_binder(Args &&... args) : saver(std::forward<Args>(args)...)
        {
            sync();
        }

        template <typename... Args> void init(Args &&... args)
        {
            saver.init(args...);
        }

        void sync() const
        {
            if (!saver.good())
                return;

            saver.sync();
            _local = ops.from_trent(saver.node());
            synced = true;
        }

        void sync_default(T defa)
        {
            if (!saver.good())
                return;

            saver.sync();

            if (saver.node().is_nil())
            {
                update(defa);
                synced = true;
                return;
            }

            _local = ops.from_trent(saver.node());
            synced = true;
        }

        void update(const T &val)
        {
            if (val == _local)
                return;

            _local = val;
            ops.set_trent(saver.node(), _local);
            saver.save();
        }

        T get() const
        {
            if (!saver.good())
                return _local;

            if (!synced)
            {
                sync();
            }

            return _local;
        }
    };
}

#endif
