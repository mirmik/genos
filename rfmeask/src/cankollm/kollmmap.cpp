#include <cankollm/kollmmap.h>
#include <map>
#include <stdexcept>

/*	Список параметров. Доступ к параметру осуществляется с помощью
    указанных данных через CAN интерфейс.
*/
std::map<std::string, kollm_param> kollmmap;

// map based on "SERCOS2 S300/S700 manual" and "CANOPEN S300/S700 manual";
kollm_param kollarray[]{
    {"in1", KollmType::INTEGER8, ro, 0x3561, 0x01},
    {"in1mode", KollmType::INTEGER8, rw, 0x3562, 0x01},
    {"in1trig", KollmType::INTEGER32, rw, 0x3563, 0x01},
    {"in2", KollmType::INTEGER8, ro, 0x3564, 0x01},
    {"in2mode", KollmType::INTEGER8, rw, 0x3565, 0x01},
    {"in2trig", KollmType::INTEGER32, rw, 0x3566, 0x01},
    {"in3", KollmType::INTEGER8, ro, 0x3567, 0x01}, // pstop
    {"in3mode", KollmType::INTEGER32, rw, 0x3568, 0x01},
    {"in3trig", KollmType::INTEGER32, rw, 0x3569, 0x01},
    {"in4", KollmType::INTEGER8, ro, 0x356A, 0x01}, // nstop
    {"in4mode", KollmType::INTEGER32, rw, 0x356B, 0x01},
    {"in4trig", KollmType::INTEGER32, rw, 0x356C, 0x01},

    {"control", KollmType::UNSIGNED16, ro, 0x6040, 0x00},
    {"status", KollmType::UNSIGNED16, ro, 0x6041, 0x00},
    {"curvel", KollmType::INTEGER32, ro, 0x606C, 0x00},
    {"tgtvel", KollmType::INTEGER32, rw, 0x60FF, 0x00},
    {"tgtpos", KollmType::INTEGER32, rw, 0x607A, 0x00},
    {"prfvel", KollmType::UNSIGNED32, rw, 0x6081, 0x00},
    {"velencnum", KollmType::UNSIGNED32, ro, 0x6094, 0x01},
    {"velencden", KollmType::UNSIGNED32, ro, 0x6094, 0x02},
    {"maxaccel", KollmType::UNSIGNED32, rw, 0x60C5, 0x00},
    {"pfb", KollmType::INTEGER32, ro, 0x6064, 0x00},
    {"curposz", KollmType::INTEGER32, ro, 0x6063, 0x00},
    {"curpos", KollmType::INTEGER32, ro, 0x6064, 0x00},
    {"o_p", KollmType::INTEGER32, rw, 0x35BE, 0x01},
    {"o_v", KollmType::INTEGER32, rw, 0x35BF, 0x01},
    {"o_c", KollmType::INTEGER32, rw, 0x35B9, 0x01},
    //{"task", KollmType::INTEGER32, rw, 0x360F, 0x01},
    {"move", KollmType::INTEGER32, rw, 0x3642, 0x01},
    {"stop", KollmType::INTEGER32, rw, 0x35FE, 0x01},
    {"opmode", KollmType::INTEGER32, rw, 0x35B4, 0x01},
    {"pgeari", KollmType::UNSIGNED32, rw, 0x35CA, 0x01},
    {"pgearo", KollmType::UNSIGNED32, rw, 0x35CB, 0x01},
    {"pfb", KollmType::UNSIGNED32, ro, 0x35C8, 0x01},
    {"pfblow", KollmType::UNSIGNED32, ro, 0x3875, 0x01},
    {"pfbhi", KollmType::UNSIGNED32, ro, 0x3876, 0x01},
    {"inpos", KollmType::UNSIGNED32, rw, 0x356D, 0x01},
    {"prbase", KollmType::INTEGER8, rw, 0x35D1, 0x01},
    {"roffs", KollmType::INTEGER32, rw, 0x35E7, 0x01},
    {"setref", KollmType::INTEGER32, rw, 0x35F0, 0x01},
    {"nref", KollmType::INTEGER32, rw, 0x35AD, 0x01},
    {"mh", KollmType::INTEGER32, rw, 0x358D, 0x01},
    {"save", KollmType::INTEGER32, rw, 0x35EB, 0x01},
    {"load", KollmType::INTEGER32, rw, 0x3583, 0x01},
    {"brake_disable_delay",
     KollmType::INTEGER32,
     rw,
     0x366E,
     0x01}, // 10...10000
    {"brake_enable_delay",
     KollmType::INTEGER32,
     rw,
     0x366F,
     0x01}, //-10...10000
    {"clrfault", KollmType::INTEGER32, rw, 0x3518, 0x01},
    {"en", KollmType::INTEGER32, rw, 0x3530, 0x01},
    {"dis", KollmType::INTEGER32, rw, 0x352B, 0x01},
};

void kollmmap_init()
{
    for (auto r : kollarray)
    {
        kollmmap[r.mnem] = r;
    }
}

uint8_t kollm_wcode(KollmType tp)
{
    switch (tp)
    {
    case KollmType::INTEGER8:
        return 0x2F;
    case KollmType::UNSIGNED8:
        return 0x2F;
    case KollmType::INTEGER16:
        return 0x2B;
    case KollmType::UNSIGNED16:
        return 0x2B;
    case KollmType::INTEGER32:
        return 0x23;
    case KollmType::UNSIGNED32:
        return 0x23;
    default:
        throw std::runtime_error("unknown type");
    }
}
