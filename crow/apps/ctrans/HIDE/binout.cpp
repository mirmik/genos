#include "binout.h"
#include "bincommon.h"

#include <crow/print.h>
#include <map>
#include <vector>

#include <string.h>

#include <nos/fprint.h>

#include <igris/util/string.h>

std::string flt32_output(void *tgt)
{
    float arg;
    memcpy(&arg, tgt, 4);
    return nos::format("{}", arg);
}

std::string int64_output(void *tgt)
{
    int64_t arg;
    memcpy(&arg, tgt, 8);
    return nos::format("{}", arg);
}

std::string int32_output(void *tgt)
{
    int32_t arg;
    memcpy(&arg, tgt, 4);
    return nos::format("{}", arg);
}

std::string int16_output(void *tgt)
{
    int16_t arg;
    memcpy(&arg, tgt, 2);
    return nos::format("{}", arg);
}

std::string int8_output(void *tgt)
{
    int8_t arg;
    memcpy(&arg, tgt, 1);
    return nos::format("{}", arg);
}

size_t binout_size = 0;
std::vector<size_t> binout_sizes;
std::vector<std::string (*)(void *)> binout_restores;

std::vector<std::string> binout_format;

std::map<std::string, std::string (*)(void *tgt)> visitor_restore = {
    {"f32", flt32_output},
    {"i64", int64_output},
    {"i32", int32_output},
    {"i16", int16_output},
    {"i8", int8_output}};

void binout_mode_prepare(const std::string &fmt)
{
    binout_format = igris::split(fmt, ',');

    for (auto &str : binout_format)
    {
        try
        {
            binout_size += visitor_size.at(str);
            binout_sizes.push_back(visitor_size.at(str));
            binout_restores.push_back(visitor_restore.at(str));
        }
        catch (std::out_of_range &ex)
        {
            dpr(str);
            dprln(" is not in format list");
            exit(-1);
        }
    }
}

void output_binary(nos::buffer data, crow::packet *pack)
{
    igris::strvec result;

    if (data.size() != binout_size)
    {
        nos::println("WARN: wrong_size");

        nos::print_dump(data);
        crow::diagnostic("binmode", pack);
        return;
    }

    char *ptr = data.data();

    for (unsigned int i = 0; i < binout_format.size(); ++i)
    {
        auto str = binout_restores[i](ptr);
        auto fstr = binout_format[i];
        ptr += binout_sizes[i];

        result.push_back(str);
    }

    nos::println(result);

    fflush(stdout);
    return;
}