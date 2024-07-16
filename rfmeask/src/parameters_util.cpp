/**
    @file
*/

#include <exception.h>
#include <parameters_utils.h>

#include <igris/util/bug.h>
#include <igris/util/numconvert.h>

#include <math.h>
#include <nos/print.h>
#include <sstream>

#include <stack>

template <typename T> std::string int_to_hex(T i)
{
    std::stringstream stream;
    stream << std::hex << std::uppercase << i;
    return stream.str();
}

std::pair<std::string, std::string>
mitsuparam_readed_value_format(int data, int dpoint, int disptype)
{
    if (disptype == 0)
    {
        return {int_to_hex((int)data), "HEX"};
    }

    else
    {
        if (dpoint == 0)
        {
            return {std::to_string((int)data), "DEC"};
        }
        else
        {
            return {std::to_string(data / pow(10, dpoint - 1)), "FLT"};
        }
    }
}

mitsuparam_value mitsuparam_value_format(const std::string &value,
                                         const std::string &format,
                                         int dpoint)
{
    auto param = mitsuparam_parse(format);
    double fvalue;

    switch (param)
    {
    case parameter_type::PARAMETER_FLT:
    case parameter_type::PARAMETER_DEC:
    {
        fvalue = strtof(value.c_str(), NULL);
        break;
    }

    case parameter_type::PARAMETER_HEX:
    {
        fvalue = strtol(value.c_str(), nullptr, 16);
        break;
    }

    case parameter_type::INVALID:
    {
        throw WrongValueException();
    }
    }

    int decimal_point_position = dpoint;

    if (dpoint != 0)
        while (--dpoint)
        {
            fvalue *= 10;
        };

    return {(int)fvalue, decimal_point_position};
}

parameter_type mitsuparam_parse(const std::string &str)
{
    if (str == "HEX")
    {
        return parameter_type::PARAMETER_HEX;
    }

    else if (str == "DEC")
    {
        return parameter_type::PARAMETER_DEC;
    }

    else if (str == "FLT")
    {
        return parameter_type::PARAMETER_FLT;
    }

    return parameter_type::INVALID;
}