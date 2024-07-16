/**
    @file
*/

#ifndef MITSU_A_PARAMETR_H
#define MITSU_A_PARAMETR_H

#include <MitsubishiCommunicator.h>
#include <map>
#include <math.h>
#include <nos/trent/trent.h>
#include <parameters_utils.h>
#include <stack>

extern std::map<int, std::string> group_name;
extern std::map<std::string, int> group_index;

struct MitsuServoAParameter
{
    parameter_type type = {};
    union
    {
        int32_t i32 = 0;
        double flt;
    } u = {};

    int gridx = 0;
    int num = 0;

    MitsuServoAParameter(int gridx,
                         int num,
                         const MitsubishiCommunicator::read_parameter_s &rp);

    MitsuServoAParameter(const nos::trent &tr);

    bool operator==(const MitsuServoAParameter &oth) const;

    bool operator!=(const MitsuServoAParameter &oth) const;

    MitsubishiCommunicator::write_parameter_s genwrite(int stantion,
                                                       int dpoint);
    std::string datastring();
    void dprint();
};

static inline unsigned int IntToBinDec(unsigned int N)
{
    std::stack<int> tmp;
    while (N > 0)
    {
        tmp.push(N % 10);
        N /= 10;
    }
    while (!tmp.empty())
    {
        N *= 16;
        N += tmp.top();
        tmp.pop();
    }
    return N;
}

static inline const char *to_string(parameter_type self)
{
    switch (self)
    {
    case parameter_type::PARAMETER_DEC:
        return "DEC";
    case parameter_type::PARAMETER_HEX:
        return "HEX";
    case parameter_type::PARAMETER_FLT:
        return "FLT";
    case parameter_type::INVALID:
        return "FLT";
    }
    return "";
}

#endif