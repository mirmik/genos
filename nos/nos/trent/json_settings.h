#ifndef nos_JSON_SETTINGS_H
#define nos_JSON_SETTINGS_H

/**
    @file
*/

#include <fstream>
#include <sstream>
#include <string>

#include <exception>
#include <fcntl.h>
#include <nos/trent/json.h>
#include <nos/trent/json_print.h>
#include <nos/trent/settings.h>

namespace nos
{
    class json_settings : public trent_settings
    {
        std::string pathstr = {};

    public:
        const std::string &path()
        {
            return pathstr;
        }
        void path(const std::string &str)
        {
            pathstr = str;
        }

        json_settings() = default;
        json_settings(const std::string &str) : pathstr(str){};

    private:
        void load()
        {
            std::fstream file(pathstr);
            if (!file.good())
            {
                perror("json_settings::load:");
                return;
            }
            std::stringstream file_contents;
            file_contents << file.rdbuf();
            tr = json::parser_str(file_contents.str()).parse();
            file.close();
            synced = true;
        }

    public:
        void sync() override
        {
            load();
        }
        bool good() const override
        {
            return synced;
        }

        void save() override
        {
            std::fstream file(pathstr, std::ios::out);
            json::pretty_print_to(tr, file);
        }
    };

    class json_syncer : public trent_syncer
    {
        nos::trent tr = {};
        bool synced = false;
        std::string pathstr = {};

    public:
        const std::string &path()
        {
            return pathstr;
        }
        void path(const std::string &str)
        {
            pathstr = str;
        }

        json_syncer() = default;
        json_syncer(const std::string &str) : pathstr(str) {}

        bool good() const override
        {
            return synced;
        }

    private:
        void load()
        {
            std::fstream file(pathstr);
            if (!file.good())
            {
                perror("json_syncer::load:");
                throw std::logic_error("file isn't exist");
            }
            std::stringstream file_contents;
            file_contents << file.rdbuf();
            tr = json::parser_str(file_contents.str()).parse();
            file.close();
            synced = true;
        }

    public:
        int sync() override
        {
            if (synced)
                return 0;
            load();
            return 0;
        }

        int save() override
        {
            std::fstream file(pathstr, std::ios::out);
            json::pretty_print_to(tr, file);
            return 0;
        }

        nos::trent &node() override
        {
            return tr;
        }
    };
}

#endif
