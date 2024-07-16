#include <nos/util/string.h>
#include <ralgo/cnc/util.h>
#include <ralgo/log.h>

igris::flat_map<int, cnc_float_type>
args_to_index_value_map(const nos::argv &args)
{
    igris::flat_map<int, cnc_float_type> ret;
    int cursor = 0;
    for (unsigned int i = 0; i < args.size(); ++i)
    {
        auto splitlst = nos::split(args[i], ":");

        if (splitlst.size() == 1)
        {
            auto index = cursor;
            auto value = std::stod(splitlst[0]);
            ret.emplace(index, value);
            cursor++;
        }
        else if (splitlst.size() == 2)
        {
            if (isalpha(splitlst[0][0]))
            {
                auto index = cnc::symbol_to_index(splitlst[0][0]);
                auto value = std::stod(splitlst[1]);
                ret.emplace(index, value);
                cursor = index + 1;
            }
            else
            {
                auto index = std::stoi(splitlst[0]);
                auto value = std::stod(splitlst[1]);
                ret.emplace(index, value);
                cursor = index + 1;
            }
        }
        else
        {
            ralgo::warnf("Invalid argument: {}", args[i]);
        }
    }
    return ret;
}