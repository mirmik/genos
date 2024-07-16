/**
    @file
*/

#include <MitsuServoAParameter.h>
#include <igris/util/hexascii.h>
#include <nos/trent/json_settings.h>

MitsuServoAParameter::MitsuServoAParameter(
    int gridx, int num, const MitsubishiCommunicator::read_parameter_s &rp)
{

    this->gridx = gridx;
    this->num = num;

    if (rp.disptype == 0)
    {
        type = parameter_type::PARAMETER_HEX;
        u.i32 = rp.data;
    }

    else
    {
        if (rp.dpoint == 0)
        {
            type = parameter_type::PARAMETER_DEC;
            u.i32 = rp.data;
        }

        else
        {
            type = parameter_type::PARAMETER_FLT;
            u.flt = rp.data / pow(10, rp.dpoint - 1);
        }
    }
}

MitsuServoAParameter::MitsuServoAParameter(const nos::trent &tr)
{
    std::string valstr;
    if (tr["value"].is_string())
        valstr = tr["value"].as_string();
    else
        valstr = std::to_string(tr["value"].as_numer());

    const std::string &typestr = tr["type"].as_string();
    const std::string &parname = tr["name"].as_string();

    std::string grpstr = parname.substr(0, 2);
    std::string idxstr = parname.substr(2, 4);

    int grpidx = group_index[grpstr];
    int idxidx = std::stoi(idxstr);

    this->gridx = grpidx;
    this->num = idxidx;

    if (typestr == "HEX")
    {
        type = parameter_type::PARAMETER_HEX;
        u.i32 = hex_to_uint16(valstr.c_str());
    }

    else if (typestr == "DEC")
    {
        auto *dotp = strchr(valstr.c_str(), '.');

        if (dotp == nullptr)
        {
            type = parameter_type::PARAMETER_DEC;
            u.i32 = std::stoi(valstr);
        }
        else
        {
            type = parameter_type::PARAMETER_FLT;
            u.flt = std::stof(valstr);
        }
    }

    else
    {
        BUG();
    }
}

bool MitsuServoAParameter::operator==(const MitsuServoAParameter &oth) const
{
    if (gridx != oth.gridx || num != oth.num || type != oth.type)
        return false;

    switch (type)
    {
    case parameter_type::PARAMETER_DEC:
    case parameter_type::PARAMETER_HEX:
        return u.i32 == oth.u.i32;
    case parameter_type::PARAMETER_FLT:
        return u.flt == oth.u.flt;
    default:
        BUG();
    }

    return true;
}

bool MitsuServoAParameter::operator!=(const MitsuServoAParameter &oth) const
{
    return !(*this == oth);
}

MitsubishiCommunicator::write_parameter_s
MitsuServoAParameter::genwrite(int stantion, int dpoint)
{
    MitsubishiCommunicator::write_parameter_s wr;

    wr.stantion = stantion;
    wr.code = num;
    wr.save = 1;

    if (dpoint == 0)
    {
        wr.dpoint = 0;
        wr.data = u.i32;
    }
    else
    {
        switch (type)
        {
        case parameter_type::PARAMETER_HEX:
        case parameter_type::PARAMETER_DEC:
        {
            wr.dpoint = dpoint;
            wr.data = u.i32 * pow(10, dpoint - 1);
        }
        break;
        case parameter_type::PARAMETER_FLT:
        {
            wr.dpoint = dpoint;
            wr.data = u.flt * pow(10, dpoint - 1) + 0.5;
            // wr.data = IntToBinDec(wr.data);
        }
        break;
        case parameter_type::INVALID:
            break;
        }
    }

    return wr;
}

std::string MitsuServoAParameter::datastring()
{
    switch (type)
    {
    case parameter_type::PARAMETER_DEC:
        return std::to_string(u.i32);
    case parameter_type::PARAMETER_HEX:
        char buf[4];
        uint16_to_hex(buf, u.i32);
        return std::string(buf, 4);
    case parameter_type::PARAMETER_FLT:
        return std::to_string(u.flt);
    case parameter_type::INVALID:
        return "";
    }
    return "";
}

void MitsuServoAParameter::dprint()
{
    std::string dstr = datastring();

    printf("g:%s n:%d t:%s d:%s",
           group_name[gridx].c_str(),
           num,
           to_string(type),
           dstr.c_str());
}
