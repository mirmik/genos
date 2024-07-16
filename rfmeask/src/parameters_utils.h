/**
    @file
*/

#ifndef PARAMETERS_UTIL_H
#define PARAMETERS_UTIL_H

#include <string>
#include <utility>

enum class parameter_type
{
    PARAMETER_DEC,
    PARAMETER_HEX,
    PARAMETER_FLT,

    INVALID
};

struct mitsuparam_value
{
    int data;
    int decimal_point;
};

mitsuparam_value mitsuparam_value_format(const std::string &value,
                                         const std::string &format,
                                         int dpoint);

parameter_type mitsuparam_parse(const std::string &str);

std::pair<std::string, std::string>
mitsuparam_readed_value_format(int data, int dpoint, int disptype);

#endif