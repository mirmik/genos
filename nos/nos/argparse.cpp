#include <nos/argparse.h>
#include <nos/fprint.h>
#include <nos/io/sstream.h>
#include <nos/print.h>
#include <string_view>

nos::argparse::argparse(const std::string &name, const std::string &description)
    : name(name), description(description)
{
}

void nos::argparse::add_argument(const std::string &name,
                                 char letter,
                                 const std::string &description,
                                 const std::string &value,
                                 bool required,
                                 bool is_flag)
{
    nodes.push_back(
        argparse_node(name, letter, description, value, required, is_flag));
}

void nos::argparse::add_argument(const std::string &name,
                                 char letter,
                                 const std::string &description,
                                 const std::string &value,
                                 bool required)
{
    add_argument(name, letter, description, value, required, false);
}

void nos::argparse::add_argument(const std::string &name,
                                 char letter,
                                 const std::string &description)
{
    add_argument(name, letter, description, "false", false, true);
}

std::string nos::argparse_namespace::value(const std::string &name)
{
    return values[name];
}

bool nos::argparse_namespace::flag(const std::string &name)
{
    return values[name] == "true";
}

void nos::argparse::print_help()
{
    nos::print(help());
}

std::string nos::argparse::help()
{
    nos::stringstream ss;
    nos::fprint_to(ss, "Usage: {} [options] [args]\n", name);
    nos::fprint_to(ss, "{}\n", description);
    nos::fprint_to(ss, "Options:\n");
    for (auto &node : nodes)
    {
        std::string letter = {node.letter};
        if (node.is_flag)
        {
            nos::fprint_to(
                ss, "\t-{}, --{}: {}\n", letter, node.name, node.description);
        }
        else
        {
            nos::fprint_to(ss,
                           "\t-{}, --{} <{}>: {}\n",
                           letter,
                           node.name,
                           "value",
                           node.description);
        }
    }
    return ss.str();
}

std::string nos::argparse_namespace::error() const
{
    nos::stringstream ss;
    nos::fprint_to(ss, "Usage: {} [options] [args]\n", first);
    nos::fprint_to(ss, "Unknown keys:\n");
    for (auto &key : unknown_keys)
    {
        nos::fprint_to(ss, "\t{}\n", key);
    }
    return ss.str();
}

nos::argparse_namespace nos::argparse::parse_args(int argc, const char *argv[])
{
    nos::argparse_namespace ns;
    const char **it = argv + 1;
    const char **eit = argv + argc;
    ns.set_first(argv[0]);

    auto current_free_node = std::begin(freenodes);

    bool wait_for_value = false;
    std::string wait_for_name = {};

    // init default values
    for (auto &node : nodes)
    {
        ns.add_value(node.name, node.value);
    }

    for (auto &node : freenodes)
    {
        ns.init_freearg(node.name);
    }

    for (; it != eit; ++it)
    {
        const std::string &arg = *it;

        if (wait_for_value)
        {
            ns.add_value(wait_for_name, arg);
            wait_for_value = false;
            continue;
        }

        if (arg.starts_with("--"))
        {
            std::string name = arg.substr(2);
            bool found = false;
            for (auto &node : nodes)
            {
                if (node.name == name)
                {
                    if (node.is_flag)
                    {
                        ns.add_value(node.name, "true");
                    }
                    else
                    {
                        wait_for_value = true;
                        wait_for_name = node.name;
                    }
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                ns.unknown_keys.push_back(name);
            }
            else
            {
                continue;
            }
        }
        else if (arg.starts_with("-"))
        {
            std::string name = arg.substr(1);
            bool found = false;
            for (auto &node : nodes)
            {
                if (node.letter == name[0])
                {
                    if (node.is_flag)
                    {
                        ns.add_value(node.name, "true");
                    }
                    else
                    {
                        wait_for_value = true;
                        wait_for_name = node.name;
                    }
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                ns.unknown_keys.push_back(name);
            }
            else
            {
                continue;
            }
        }
        else
        {
        __add_free_value__:
            if (current_free_node == std::end(freenodes))
            {
                ns.unknown_keys.push_back(arg);
                continue;
            }

            auto freevalues = ns.freeargs(current_free_node->name);
            if (current_free_node->count == "*")
            {
                freevalues.push_back(arg);
                ns.add_freearg(current_free_node->name, arg);
            }
            else
            {
                unsigned int count = std::stoul(current_free_node->count);
                if (freevalues.size() < count)
                {
                    freevalues.push_back(arg);
                    ns.add_freearg(current_free_node->name, arg);
                }
                else
                {
                    ++current_free_node;
                    goto __add_free_value__;
                }
            }
        }
    }

    return ns;
}

void nos::argparse_namespace::set_first(const std::string &first)
{
    this->first = first;
}

void nos::argparse_namespace::add_value(const std::string &name,
                                        const std::string &value)
{
    values[name] = value;
}

bool nos::argparse_namespace::is_valid_namespace() const
{
    return unknown_keys.empty();
}

void nos::argparse::add_free_argument(const std::string &name,
                                      const std::string &description,
                                      const std::string &count)
{
    freenodes.push_back({name, description, count});
}