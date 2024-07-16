#ifndef NOS_ARGPARSE_H
#define NOS_ARGPARSE_H

#include <nos/fprint.h>
#include <nos/io/ostream.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace nos
{
    class argparse_node
    {
    public:
        argparse_node(const std::string &name,
                      const char letter,
                      const std::string &description,
                      const std::string &value = "",
                      bool required = false,
                      bool is_flag = false)
            : name(name), letter(letter), description(description),
              value(value), required(required), is_flag(is_flag)
        {
        }

        std::string name = {};
        char letter = 0;
        std::string description = {};
        std::string value = {};
        bool required = false;
        bool is_flag = false;
    };

    class argparse_free_node
    {
    public:
        std::string name = {};
        std::string description = {};
        std::string count = {};

    public:
        argparse_free_node(const std::string &name,
                           const std::string &description,
                           std::string count)
            : name(name), description(description), count(count)
        {
        }
    };

    class argparse;

    class argparse_namespace
    {
        std::string first = {};
        std::unordered_map<std::string, std::string> values = {};
        std::unordered_map<std::string, std::vector<std::string>> _freeargs =
            {};

        std::vector<std::string> unknown_keys = {};

    private:
        void set_first(const std::string &first);
        void add_value(const std::string &name, const std::string &value);
        bool is_valid_namespace() const;

        void add_freearg(const std::string &name, const std::string &arg)
        {
            _freeargs[name].push_back(arg);
        }

    public:
        argparse_namespace() {}

        std::string value(const std::string &name);
        bool flag(const std::string &name);

        const std::vector<std::string> &freeargs(std::string name)
        {
            return _freeargs[name];
        }

        operator bool() const
        {
            return is_valid_namespace();
        }

        void init_freearg(std::string name)
        {
            _freeargs[name] = {};
        }

        int print_to(nos::ostream &os) const
        {
            int ret = 0;
            ret += *nos::fprint_to(os, "first: {}\n", first);
            ret += *nos::fprint_to(os, "values:\n");
            for (auto &[name, value] : values)
            {
                ret += *nos::fprint_to(os, "\t{}: {}\n", name, value);
            }
            ret += *nos::fprint_to(os, "free args:\n");
            for (auto &[name, args] : _freeargs)
            {
                ret += *nos::fprint_to(os, "\t{}: {}\n", name, args);
            }
            return ret;
        }

    public:
        friend argparse;
    };

    class argparse
    {
        std::string name = {};
        std::string description = {};
        std::vector<argparse_node> nodes = {};
        std::vector<argparse_free_node> freenodes = {};

    public:
        argparse(const std::string &name, const std::string &description);
        ~argparse() = default;

        void add_argument(const std::string &name,
                          char letter,
                          const std::string &description,
                          const std::string &value,
                          bool required,
                          bool is_flag);

        void add_argument(const std::string &name,
                          char letter,
                          const std::string &description,
                          const std::string &value,
                          bool required);

        void add_argument(const std::string &name,
                          char letter,
                          const std::string &description);

        void add_free_argument(const std::string &name,
                               const std::string &description,
                               const std::string &count);

        argparse_namespace parse_args(int argc, const char *argv[]);
        argparse_namespace parse_args(int argc, char *argv[])
        {
            return parse_args(argc, const_cast<const char **>(argv));
        }

        void print_help();
        std::string help();
    };
}

#endif