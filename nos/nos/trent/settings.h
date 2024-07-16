#ifndef nos_TRENT_SETTINGS_H
#define nos_TRENT_SETTINGS_H

/**
    @file
*/

#include <nos/trent/trent.h>
#include <nos/trent/trent_path.h>

namespace nos
{
    class trent_syncer
    {
    public:
        virtual int sync() = 0;
        virtual int save() = 0;
        virtual nos::trent &node() = 0;
        virtual bool good() const = 0;
        virtual ~trent_syncer() = default;
    };

    class trent_syncer_slice : public trent_syncer
    {
    public:
        nos::trent_syncer *syncer = nullptr;
        nos::trent_path path = {};

        trent_syncer_slice(nos::trent_syncer &stgs, const nos::trent_path &path)
            : syncer(&stgs), path(path)
        {
        }

        trent_syncer_slice() = default;
        trent_syncer_slice(const trent_syncer_slice &) = default;
        trent_syncer_slice &operator=(const trent_syncer_slice &) = default;

        void init(nos::trent_syncer &stgs, const nos::trent_path &path)
        {
            syncer = &stgs;
            this->path = path;
        }

        bool good() const
        {
            return (syncer != nullptr) && syncer->good();
        }

        int sync() override
        {
            return syncer->sync();
        }

        int save() override
        {
            return syncer->save();
        }

        nos::trent &node() override
        {
            return syncer->node()[path];
        }
    };

    class trent_settings
    {
    public:
        nos::trent tr = {};
        bool synced = false;

        virtual void sync() = 0;
        virtual void save() = 0;

        virtual bool good() const = 0;

        nos::trent &node()
        {
            return tr;
        }
        const nos::trent &node() const
        {
            return tr;
        }
        virtual ~trent_settings() = default;
    };

    class trent_settings_slice : public trent_settings
    {
    public:
        nos::trent_settings &settings;
        nos::trent_path path = {};

        trent_settings_slice(nos::trent_settings &stgs,
                             const nos::trent_path &path)
            : settings(stgs), path(path)
        {
        }

        operator bool() const
        {
            return good();
        }

        void sync() override
        {
            if (!settings.synced)
            {
                settings.sync();
            }
            tr = settings.node()[path];
            synced = true;
        }

        void save() override
        {
            settings.node()[path] = tr;
            settings.save();
        }
    };

    class settings_binder_int64 : public trent_settings_slice
    {
    public:
        settings_binder_int64(trent_settings &base, const trent_path &name)
            : trent_settings_slice(base, name)
        {
        }

        void sync_default(int64_t def)
        {
            sync();
            if (node().is_nil())
                node() = def;
        }

        settings_binder_int64 &operator=(int64_t i)
        {
            node() = i;
            return *this;
        }

        operator int64_t() const
        {
            return node().as_integer_except();
        }
    };

    class settings_binder_int32 : public trent_settings_slice
    {
    public:
        settings_binder_int32(trent_settings &base, const trent_path &name)
            : trent_settings_slice(base, name)
        {
        }

        void sync_default(int32_t def)
        {
            sync();
            if (node().is_nil())
                node() = def;
        }

        settings_binder_int32 &operator=(int32_t i)
        {
            node() = i;
            return *this;
        }

        operator int32_t() const
        {
            return node().as_integer_except();
        }
    };

}

#endif
